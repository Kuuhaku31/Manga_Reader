
// main.cpp

#include "bookshelf.h"
#include "config.h"
#include "console.h"
#include "input.h"

#define D_MOVE 10
#define D_ZOOM 0.01f

static ImGui_setup& imgui     = ImGui_setup::Instance();
static Config&      config    = Config::Instance();
static Input&       input     = Input::Instance();
static Bookshelf&   bookshelf = Bookshelf::Instance();

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

    Load_page(pair, tex_page);
}

void
ImGui_Window_Book(SDL_Texture* texture, PageOutputFlag flag = PageOutputFlag::None) // 显示图片
{
    int w, h = 0;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h); // 获取纹理大小

    ImVec2 output_size(w * config.manga_page_zoom, h * config.manga_page_zoom);

    switch(flag)
    {
    case PageOutputFlag::None:
    {
        break;
    }
    case PageOutputFlag::Center:
    {
        config.manga_page_pos.x = (imgui.io->DisplaySize.x - output_size.x) / 2;
        config.manga_page_pos.y = (imgui.io->DisplaySize.y - output_size.y) / 2;
        break;
    }
    case PageOutputFlag::LeftTop:
    {
        config.manga_page_pos = ImVec2(0, 0);
        break;
    }
    case PageOutputFlag::NormalSize:
    {
        float d_zoom = 1 / config.manga_page_zoom;

        ImVec2 center = ImVec2(imgui.io->DisplaySize.x / 2, imgui.io->DisplaySize.y / 2);

        ImVec2 dv;
        dv.x = center.x - config.manga_page_pos.x;
        dv.y = center.y - config.manga_page_pos.y;

        dv.x = -dv.x * d_zoom;
        dv.y = -dv.y * d_zoom;

        config.manga_page_pos.x = center.x + dv.x;
        config.manga_page_pos.y = center.y + dv.y;


        config.manga_page_zoom = 1.0f;

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
            config.manga_page_zoom  = (float)display_w / w;
            output_size             = ImVec2(display_w, h * config.manga_page_zoom);
            config.manga_page_pos.x = 10;
            config.manga_page_pos.y = (imgui.io->DisplaySize.y - output_size.y) / 2;
        }
        else
        {
            config.manga_page_zoom  = (float)display_h / h;
            output_size             = ImVec2(w * config.manga_page_zoom, display_h);
            config.manga_page_pos.x = (imgui.io->DisplaySize.x - output_size.x) / 2;
            config.manga_page_pos.y = 10;
        }

        break;
    }
    default: break;
    }

    ImVec2 p_max(config.manga_page_pos.x + output_size.x, config.manga_page_pos.y + output_size.y);

    ImGui::GetBackgroundDrawList()->AddImage((ImTextureID)texture, config.manga_page_pos, p_max);
}

void
ImGui_Window_config(bool& is_running, float& zoom) // 显示配置窗口
{
    ImGui::Begin("window data", &config.is_show_config_window);
    float color[4] = { config.clear_color.r / 255.0f, config.clear_color.g / 255.0f, config.clear_color.b / 255.0f, config.clear_color.a / 255.0f };
    ImGui::ColorEdit4("Clear Color", color);
    config.clear_color.r = color[0] * 255;
    config.clear_color.g = color[1] * 255;
    config.clear_color.b = color[2] * 255;
    config.clear_color.a = color[3] * 255;

    ImGui::DragFloat("Zoom", &zoom, 0.01f, 0.1f, 10.0f);

    ImGui::Checkbox("Show Console", &config.is_show_console_window);
    ImGui::SameLine();
    ImGui::Checkbox("Show Demo", &config.is_show_demo_window);

    ImGui::Text("page_index: %d", config.manga_page_idx);

    // 显示帧率
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    is_running = !ImGui::Button("Quit", ImVec2(75, 25));
    ImGui::End();

    if(config.is_show_demo_window) ImGui::ShowDemoWindow(&config.is_show_demo_window);
}

// 递归展示 JSON 节点
void
DisplayJSONNode(cJSON* node, const char* name = nullptr)
{
    if(node == nullptr) return;

    // 根据节点类型显示不同的内容
    if(cJSON_IsObject(node))
    {
        // 对象节点
        if(ImGui::TreeNode(name ? name : "Object"))
        {
            cJSON* child = node->child;
            while(child)
            {
                DisplayJSONNode(child, child->string); // 递归显示子节点
                child = child->next;
            }
            ImGui::TreePop();
        }
    }
    else if(cJSON_IsArray(node))
    {
        // 数组节点
        if(ImGui::TreeNode(name ? name : "Array"))
        {
            int    index = 0;
            cJSON* child = node->child;
            while(child)
            {
                char index_label[16];
                snprintf(index_label, sizeof(index_label), "[%d]", index++);
                DisplayJSONNode(child, index_label); // 递归显示子节点
                child = child->next;
            }
            ImGui::TreePop();
        }
    }
    else
    {
        // 基础类型节点
        if(name) ImGui::Text("%s: ", name); // 显示键名
        if(cJSON_IsString(node))
        {
            ImGui::SameLine();
            ImGui::Text("\"%s\"", node->valuestring);
        }
        else if(cJSON_IsNumber(node))
        {
            ImGui::SameLine();
            ImGui::Text("%g", node->valuedouble);
        }
        else if(cJSON_IsBool(node))
        {
            ImGui::SameLine();
            ImGui::Text("%s", cJSON_IsTrue(node) ? "true" : "false");
        }
        else if(cJSON_IsNull(node))
        {
            ImGui::SameLine();
            ImGui::Text("null");
        }
    }
}


void
ImGui_Window_Manga_list(bool* is_show = nullptr) // 显示漫画列表
{
    if(is_show && !*is_show) return;

    ImGui::PushFont(config.font_SmileySans_Oblique);

    ImGui::Begin("Manga List", is_show);

    cJSON* manga = bookshelf.Get_json_root();

    DisplayJSONNode(manga);

    ImGui::End();

    ImGui::PopFont();
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

        if(input.is_key_c_clicked) config.is_show_config_window = !config.is_show_config_window;
        if(input.is_key_v_clicked) config.is_show_manga_list = !config.is_show_manga_list;

        if(input.is_arrow_right_clicked) Change_page(2);
        if(input.is_arrow_left_clicked) Change_page(-2);
        if(input.is_arrow_up_clicked) Change_page(-1);
        if(input.is_arrow_down_clicked) Change_page(1);

        page_outpt_flag = PageOutputFlag::None;
        if(input.is_key_1_clicked) page_outpt_flag = PageOutputFlag::Center;
        if(input.is_key_2_clicked) page_outpt_flag = PageOutputFlag::LeftTop;
        if(input.is_key_3_clicked) page_outpt_flag = PageOutputFlag::NormalSize;
        if(input.is_key_4_clicked) page_outpt_flag = PageOutputFlag::CenterInWindow;

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

        ImGui_Window_Book(tex_page, page_outpt_flag);

        ImGui_Window_Manga_list(&config.is_show_manga_list);

        if(config.is_show_config_window) ImGui_Window_config(config.is_running, config.manga_page_zoom);

        if(config.is_show_console_window) console.Draw("Console", &config.is_show_console_window);

        imgui.On_frame_end(&config.clear_color);
    }

    SDL_DestroyTexture(tex_page);

    config.Quit();

    return 0;
}
