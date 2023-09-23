/**
 * @file MainWindow.cpp
 * @author André Lucas Maegima
 * @brief Disklist Main Window Implementations
 * @version 0.2
 * @date 2023-09-19
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include "MainWindow.hpp"
#include "Controllers/Image.hpp"

void getFileInfoList(std::filesystem::directory_entry ent) {
    if (ent.is_directory()) {
        if(ImGui::TreeNode(ent.path().filename().c_str())){
            for (auto const &entry : std::filesystem::directory_iterator{ent.path()}) {
                getFileInfoList(entry);
            }
            ImGui::TreePop();
        }
    } else {
        ImGui::BulletText(ent.path().filename().c_str());    
    }
}

void TextCentered(std::string text, ImVec4 color = ImVec4(1, 1, 1, 1)) {
    bool t;
    ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
    ImGui::PushStyleColor(ImGuiCol_Text, color);
    ImGui::Selectable(text.c_str(), &t, 0, ImVec2(200, 15));
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
}

std::map<std::string, Image*> LoadDefaultImages(std::string confpath){
    std::fstream file(confpath, std::ios::in);
    std::string data;
    std::map<std::string, Image*> images = std::map<std::string, Image*>();
    while(getline(file, data)){
        size_t pos = data.find('=');
        if(pos != std::string::npos){
            Image *img = new Image(data.substr(pos + 1).c_str());
            std::string ext = data.substr(0, pos);
            images.insert({ext, img});
        }
    }
    return images;
}

MainWindow::MainWindow(SDL_Window *window) : window(window) {
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    bool done = false;

    std::filesystem::path current_folder = std::filesystem::current_path();

    auto imgs = LoadDefaultImages(".conf");

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
        for (auto const &entry : std::filesystem::directory_iterator{current_folder}) {
            //getFileInfoList(entry);
            ImGui::BeginGroup();
            Image* img;
            if(entry.is_directory()){
                TextCentered(entry.path().filename(), ImVec4(0.20f, 1.0f, 1.0f, 1.0f));
                img = imgs["folder"];
            }
            else {
                TextCentered(entry.path().filename());
                auto ext = entry.path().extension().c_str();
                if(imgs.contains(ext))
                    img = imgs[ext];
                else
                    img = imgs["default"];
            }
            ImGui::Text("pointer = %p", img->GetTexture());
            ImGui::Text("size = %d x %d", img->width, img->height);
            ImVec2 curr = ImGui::GetCursorPos();
            ImGui::SetCursorPos(ImVec2(curr.x + (200 - 64)*0.5, curr.y));
            ImGui::Image(img->GetTexture(), ImVec2(64, 64));
            //ImGui::SetCursorPos(curr);
            ImGui::PushTextWrapPos((count % 4)*208 + 208);
            ImGui::TextWrapped(entry.path().c_str());
            ImGui::PopTextWrapPos();
            ImGui::EndGroup();
            if(entry.is_directory() && ImGui::IsItemClicked()){
                current_folder = entry.path();
            }
            if(++count % 4 != 0){
                ImGui::SameLine();
            }
        }
        if(ImGui::Button("Back")){
            current_folder = current_folder.parent_path();
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
