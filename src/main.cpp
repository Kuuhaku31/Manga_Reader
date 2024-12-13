
// main.cpp

#include "config.h"
#include "console.h"
#include "input.h"

#define D_MOVE 10
#define D_ZOOM 0.01f

std::string path = "D:\\manga\\Bocchi The Rock\\DLRAW.TO_Bocchi The Rock vol 01-06\\DLRAW.TO_Bocchi The Rock v01";

static ImGui_setup& imgui  = ImGui_setup::Instance();
static Config&      config = Config::Instance();
static Input&       input  = Input::Instance();

bool is_show_console = false;

ImVec2 tex_page_pos; // 纹理位置

enum class PageOutputFlag
{
    None,
    Center,
    LeftTop,
    NormalSize,
    CenterInWindow
};

PageOutputFlag page_outpt_flag = PageOutputFlag::None;

SDL_Texture* tex_page = nullptr;

struct Page_pair
{
    std::string path_A;
    std::string path_B;
};

void
Load_page(const Page_pair& pair, SDL_Texture*& texture) // 加载图片
{
    SDL_DestroyTexture(texture); // 释放纹理

    // 加载图片
    SDL_Surface* surface_A = IMG_Load(pair.path_A.c_str());
    SDL_Surface* surface_B = IMG_Load(pair.path_B.c_str());

    int texture_width, texture_height = 0;

    if(surface_A && surface_B)
    {
        // 创建一个合并后的表面
        texture_width  = surface_A->w + surface_B->w;
        texture_height = surface_A->h > surface_B->h ? surface_A->h : surface_B->h;

        SDL_Surface* mergedImage = SDL_CreateRGBSurface(
            0,
            texture_width,
            texture_height,
            32,
            0x00FF0000,
            0x0000FF00,
            0x000000FF,
            0xFF000000);
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

        SDL_FreeSurface(mergedImage);
    }
    else if(surface_A)
    {
        texture_width  = surface_A->w;
        texture_height = surface_A->h;

        texture = SDL_CreateTextureFromSurface(imgui.renderer, surface_A);
    }
    else if(surface_B)
    {
        texture_width  = surface_B->w;
        texture_height = surface_B->h;

        texture = SDL_CreateTextureFromSurface(imgui.renderer, surface_B);
    }
    else
    {
        return;
    }

    // 设置纹理过滤模式为线性过滤（抗锯齿）
    SDL_SetTextureScaleMode(texture, SDL_ScaleModeLinear);

    // 释放表面
    SDL_FreeSurface(surface_A);
    SDL_FreeSurface(surface_B);
}

void
Change_page(int d_page = 0)
{
    static int page_index = 0;

    Page_pair   pair;
    std::string reading_direction = config.Get_reading_direction(0, 1);
    int         page_count        = config.Get_volume_page_count(0, 1);

    if(reading_direction == "right-to-left")
    {
        page_index -= d_page;

        if(page_index < 0) page_index = 0;

        config.Get_manga_page(&pair.path_B, 0, 1, page_index);
        config.Get_manga_page(&pair.path_A, 0, 1, page_index + 1);
    }
    else
    {
        page_index += d_page;

        if(page_index >= page_count) page_index = page_count - 1;

        config.Get_manga_page(&pair.path_A, 0, 1, page_index);
        config.Get_manga_page(&pair.path_B, 0, 1, page_index + 1);
    }

    Load_page(pair, tex_page);
}

