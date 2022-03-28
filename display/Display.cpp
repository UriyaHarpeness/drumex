#include "Display.h"
#include "imgui_internal.h"

Display::Display() {
    // Setup SDL
    // (Some versions of SDL before <2.0.10 appears to have performance/stalling issues on a minority of Windows systems,
    // depending on whether SDL_INIT_GAMECONTROLLER is enabled or disabled.. updating to latest version of SDL is recommended!)
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
        printf("Error: %s\n", SDL_GetError());
    }

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
    // GL 3.2 Core + GLSL 150
    const char *glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags = (SDL_WindowFlags) (SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE |
                                                      SDL_WINDOW_ALLOW_HIGHDPI);
    m_window = SDL_CreateWindow("DrumEX", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                DisplayConstants::initial_window_width, DisplayConstants::initial_window_height,
                                window_flags);
    m_gl_context = SDL_GL_CreateContext(m_window);
    SDL_GL_MakeCurrent(m_window, m_gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    m_io = &ImGui::GetIO();
    (void) m_io;

    // Setup Dear ImGui style
    // todo: setup real style.
    ImGuiStyle &style = ImGui::GetStyle();
    ImGui::StyleColorsLight(&style);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.867f, 0.867f, 0.867f, 1);

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(m_window, m_gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    m_io->Fonts->AddFontDefault();
    m_music_font = m_io->Fonts->AddFontFromFileTTF("/Users/uriya/projects/drumex/Drumex.ttf", 36.0f);
    IM_ASSERT(m_music_font != nullptr);
    m_io->Fonts->Build();
}

Display::~Display() {
    // todo: there has been major improvement in code memory efficiency after calling SDL's destructors wherever needed, may help to look for more leaks.
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(m_gl_context);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

pair<int, int> Display::get_window_size() {
    int w, h;
    SDL_GetWindowSize(m_window, &w, &h);
    return {w, h};
}

void Display::clear_screen() {
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // Content.
    ImGui::SetNextWindowPos({0, 0});
    auto window_size = get_window_size();
    ImGui::SetNextWindowSize({(float) window_size.first, (float) window_size.second});
    ImGui::Begin("Menu", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoCollapse);
}

string Display::resolve_non_ascii(const string &s) {
    string generated;

    for (const char &c: s) {
        if ((unsigned char) c >= 0x80) {
            generated += "\xc2";
        }
        generated += c;
    }

    return generated;
}

void Display::draw_text(const string &text, int x, int y) {
    ImGui::PushFont(m_music_font);
    ImGui::SetCursorPos({(float) x, (float) y});
    ImGui::Text("%s", text.c_str());
    ImGui::PopFont();
}

void Display::draw_text(MusicSymbols value, int x, int y) {
    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    const ImU32 glyph_col = ImGui::GetColorU32(ImGuiCol_Text);
    const float cell_size = m_music_font->FontSize * 1;
    ImVec2 cell_p1((float) x, (float) y);
    m_music_font->RenderChar(draw_list, cell_size, cell_p1, glyph_col, (ImWchar) (value));
}

void Display::draw_text(MusicSymbols value, int x, int staff_y, int line, int off_x, int off_y) {
    draw_text(value, x + off_x, staff_y + (line * DisplayConstants::line_height) + off_y);
}

void Display::draw_text_c(const string &text, int x, int y) {
    ImGui::PushFont(m_music_font);
    auto good = resolve_non_ascii(text);
    auto a = ImGui::CalcTextSize(good.c_str());
    ImGui::PopFont();
    draw_text(good, x - a.x / 2, y - a.y / 2);
}

pair<int, int> Display::get_size(const string &text) {
    ImGui::PushFont(m_music_font);
    auto a = ImGui::CalcTextSize(resolve_non_ascii(text).c_str());
    ImGui::PopFont();

    return {a.x, a.y};
}

void Display::draw_rect(int x, int y, int h, int w, int gray_scale) {
    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilled({(float) x, (float) y}, {(float) x + w, (float) y + h},
                             IM_COL32(gray_scale, gray_scale, gray_scale, 255));
}

void Display::draw_rect(int x, int y, int h, int w, int r, int g, int b, int a) {
    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilled({(float) x, (float) y}, {(float) x + w, (float) y + h}, IM_COL32(r, g, b, a));
}

void Display::draw_rect_c(int x, int y, int h, int w, int gray_scale = 0) {
    draw_rect(x - (w / 2), y - (h / 2), h, w, gray_scale);
}

void Display::draw_base(int y, uint8_t numerator, uint8_t denominator) {
    auto window_size = get_window_size();
    for (uint8_t i = 0; i < 5; i++) {
        draw_rect(DisplayConstants::line_x_offset, y + (DisplayConstants::line_height * (i * 2)), 1,
                  window_size.first - DisplayConstants::line_x_offset * 2, 0);
    }
    draw_rect_c(DisplayConstants::line_x_offset + 16, y + 4, 4, 10, 0);
    draw_rect_c(DisplayConstants::line_x_offset + 16, y + (DisplayConstants::line_height * 8) - 3, 4, 10, 0);
    draw_rect_c(DisplayConstants::line_x_offset + 12, y + (DisplayConstants::line_height * 4), 32, 2, 0);
    draw_rect_c(DisplayConstants::line_x_offset + 20, y + (DisplayConstants::line_height * 4), 32, 2, 0);

    draw_text_c(to_string(numerator), DisplayConstants::line_x_offset + 40, y - 1);
    draw_text_c(to_string(denominator), DisplayConstants::line_x_offset + 40,
                y - 1 + DisplayConstants::line_height * 4);
}

pair<int, int> Display::reset_window_size(int maximum_bar_size) {
    auto window_size = get_window_size();
    pair<int, int> new_size = {
            max(window_size.first,
                DisplayConstants::displaying_init_x + maximum_bar_size + DisplayConstants::line_x_offset),
            max(window_size.second, DisplayConstants::minimum_window_height)};
    if (new_size != window_size) {
        SDL_SetWindowSize(m_window, new_size.first, new_size.second);
    }
    return move(new_size);
}

void Display::present() {
    ImGui::End();

    // Our state
    ImVec4 clear_color = ImVec4(0.3f, 0.3f, 0.3f, 1.00f);

    // Rendering
    ImGui::Render();
    glViewport(0, 0, (int) m_io->DisplaySize.x, (int) m_io->DisplaySize.y);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w,
                 clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(m_window);
}
