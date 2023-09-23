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
#include <filesystem>
#include <list>
#include <iostream>
#include "MainWindow.hpp"
#include "Controllers/Image.hpp"
#include "Controllers/DirectoryEntry.hpp"

void TextCentered(std::string text, ImVec4 color = ImVec4(1, 1, 1, 1)) {
    bool t;
    ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
    ImGui::PushStyleColor(ImGuiCol_Text, color);
    ImGui::Selectable(text.c_str(), &t, 0, ImVec2(200, 15));
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
}

std::list<DirectoryEntry*> *LoadEntries(std::filesystem::path path){
    auto entries = new std::list<DirectoryEntry*>();
    for (auto const &entry : std::filesystem::directory_iterator{path}) {
        entries->push_back(new DirectoryEntry(entry));
    }
    return entries;
}

MainWindow::MainWindow(SDL_Window *window) : window(window) {
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    bool done = false;

    std::filesystem::path current_folder = std::filesystem::current_path();

    DirectoryEntry::InitializeDefaultIcons(".conf");
    auto entries = LoadEntries(current_folder);

    while (!done) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        int x, y;
        SDL_GetWindowSize(window, &x, &y);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

        ImGui::Begin("Show files!", (bool*)__null, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_HorizontalScrollbar);                          // Create a window called "Hello, world!" and append into it.
        ImGui::SetWindowSize(ImVec2(856, 600), 0);

        int count = 0;
        for (auto const &entry : *entries) {
            ImGui::BeginGroup();
            TextCentered(entry->path.filename(), *((ImVec4*) entry->text_color));
            ImVec2 curr = ImGui::GetCursorPos();
            int width = 96*entry->img->width/entry->img->height;
            if(width > 200) width = 200;
            ImGui::SetCursorPos(ImVec2(curr.x + (200 - width)*0.5, curr.y));
            ImGui::Image(entry->img->GetTexture(), ImVec2(width, 96));
            ImGui::PushTextWrapPos((count % 4)*208 + 208);
            ImGui::TextWrapped(entry->path.c_str());
            ImGui::PopTextWrapPos();
            ImGui::EndGroup();
            if(entry->is_directory && ImGui::IsItemClicked()){
                current_folder = entry->path;
                for (auto const &entry : *entries) {
                    entry->~DirectoryEntry();
                }
                entries->~list();
                entries = LoadEntries(current_folder);
                break;
            }
            if(++count % 4 != 0){
                ImGui::SameLine();
            }
        }
        if(ImGui::Button("Back")){
            current_folder = current_folder.parent_path();
            for (auto const &entry : *entries) {
                entry->~DirectoryEntry();
            }
            entries->~list();
            entries = LoadEntries(current_folder);
        }

        ImGuiIO& io = ImGui::GetIO();
        ImGui::TextWrapped("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();


        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }
}
