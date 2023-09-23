/**
 * @file MainWindow.cpp
 * @author André Lucas Maegima
 * @brief Disklist Main Window Implementations
 * @version 0.2
 * @date 2023-09-23
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include "MainWindow.hpp"

void TextCentered(std::string text, ImVec4 color = ImVec4(1, 1, 1, 1)) {
    bool t;
    ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
    ImGui::PushStyleColor(ImGuiCol_Text, color);
    ImGui::Selectable(text.c_str(), &t, 0, ImVec2(200, 15));
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
}

MainWindow::MainWindow(const char *title, int width, int height, SDL_WindowFlags window_flags) {
    this->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, window_flags);
    this->gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1);  // Enable vsync

    // Setup ImGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
    io.IniFilename = nullptr;
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(window, nullptr);
    ImGui_ImplOpenGL3_Init(glsl_version);

    DirectoryEntry::InitializeDefaultIcons(".conf");

    this->clear_color = new float[]{0.45f, 0.55f, 0.60f, 1.00f};
    this->done = false;
    this->current_folder = std::filesystem::current_path();
    this->LoadDirectoryEntries();
}

MainWindow::~MainWindow() {
    // ShutDown ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    // Destroy Window
    SDL_GL_DeleteContext(this->gl_context);
    SDL_DestroyWindow(this->window);
}

void MainWindow::ProcessEvent() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT)
            done = true;
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
            done = true;
    }
}

void MainWindow::NewFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void MainWindow::Loop() {
    auto io = ImGui::GetIO();
    while (!this->done) {
        // int x, y;
        // SDL_GetWindowSize(window, &x, &y);
        this->ProcessEvent();

        this->NewFrame();

        ImGui::ShowDemoWindow();

        ImGui::Begin("Show files!", (bool *)__null, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_HorizontalScrollbar);  // Create a window called "Hello, world!" and append into it.
        ImGui::SetWindowSize(ImVec2(856, 600), 0);

        int count = 0;
        for (auto const &entry : *directory_entries) {
            ImGui::BeginGroup();
            TextCentered(entry->path.filename(), *((ImVec4 *)entry->text_color));
            ImVec2 curr = ImGui::GetCursorPos();
            int width = 96 * entry->img->width / entry->img->height;
            if (width > 200) width = 200;
            ImGui::SetCursorPos(ImVec2(curr.x + (200 - width) * 0.5, curr.y));
            ImGui::Image(entry->img->GetTexture(), ImVec2(width, 96));
            ImGui::PushTextWrapPos((count % 4) * 208 + 208);
            ImGui::TextWrapped(entry->path.c_str());
            ImGui::PopTextWrapPos();
            ImGui::EndGroup();
            if (entry->is_directory && ImGui::IsItemClicked()) {
                this->ChangeDirectoryEntries(entry->path);
                break;
            }
            if (++count % 4 != 0) {
                ImGui::SameLine();
            }
        }
        if (ImGui::Button("Back")) {
            this->ChangeDirectoryEntries(current_folder.parent_path());
        }
        ImGui::TextWrapped("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();

        this->Render();
    }
}

void MainWindow::Render() {
    // Rendering
    ImGui::Render();
    auto x = (GLsizei)ImGui::GetMainViewport()->Size.x;
    auto y = (GLsizei)ImGui::GetMainViewport()->Size.y;
    glViewport(0, 0, x, y);
    glClearColor(clear_color[0], clear_color[1], clear_color[2], clear_color[3]);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
}

void MainWindow::LoadDirectoryEntries() {
    directory_entries = new std::list<DirectoryEntry *>();
    for (auto const &entry : std::filesystem::directory_iterator{current_folder}) {
        directory_entries->push_back(new DirectoryEntry(entry));
    }
}

void MainWindow::ChangeDirectoryEntries(std::filesystem::path new_path) {
    this->current_folder = new_path;
    for (auto const &entry : *directory_entries) {
        entry->~DirectoryEntry();
    }
    directory_entries->~list();
    LoadDirectoryEntries();
}