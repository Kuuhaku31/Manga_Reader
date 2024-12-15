
// main.cpp

#include "bookshelf.h"
#include "config.h"
#include "console.h"
#include "imgui_windows.h"
#include "input.h"

#define D_MOVE 10
#define D_ZOOM 0.01f

static ImGui_setup& imgui     = ImGui_setup::Instance();
static Config&      config    = Config::Instance();
static Input&       input     = Input::Instance();
static Bookshelf&   bookshelf = Bookshelf::Instance();
static Console&     console   = Console::Instance();

struct Page_pair
{
    const char* path_A = nullptr;
    const char* path_B = nullptr;
};

void
Load_page(const Page_pair& pair, SDL_Texture*& texture) // 加载图片
{
    SDL_DestroyTexture(texture); // 释放纹理

    int texture_width, texture_height = 0;

    if(pair.path_A && pair.path_B)
    {
        // 加载图片
        SDL_Surface* surface_A = IMG_Load(pair.path_A);
        SDL_Surface* surface_B = IMG_Load(pair.path_B);

        if(!surface_A || !surface_B)
        {
            printf("Error: %s\n", SDL_GetError());
            SDL_FreeSurface(surface_A);
            SDL_FreeSurface(surface_B);
            return;
        }

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
        SDL_FreeSurface(surface_A);
        SDL_FreeSurface(surface_B);
    }
    else if(pair.path_A)
    {
        SDL_Surface* surface_A = IMG_Load(pair.path_A);
        if(!surface_A)
        {
            printf("Error: %s\n", SDL_GetError());
            SDL_FreeSurface(surface_A);
            return;
        }

        texture_width  = surface_A->w;
        texture_height = surface_A->h;

        texture = SDL_CreateTextureFromSurface(imgui.renderer, surface_A);

        SDL_FreeSurface(surface_A);
    }
    else if(pair.path_B)
    {
        SDL_Surface* surface_B = IMG_Load(pair.path_B);
        if(!surface_B)
        {
            printf("Error: %s\n", SDL_GetError());
            SDL_FreeSurface(surface_B);
            return;
        }

        texture_width  = surface_B->w;
        texture_height = surface_B->h;

        texture = SDL_CreateTextureFromSurface(imgui.renderer, surface_B);

        SDL_FreeSurface(surface_B);
    }
    else
    {
        return;
    }

    // 设置纹理过滤模式为线性过滤（抗锯齿）
    SDL_SetTextureScaleMode(texture, SDL_ScaleModeLinear);
}

void
Change_page(int d_page = 0)
{
    Page_pair   pair;
    const char* reading_direction = bookshelf.Get_reading_direction(config.manga_title.c_str(), config.manga_volume_idx);

    // 如果没有阅读方向
    if(!reading_direction) return;

    int page_count = bookshelf.Get_volume_page_count(config.manga_title.c_str(), config.manga_volume_idx);

    if(strcmp(reading_direction, "right-to-left") == 0)
    {
        config.manga_page_idx -= d_page;

        if(config.manga_page_idx < 0) config.manga_page_idx = 0;
        if(config.manga_page_idx >= page_count) config.manga_page_idx = page_count - 1;

        pair.path_B = bookshelf.Get_manga_page(config.manga_title.c_str(), config.manga_volume_idx, config.manga_page_idx);
        pair.path_A = bookshelf.Get_manga_page(config.manga_title.c_str(), config.manga_volume_idx, config.manga_page_idx + 1);
    }
    else
    {
        config.manga_page_idx += d_page;

        if(config.manga_page_idx < 0) config.manga_page_idx = 0;
        if(config.manga_page_idx >= page_count) config.manga_page_idx = page_count - 1;

        pair.path_A = bookshelf.Get_manga_page(config.manga_title.c_str(), config.manga_volume_idx, config.manga_page_idx);
        pair.path_B = bookshelf.Get_manga_page(config.manga_title.c_str(), config.manga_volume_idx, config.manga_page_idx + 1);
    }

    Load_page(pair, config.tex_page);
}

int
main()
{
    // 设置控制台输出编码为 UTF-8
    SetConsoleOutputCP(CP_UTF8);

    config.Init();

    console.AddLog("Welcome to ImGui Console!");

    Change_page();

    while(config.is_running)
    {
        imgui.On_frame_begin();

        if(input.is_escape_clicked)
        {
            if(config.Is_fullscreen())
            {
                config.Make_fullscreen(false);
            }
            else
            {
                config.is_running = false;
            }
        }
        if(input.is_F11_clicked) config.Make_fullscreen(!config.Is_fullscreen());

        if(input.is_key_c_clicked) config.is_show_config_window = !config.is_show_config_window;
        if(input.is_key_v_clicked) config.is_show_manga_list = !config.is_show_manga_list;
        if(input.is_key_m_clicked) config.is_show_menu = !config.is_show_menu;

        if(input.is_arrow_right_clicked) Change_page(2);
        if(input.is_arrow_left_clicked) Change_page(-2);
        if(input.is_arrow_up_clicked) Change_page(-1);
        if(input.is_arrow_down_clicked) Change_page(1);

        config.page_outpt_flag = PageOutputFlag::None;
        if(input.is_key_1_clicked) config.page_outpt_flag = PageOutputFlag::Center;
        if(input.is_key_2_clicked) config.page_outpt_flag = PageOutputFlag::LeftTop;
        if(input.is_key_3_clicked) config.page_outpt_flag = PageOutputFlag::NormalSize;
        if(input.is_key_4_clicked) config.page_outpt_flag = PageOutputFlag::CenterInWindow;

        input.Process_input();

        if(input.is_key_w_pressed) config.manga_page_pos.y += D_MOVE;
        if(input.is_key_s_pressed) config.manga_page_pos.y -= D_MOVE;
        if(input.is_key_a_pressed) config.manga_page_pos.x += D_MOVE;
        if(input.is_key_d_pressed) config.manga_page_pos.x -= D_MOVE;

        float d_zoom = 1.0f;
        if(input.is_comma_pressed) d_zoom = (1 - D_ZOOM);
        if(input.is_period_pressed) d_zoom = (1 + D_ZOOM);

        if(d_zoom != 1.0f)
        {
            config.manga_page_zoom *= d_zoom;

            ImVec2 center = ImVec2(imgui.io->DisplaySize.x / 2, imgui.io->DisplaySize.y / 2);

            ImVec2 dv;
            dv.x = center.x - config.manga_page_pos.x;
            dv.y = center.y - config.manga_page_pos.y;

            dv.x = -dv.x * d_zoom;
            dv.y = -dv.y * d_zoom;

            config.manga_page_pos.x = center.x + dv.x;
            config.manga_page_pos.y = center.y + dv.y;
        }

        ImGui_Window_Book(config.tex_page);

        ImGui_Window_Menu(&config.is_show_menu);

        imgui.On_frame_end(&config.clear_color);
    }

    SDL_DestroyTexture(config.tex_page);

    config.Quit();

    return 0;
}
