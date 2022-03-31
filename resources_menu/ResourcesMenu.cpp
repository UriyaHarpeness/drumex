#include "ResourcesMenu.h"

ResourcesMenu::ResourcesMenu(filesystem::directory_entry path) : m_path(std::move(path)) {}

Json::Value ResourcesMenu::read_json_file(const string &path) {
    Json::CharReaderBuilder reader;
    Json::Value obj;
    ifstream file(path);

    string errs;
    Json::parseFromStream(reader, file, &obj, &errs);
    file.close();

    return move(obj);
}

void ResourcesMenu::display_path(const filesystem::directory_entry &path) { // NOLINT(misc-no-recursion)
    if (path.is_directory()) {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        bool open = ImGui::TreeNodeEx(path.path().filename().c_str(),
                                      ImGuiTreeNodeFlags_SpanFullWidth);
        ImGui::TableNextColumn();
        ImGui::TextUnformatted("Folder");
        if (open) {
            vector<filesystem::directory_entry> files_in_directory;
            copy(filesystem::directory_iterator(path), filesystem::directory_iterator(),
                 back_inserter(files_in_directory));
            sort(files_in_directory.begin(), files_in_directory.end());

            for (const auto &entry: files_in_directory) {
                display_path(entry);
            }
            ImGui::TreePop();
        }
        return;
    }

    string type;
    string filename = path.path().filename();
    if (filename.ends_with(".part.json")) {
        type = "Part";
    } else if (filename.ends_with(".exercise.json")) {
        type = "Exercise";
    } else {
        return;
    }

    ImGui::TableNextRow();
    ImGui::TableNextColumn();

    bool selected = false;
    ImGui::Selectable(("##" + path.path().string()).c_str(), &selected,
                      ImGuiSelectableFlags_AllowDoubleClick); // FIXME-TABLE: Selection overlap
    ImGui::SameLine(0, 0);
    ImGui::TreeNodeEx(filename.c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet |
                                        ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);
    if (selected) {
        if (m_toggled_files.contains(path.path().string())) {
            m_toggled_files.erase(path.path().string());
        } else {
            m_toggled_files.insert(path.path().string());
        }
    }

    if (m_toggled_files.contains(path.path().string())) {
        auto size = type == "Part" ? ResourcesMenu::read_json_file(path.path())["Parts"].size()
                                   : ResourcesMenu::read_json_file(path.path())["Variations"].size();
        ImGui::BeginGroup();
        // todo: find actual sizes.
        ImGui::BeginChild(path.path().c_str(), ImVec2(0, min(200.f, (float) size * 17 + 12)), true);
        if (ImGui::BeginMenuBar()) {
            ImGui::TextUnformatted("Index");
            ImGui::EndMenuBar();
        }
        for (unsigned int item = 0; item < size; item++) {
            bool index_selected = false;
            ImGui::Selectable((to_string(item) + "##index" + path.path().string()).c_str(), &index_selected,
                              ImGuiSelectableFlags_AllowDoubleClick); // FIXME-TABLE: Selection overlap
            if (index_selected) {
                m_selected = {path.path().string(), item, type == "Part" ? PART : EXERCISE};
            }
        }
        ImGui::EndChild();
        ImGui::EndGroup();
    }

    ImGui::TableNextColumn();
    ImGui::TextUnformatted(type.c_str());
}

void ResourcesMenu::display() {
    ImGui::Text("Select a part/exercise from below.");

    static ImGuiTableFlags flags =
            ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable |
            ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_ScrollX |
            ImGuiTableFlags_ScrollY;

    if (ImGui::BeginTable("Files", 2, flags)) {
        const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
        // todo: support scrolling for long names.
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide | ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 18.0f);
        ImGui::TableHeadersRow();

        display_path(m_path);

        ImGui::EndTable();
    }
}
