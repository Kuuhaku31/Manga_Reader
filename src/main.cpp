
// main.cpp

#include "imgui_setup.h"

#include "config.h"

#include "console.h"

#define D_MOVE 10
#define D_ZOOM 0.01f

std::string path = "D:\\manga\\Bocchi The Rock\\DLRAW.TO_Bocchi The Rock vol 01-06\\DLRAW.TO_Bocchi The Rock v01";

static ImGui_setup& imgui = ImGui_setup::Instance();

static Config& config = Config::Instance();

bool is_show_console = true;

float  zoom = 0.75f; // 缩放比例
ImVec2 tex_page_pos; // 纹理位置

bool is_arrow_up_pressed    = false;
bool is_arrow_down_pressed  = false;
bool is_arrow_left_pressed  = false;
bool is_arrow_right_pressed = false;

bool is_comma_pressed  = false;
bool is_period_pressed = false;

SDL_Texture* tex_page = nullptr;

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
ImGui_Window_Book(SDL_Texture* texture) // 显示图片
{
    uint32_t flag = 0;
    flag |= ImGuiWindowFlags_NoTitleBar;            // 隐藏标题栏
    flag |= ImGuiWindowFlags_NoResize;              // 禁用调整大小
    flag |= ImGuiWindowFlags_NoMove;                // 禁用移动
    flag |= ImGuiWindowFlags_NoCollapse;            // 禁用折叠
    flag |= ImGuiWindowFlags_NoBringToFrontOnFocus; // 禁用焦点
    flag |= ImGuiWindowFlags_NoScrollbar;           // 隐藏滚动条
    flag |= ImGuiWindowFlags_NoScrollWithMouse;     // 禁止滚动
    flag |= ImGuiWindowFlags_NoDocking;             // 禁用Docking

    int w, h = 0;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h); // 获取纹理大小

    ImVec2 window_size = ImVec2(w * zoom, h * zoom); // 窗口大小

    ImGui::SetNextWindowSize(window_size); // 设置窗口大小
    ImGui::SetNextWindowPos(tex_page_pos); // 设置窗口位置

    ImGui::Begin("Fullscreen Window", NULL, flag);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0)); // 禁用边距
    ImGui::SetCursorPos(ImVec2(0, 0));                              // 设置光标位置
    ImGui::Image((ImTextureID)texture, ImGui::GetWindowSize());     // 显示图片
    ImGui::PopStyleVar();                                           // 恢复边距

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

int
main()
{
    // 设置控制台输出编码为 UTF-8
    SetConsoleOutputCP(CP_UTF8);

    config.Init();

    Page_pair pair;
    int       n = 4;
    config.Get_manga_page(&pair.path_A, 0, 1, n);
    config.Get_manga_page(&pair.path_B, 0, 1, n + 1);
    pair.is_swap = true;
    Load_page(pair, tex_page);

    Console console;
    console.AddLog("Welcome to ImGui Console!");

    Event e;
    bool  is_running = true;
    while(is_running)
    {
        imgui.On_frame_begin();


        while(SDL_PollEvent(&e))
        {
            ImGui_ImplSDL2_ProcessEvent(&e);

            switch(e.type)
            {
            case SDL_QUIT:
            {
                is_running = false;
                break;
            }
            case SDL_WINDOWEVENT:
            {
                if(e.window.event == SDL_WINDOWEVENT_CLOSE && e.window.windowID == imgui.Get_window_id())
                {
                    is_running = false;
                }
                break;
            }
            case SDL_KEYDOWN:
            {
                switch(e.key.keysym.sym)
                {
                case SDLK_ESCAPE: imgui.Exit_fullscreen(); break;
                case SDLK_F11: imgui.Enter_fullscreen(); break;
                case SDLK_MINUS: // "-"
                {
                    zoom -= 0.1f;
                    break;
                }
                case SDLK_EQUALS: // "="
                {
                    zoom += 0.1f;
                    break;
                }
                case SDLK_UP: // 向上
                {
                    is_arrow_up_pressed = true;
                    break;
                }
                case SDLK_DOWN: // 向下
                {
                    is_arrow_down_pressed = true;
                    break;
                }
                case SDLK_LEFT: // 向左
                {
                    is_arrow_left_pressed = true;
                    break;
                }
                case SDLK_RIGHT: // 向右
                {
                    is_arrow_right_pressed = true;
                    break;
                }
                case SDLK_COMMA: // ","
                {
                    is_comma_pressed = true;
                    break;
                }
                case SDLK_PERIOD: //"."
                {
                    is_period_pressed = true;
                    break;
                }
                default: break;
                };
                break;
            }
            case SDL_KEYUP:
            {
                switch(e.key.keysym.sym)
                {
                case SDLK_UP: // 向上
                {
                    is_arrow_up_pressed = false;
                    break;
                }
                case SDLK_DOWN: // 向下
                {
                    is_arrow_down_pressed = false;
                    break;
                }
                case SDLK_LEFT: // 向左
                {
                    is_arrow_left_pressed = false;
                    break;
                }
                case SDLK_RIGHT: // 向右
                {
                    is_arrow_right_pressed = false;
                    break;
                }
                case SDLK_COMMA: // ","
                {
                    is_comma_pressed = false;
                    break;
                }
                case SDLK_PERIOD: //"."
                {
                    is_period_pressed = false;
                    break;
                }
                default: break;
                };
                break;
            }
            case SDL_MOUSEWHEEL:
            {
                break;
            }
            default:
            {
                break;
            }
            }
        }

        if(is_arrow_up_pressed) tex_page_pos.y += D_MOVE;
        if(is_arrow_down_pressed) tex_page_pos.y -= D_MOVE;
        if(is_arrow_left_pressed) tex_page_pos.x += D_MOVE;
        if(is_arrow_right_pressed) tex_page_pos.x -= D_MOVE;

        float d_zoom = 1.0f;
        if(is_comma_pressed) d_zoom = (1 - D_ZOOM);
        if(is_period_pressed) d_zoom = (1 + D_ZOOM);

        if(d_zoom != 1.0f)
        {
            zoom *= d_zoom;

            ImVec2 center = ImVec2(imgui.io->DisplaySize.x / 2, imgui.io->DisplaySize.y / 2);

            ImVec2 dv;
            dv.x = center.x - tex_page_pos.x;
            dv.y = center.y - tex_page_pos.y;

            dv.x = -dv.x * d_zoom;
            dv.y = -dv.y * d_zoom;

            tex_page_pos.x = center.x + dv.x;
            tex_page_pos.y = center.y + dv.y;
        }

        ImGui_Window_Book(tex_page);
        ImGui_Window_config(is_running, zoom);

        if(is_show_console) console.Draw("Console", &is_show_console);

        ImGui::ShowDemoWindow();

        imgui.On_frame_end();
    }

    SDL_DestroyTexture(tex_page);

    config.Quit();

    return 0;
}
