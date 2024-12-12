
// main.cpp

#include "imgui_setup.h"

#include "config.h"

#include "console.h"

std::string path = "D:\\manga\\Bocchi The Rock\\DLRAW.TO_Bocchi The Rock vol 01-06\\DLRAW.TO_Bocchi The Rock v01";

static ImGui_setup& imgui = ImGui_setup::Instance();

static Config& config = Config::Instance();

bool is_show_console = true;

struct Page_pair
{
    std::string path_A;
    std::string path_B;

    bool is_swap = false;
};

void
Load_page(const Page_pair& pair, SDL_Texture*& texture) // 加载图片
{
    SDL_DestroyTexture(texture); // 释放纹理

    SDL_Surface* surface_A = nullptr;
    SDL_Surface* surface_B = nullptr;

    // 加载图片
    surface_A = IMG_Load(pair.is_swap ? pair.path_B.c_str() : pair.path_A.c_str());
    surface_B = IMG_Load(pair.is_swap ? pair.path_A.c_str() : pair.path_B.c_str());

    if(surface_A == nullptr || surface_B == nullptr)
    {
        printf("Error: %s\n", IMG_GetError());
        SDL_FreeSurface(surface_A);
        SDL_FreeSurface(surface_B);
        return;
    }

    // 创建一个合并后的表面
    int mergedWidth  = surface_A->w + surface_B->w;
    int mergedHeight = surface_A->h > surface_B->h ? surface_A->h : surface_B->h;

    SDL_Surface* mergedImage = SDL_CreateRGBSurface(0, mergedWidth, mergedHeight, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    if(!mergedImage)
    {
        printf("Error: %s\n", SDL_GetError());
        SDL_FreeSurface(surface_A);
        SDL_FreeSurface(surface_B);
        return;
    }

    // 将两张图片合并到一个表面上
    SDL_Rect destRect1 = { 0, 0, surface_A->w, surface_A->h };
    SDL_Rect destRect2 = { surface_A->w, 0, surface_B->w, surface_B->h };
    SDL_BlitSurface(surface_A, NULL, mergedImage, &destRect1);
    SDL_BlitSurface(surface_B, NULL, mergedImage, &destRect2);

    // 将合并后的表面转换为纹理
    texture = SDL_CreateTextureFromSurface(imgui.renderer, mergedImage);
    if(!texture)
    {
        printf("Error: %s\n", SDL_GetError());
        SDL_FreeSurface(surface_A);
        SDL_FreeSurface(surface_B);
        SDL_FreeSurface(mergedImage);
        return;
    }

    // 设置纹理过滤模式为线性过滤（抗锯齿）
    SDL_SetTextureScaleMode(texture, SDL_ScaleModeLinear);

    // 释放表面
    SDL_FreeSurface(surface_A);
    SDL_FreeSurface(surface_B);
    SDL_FreeSurface(mergedImage);
}

void
ImGui_Window_Book(SDL_Texture* texture, const ImVec2& size) // 显示图片
{
    ImGui::SetNextWindowSize(imgui.io->DisplaySize);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    uint32_t flag = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus;
    ImGui::Begin("Fullscreen Window", NULL, flag);

    if(ImGui::Button("Scroll to Bottom"))
    {
        ImGui::SetScrollY(ImGui::GetScrollMaxY());
        ImGui::SetScrollX(ImGui::GetScrollMaxX());
    }

    ImGui::Image((ImTextureID)texture, size);

    // 添加按钮来调整滚动条位置
    if(ImGui::Button("Scroll to Top"))
    {
        ImGui::SetScrollY(0.0f);
    }

    ImGui::End();
}

void
ImGui_Window_config(bool& is_running, float& zoom) // 显示配置窗口
{
    ImGui::Begin("window data");
    float color[4] = { imgui.clear_color.r / 255.0f, imgui.clear_color.g / 255.0f, imgui.clear_color.b / 255.0f, imgui.clear_color.a / 255.0f };
    ImGui::ColorEdit4("Clear Color", color);
    imgui.clear_color.r = color[0] * 255;
    imgui.clear_color.g = color[1] * 255;
    imgui.clear_color.b = color[2] * 255;
    imgui.clear_color.a = color[3] * 255;

    ImGui::DragFloat("Zoom", &zoom, 0.01f, 0.1f, 10.0f);

    ImGui::Checkbox("Show Console", &is_show_console);

    // 显示帧率
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    is_running = !ImGui::Button("Quit", ImVec2(75, 25));
    ImGui::End();
}

void
ImGui_Window_show_input()
{
}

int
main()
{
    // 设置控制台输出编码为 UTF-8
    SetConsoleOutputCP(CP_UTF8);

    config.Init();

    SDL_Texture* texture = nullptr;

    Page_pair pair;
    int       n = 4;
    config.Get_manga_page(&pair.path_A, 0, 1, n);
    config.Get_manga_page(&pair.path_B, 0, 1, n + 1);
    pair.is_swap = true;
    Load_page(pair, texture);

    float zoom = 1.0f;

    Console console;
    console.AddLog("Welcome to ImGui Console!");

    Event e;
    bool  is_running = true;
    while(is_running)
    {
        imgui.On_frame_begin();


        while(SDL_PollEvent(&e))
        {
            // 添加log
            console.AddLog("Event type: %d", e.type);

            ImGui_ImplSDL2_ProcessEvent(&e);
            if(e.type == SDL_QUIT) is_running = false;

            if(e.type == SDL_KEYDOWN)
            {
                switch(e.key.keysym.sym)
                {
                    case SDLK_ESCAPE: imgui.Exit_fullscreen(); break;
                    case SDLK_F11: imgui.Enter_fullscreen(); break;
                    default: break;
                };
            }
        }

        int size_x, size_y = 0;
        SDL_QueryTexture(texture, nullptr, nullptr, &size_x, &size_y);

        ImVec2 size = { (float)size_x * zoom, (float)size_y * zoom };

        ImGui_Window_Book(texture, size);
        ImGui_Window_config(is_running, zoom);

        if(is_show_console) console.Draw("Console", &is_show_console);

        ImGui::ShowDemoWindow();

        imgui.On_frame_end();
    }

    SDL_DestroyTexture(texture);

    config.Quit();

    return 0;
}