void
ImGui_Window_Book(SDL_Texture* texture, PageOutputFlag flag = PageOutputFlag::None) // 显示图片
{
    int w, h = 0;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h); // 获取纹理大小

    ImVec2 output_size(w * config.page_zoom, h * config.page_zoom);

    switch(flag)
    {
    case PageOutputFlag::None:
    {
        break;
    }
    case PageOutputFlag::Center:
    {
        tex_page_pos.x = (imgui.io->DisplaySize.x - output_size.x) / 2;
        tex_page_pos.y = (imgui.io->DisplaySize.y - output_size.y) / 2;
        break;
    }
    case PageOutputFlag::LeftTop:
    {
        tex_page_pos = ImVec2(0, 0);
        break;
    }
    case PageOutputFlag::NormalSize:
    {
        float d_zoom = 1 / config.page_zoom;

        ImVec2 center = ImVec2(imgui.io->DisplaySize.x / 2, imgui.io->DisplaySize.y / 2);

        ImVec2 dv;
        dv.x = center.x - tex_page_pos.x;
        dv.y = center.y - tex_page_pos.y;

        dv.x = -dv.x * d_zoom;
        dv.y = -dv.y * d_zoom;

        tex_page_pos.x = center.x + dv.x;
        tex_page_pos.y = center.y + dv.y;


        config.page_zoom = 1.0f;

        output_size = ImVec2(w, h);
        break;
    }
    case PageOutputFlag::CenterInWindow:
    {
        float ratio_page = (float)w / h;
        int   display_w  = imgui.io->DisplaySize.x - 20;
        int   display_h  = imgui.io->DisplaySize.y - 20;
        float ratio_win  = (float)display_w / display_h;

        if(ratio_page > ratio_win)
        {
            config.page_zoom = (float)display_w / w;
            output_size      = ImVec2(display_w, h * config.page_zoom);
            tex_page_pos.x   = 10;
            tex_page_pos.y   = (imgui.io->DisplaySize.y - output_size.y) / 2;
        }
        else
        {
            config.page_zoom = (float)display_h / h;
            output_size      = ImVec2(w * config.page_zoom, display_h);
            tex_page_pos.x   = (imgui.io->DisplaySize.x - output_size.x) / 2;
            tex_page_pos.y   = 10;
        }

        break;
    }
    default: break;
    }

    ImVec2 p_max(tex_page_pos.x + output_size.x, tex_page_pos.y + output_size.y);

    ImGui::GetBackgroundDrawList()->AddImage((ImTextureID)texture, tex_page_pos, p_max);
}

void
ImGui_Window_config(bool& is_running, float& zoom) // 显示配置窗口
{
    ImGui::Begin("window data");
    float color[4] = { config.clear_color.r / 255.0f, config.clear_color.g / 255.0f, config.clear_color.b / 255.0f, config.clear_color.a / 255.0f };
    ImGui::ColorEdit4("Clear Color", color);
    config.clear_color.r = color[0] * 255;
    config.clear_color.g = color[1] * 255;
    config.clear_color.b = color[2] * 255;
    config.clear_color.a = color[3] * 255;

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

    Change_page(0);

    Console console;
    console.AddLog("Welcome to ImGui Console!");

    while(config.is_running)
    {
        imgui.On_frame_begin();

        if(input.is_arrow_right_clicked) Change_page(2);
        if(input.is_arrow_left_clicked) Change_page(-2);
        if(input.is_arrow_up_clicked) Change_page(1);
        if(input.is_arrow_down_clicked) Change_page(-1);

        page_outpt_flag = PageOutputFlag::None;
        if(input.is_key_1_clicked) page_outpt_flag = PageOutputFlag::Center;
        if(input.is_key_2_clicked) page_outpt_flag = PageOutputFlag::LeftTop;
        if(input.is_key_3_clicked) page_outpt_flag = PageOutputFlag::NormalSize;
        if(input.is_key_4_clicked) page_outpt_flag = PageOutputFlag::CenterInWindow;

        input.Process_input();

        if(input.is_key_w_pressed) tex_page_pos.y += D_MOVE;
        if(input.is_key_s_pressed) tex_page_pos.y -= D_MOVE;
        if(input.is_key_a_pressed) tex_page_pos.x += D_MOVE;
        if(input.is_key_d_pressed) tex_page_pos.x -= D_MOVE;

        float d_zoom = 1.0f;
        if(input.is_comma_pressed) d_zoom = (1 - D_ZOOM);
        if(input.is_period_pressed) d_zoom = (1 + D_ZOOM);

        if(d_zoom != 1.0f)
        {
            config.page_zoom *= d_zoom;

            ImVec2 center = ImVec2(imgui.io->DisplaySize.x / 2, imgui.io->DisplaySize.y / 2);

            ImVec2 dv;
            dv.x = center.x - tex_page_pos.x;
            dv.y = center.y - tex_page_pos.y;

            dv.x = -dv.x * d_zoom;
            dv.y = -dv.y * d_zoom;

            tex_page_pos.x = center.x + dv.x;
            tex_page_pos.y = center.y + dv.y;
        }

        ImGui_Window_Book(tex_page, page_outpt_flag);
        ImGui_Window_config(config.is_running, config.page_zoom);

        if(is_show_console) console.Draw("Console", &is_show_console);

        // ImGui::ShowDemoWindow();

        imgui.On_frame_end();
    }

    SDL_DestroyTexture(tex_page);

    config.Quit();

    return 0;
}
