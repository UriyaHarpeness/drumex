#include "Exercise.h"


Exercise::Exercise(const string &path, int index) {
    Json::Reader reader;
    Json::Value obj;
    ifstream exercise_file(path);

    if (!exercise_file.good()) {
        throw runtime_error("Exercise File Does Not Exist");
    }

    reader.parse(exercise_file, obj);
    exercise_file.close();

    Log(INFO).Get() << "Loading exercise: " << obj["Name"].asString() << "[" << index << "]" << endl;
    Json::Value variation = obj["Variations"][index];

    vector<Part> parts;
    vector<vector<Part>> all_parts;
    for (const auto &parts_variation : variation) {
        if (parts_variation["Indexes"] == "All") {
            for (int i = 0;
                 i < Part::get_parts_number("resources/" + parts_variation["Part"].asString()); parts.emplace_back(
                    "resources/" + parts_variation["Part"].asString(), i++));
        } else {
            for (const auto &i : parts_variation["Indexes"]) {
                parts.emplace_back("resources/" + parts_variation["Part"].asString(), i.asInt());
            }
        }

        for (auto &part : parts) {
            for (const auto &single_variation : parts_variation["Apply"]) {
                Log(INFO).Get() << "Applying variation: " << single_variation["Name"].asString() << endl;
                variations::name_to_variation.at(single_variation["Name"].asString())
                        (part, single_variation["Arguments"]);
            }
        }

        all_parts.push_back(move(parts));
        parts.clear();
    }

    vector<Part> all_parts_flat;
    for (const auto &it : all_parts) {
        all_parts_flat.insert(all_parts_flat.end(), it.begin(), it.end());
    }

    m_part = Part::merge_parts(move(all_parts_flat));
}
