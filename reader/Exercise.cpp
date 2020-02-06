#include "Exercise.h"


Exercise::Exercise(const string &path, int index) {
    Json::Reader reader;
    Json::Value obj;
    ifstream f(path);
    reader.parse(f, obj);

    cout << "Loading exercise: " << obj["Name"].asString() << "[" << index << "]" << endl;
    Json::Value variation = obj["Variations"][index];

    // todo: change working directory or the dir to search for sources.

    // todo: think about the correct files' structures.
    if (variation[1].empty()) {
        for (int i = 0; i < Part::get_parts_number("../resources/" + variation[0].asString()); m_parts.emplace_back(
                "../resources/" + variation[0].asString(), i++));
    } else {
        for (const auto &i : variation[1]) {
            m_parts.emplace_back("../resources/" + variation[0].asString(), i.asInt());
        }
    }

    for (auto &part : m_parts) {
        for (const auto &single_variation : variation[2]) {
            variations::name_to_variation.at(single_variation["Name"].asString())(part, single_variation["Arguments"]);
        }

    }

    /*Json::Value &links = obj["parse"]["links"];

    auto *str_links = new string[links.end() - links.begin()];*/
}
