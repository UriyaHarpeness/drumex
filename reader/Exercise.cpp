#include "Exercise.h"

Exercise::Exercise(const string &path, int index) {
    Json::CharReaderBuilder reader;
    Json::Value obj;
    ifstream exercise_file(path);

    if (!exercise_file.good()) {
        throw runtime_error("Exercise File Does Not Exist");
    }

    string errs;
    Json::parseFromStream(reader, exercise_file, &obj, &errs);
    exercise_file.close();

    if (obj["Variations"].empty()) {
        throw runtime_error("Could not find \"Variations\"");
    }
    if (index >= obj["Variations"].size()) {
        throw runtime_error(
                "Index " + to_string(index) + " does not exist (maximum " + to_string(obj["Variations"].size() - 1) +
                ")");
    }

    Log(INFO).Get() << "Loading Exercise: " << obj["Name"].asString() << "[" << index << "]" << endl;
    Json::Value variation = obj["Variations"][index];

    vector<Part> parts;
    vector<vector<Part>> all_parts;
    for (const auto &parts_variation: variation["Parts"]) {
        if (parts_variation["Indexes"] == "All") {
            for (int i = 0;
                 i < Part::get_parts_number("resources/" + parts_variation["Part"].asString()); parts.emplace_back(
                    "resources/" + parts_variation["Part"].asString(), i++));
        } else {
            for (const auto &i: parts_variation["Indexes"]) {
                parts.emplace_back("resources/" + parts_variation["Part"].asString(), i.asInt());
            }
        }

        for (auto &part: parts) {
            for (const auto &single_variation: parts_variation["Apply"]) {
                Log(INFO).Get() << "Applying Variation: " << single_variation["Name"].asString() << endl;
                variations::name_to_variation.at(single_variation["Name"].asString())
                        (part, single_variation["Arguments"]);
                location::optimize_location(part.get_mutable_location());
            }
        }

        all_parts.push_back(move(parts));
        parts.clear();
    }

    vector<Part> all_parts_flat;
    for (const auto &it: all_parts) {
        all_parts_flat.insert(all_parts_flat.end(), it.begin(), it.end());
    }

    m_part = merge_parts(move(all_parts_flat), variation["Combination"], variation["Time Signature"]);
}

Part Exercise::merge_parts(vector<Part> parts, const Json::Value &combination, const Json::Value &signature) {
    vector<Locations> locations;
    Locations merged_locations;
    vector<Fraction> lengths;
    Fraction length;

    // merged_signature denominator can't be 1, makes problem with beams and the beat, so currently solve with sort of a patch.
    // todo: solve this in some better way.
    TimeSignature time_signature = signature.isNull() ? parts[0].get_signature() :
                                   TimeSignature(signature[0].asUInt(), signature[1].asUInt());

    if (combination["Type"].asString() == "Merge") {
        for_each(parts.begin(), parts.end(), [&lengths](const Part &part) { lengths.push_back(part.get_length()); });

        Fraction merged_length = NotationUtils::merge_lengths(lengths);

        merged_length = merged_length * Fraction((merged_length % time_signature).get_value().second);

        for (auto &part: parts) {
            part.multiple_length(merged_length);
            locations.push_back(part.get_location());
        }

        merged_locations = location::merge_locations(locations);

        return move(Part(move(merged_locations), time_signature, length));
    }

    map<int, int> parts_bar_indexes;

    for (int i = 0; i < parts.size(); i++) {
        parts_bar_indexes[i] = 0;
        parts[i].multiple_length((parts[i].get_length() % time_signature).get_value().second);
    }

    int sum, part_index, global_bar_index = 0;
    do {
        part_index = combination["Arguments"]["Indexes"][global_bar_index %
                                                         combination["Arguments"]["Indexes"].size()].asInt();
        for (auto location = parts[part_index].get_location().lower_bound(
                Fraction(parts_bar_indexes[part_index]) * time_signature);
             location->first < (Fraction(parts_bar_indexes[part_index] + 1) * time_signature); location++) {
            merged_locations[(Fraction(global_bar_index) * time_signature) + (location->first -
                                                                              (Fraction(parts_bar_indexes[part_index]) *
                                                                               time_signature))] = location->second;
        }

        parts_bar_indexes[part_index] = (parts_bar_indexes[part_index] + 1) %
                                        (int) static_cast<double>(parts[part_index].get_length() / time_signature);

        sum = 0;
        for_each(parts_bar_indexes.begin(), parts_bar_indexes.end(),
                 [&sum](const auto &bar_index) { sum += bar_index.second; });
        global_bar_index++;
    } while (sum || (global_bar_index % combination["Arguments"]["Indexes"].size()));
    for_each(parts.begin(), parts.end(), [&lengths](const Part &part) { lengths.push_back(part.get_length()); });

    Fraction merged_length = Fraction(global_bar_index) * time_signature;
    merged_locations[merged_length] = {};

    return move(Part(move(merged_locations), time_signature, merged_length));
}
