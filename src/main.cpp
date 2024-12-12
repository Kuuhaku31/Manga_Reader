
// main.cpp

#include "imgui_setup.h"

#include "config.h"

std::string path = "D:\\manga\\Bocchi The Rock\\DLRAW.TO_Bocchi The Rock vol 01-06\\DLRAW.TO_Bocchi The Rock v01";

static ImGui_setup& imgui = ImGui_setup::Instance();

static Config& config = Config::Instance();

int
main()
{
    // 设置控制台输出编码为 UTF-8
    SetConsoleOutputCP(CP_UTF8);

    config.Init();

    SDL_Surface* surface = nullptr;
    SDL_Texture* texture = nullptr;

    Event e;
    bool  is_running = true;
    while(is_running)
    {
        imgui.On_frame_begin();

        while(SDL_PollEvent(&e))
        {
            ImGui_ImplSDL2_ProcessEvent(&e);
            if(e.type == SDL_QUIT) is_running = false;
        }

        ImGui::Begin("Manga Book");
        ImGui::Text("Hello, Book!");
        std::string page_path;
        config.Get_manga_page(&page_path, 0, 1, 1);

        // 加载图片
        surface = IMG_Load(page_path.c_str());
        if(surface == nullptr)
        {
            printf("Error: %s\n", IMG_GetError());
            return -1;
        }

        // 创建纹理
        texture = SDL_CreateTextureFromSurface(imgui.renderer, surface);

        // 获取图片大小
        int w, h;
        SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);

        // 显示图片
        ImGui::Image((ImTextureID)texture, ImVec2(w, h));
        ImGui::Text("Page Path: %s", page_path.c_str());
        ImGui::Text("Page w: %d, h: %d", w, h);

        ImGui::End();


        ImGui::Begin("window data");
        float color[4] = { imgui.clear_color.r / 255.0f, imgui.clear_color.g / 255.0f, imgui.clear_color.b / 255.0f, imgui.clear_color.a / 255.0f };
        ImGui::ColorEdit4("Clear Color", color);
        imgui.clear_color.r = color[0] * 255;
        imgui.clear_color.g = color[1] * 255;
        imgui.clear_color.b = color[2] * 255;
        imgui.clear_color.a = color[3] * 255;

        // 显示帧率
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        imgui.On_frame_end();
    }

    // 释放资源
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    config.Quit();

    return 0;
}
