#pragma once

#include <filesystem>
#include <fstream>
#include <json/json.h>
#include <set>

#include "imgui.h"

using namespace std;


enum ResourceType {
    PART,
    EXERCISE
};

class ResourcesMenu {
public:
    ResourcesMenu(filesystem::directory_entry path);

    static Json::Value read_json_file(const string &path);

    void display_path(const filesystem::directory_entry &path);

    void display();

    [[nodiscard]] optional<tuple<string, unsigned int, ResourceType>> selected() const { return m_selected; };

private:
    filesystem::directory_entry m_path;

    optional<tuple<string, unsigned int, ResourceType>> m_selected;

    set<string> m_toggled_files;
};
