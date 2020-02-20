#include "Exercise.h"


Exercise::Exercise(const string &path, int index) {
    Json::Reader reader;
    Json::Value obj;
    ifstream f(path);

    if (!f.good()) {
        throw runtime_error("Exercise File Does Not Exist");
    }

    reader.parse(f, obj);
    f.close();

    cout << "Loading exercise: " << obj["Name"].asString() << "[" << index << "]" << endl;
    Json::Value variation = obj["Variations"][index];

    if (variation.isNull()) {
        throw runtime_error("Invalid Variation Index");
    }

    vector<Part> parts;
    vector<vector<Part>> all_parts;
    for (const auto &parts_variation : variation) {
        if (parts_variation[1].empty()) {
            for (int i = 0;
                 i < Part::get_parts_number("resources/" + parts_variation[0].asString()); parts.emplace_back(
                    "resources/" + parts_variation[0].asString(), i++));
        } else {
            for (const auto &i : parts_variation[1]) {
                parts.emplace_back("resources/" + parts_variation[0].asString(), i.asInt());
            }
        }

        for (auto &part : parts) {
            for (const auto &single_variation : parts_variation[2]) {
                cout << "Applying variation: " << single_variation["Name"].asString() << endl;
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
