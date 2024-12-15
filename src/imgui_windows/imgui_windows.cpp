
// imgui_windows.cpp

#include "imgui_windows.h"

#include "bookshelf.h"
#include "config.h"
#include "console.h"

static ImGui_setup& imgui     = ImGui_setup::Instance();
static Config&      config    = Config::Instance();
static Bookshelf&   bookshelf = Bookshelf::Instance();
static Console&     console   = Console::Instance();

void
ImGui_Window_Book() // 显示图片
{
    int w, h = 0;
    SDL_QueryTexture(config.tex_page, NULL, NULL, &w, &h); // 获取纹理大小

    ImVec2 output_size(w * config.manga_page_zoom, h * config.manga_page_zoom);

    switch(config.page_outpt_flag)
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

    ImGui::GetBackgroundDrawList()->AddImage((ImTextureID)config.tex_page, config.manga_page_pos, p_max);
}

void
ImGui_Window_config(float& zoom) // 显示配置窗口
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
    if(ImGui::Button("Quit", ImVec2(75, 25))) config.Stop_running();

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
ImGui_Window_Manga_list(bool* is_show) // 显示漫画列表
{
    if(is_show && !*is_show) return;

    ImGui::PushFont(config.font_SmileySans_Oblique);

    ImGui::Begin("Manga List", is_show);

    cJSON* manga = bookshelf.Get_json_root();

    DisplayJSONNode(manga);

    ImGui::End();

    ImGui::PopFont();
}

// Load Resource
void
ImGui_Window_Load_Resource(bool* is_show)
{
    static int selected_manga_idx  = -1;
    static int selected_volume_idx = -1;

    if(is_show && !*is_show) return;

    ImGui::PushFont(config.font_SmileySans_Oblique);

    // 首次出现时居中
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::Begin("加载资源", is_show);

    // // 下拉框，选择漫画
    // cJSON* mangas = bookshelf.Get_json_root();
    // cJSON* manga  = nullptr;
    // if(ImGui::BeginCombo("Manga", cJSON_GetObjectItem(cJSON_GetArrayItem(mangas, selected_manga_idx), "title")->valuestring))
    // {
    //     for(int i = 0; i < cJSON_GetArraySize(mangas); i++)
    //     {
    //         cJSON* manga       = cJSON_GetArrayItem(mangas, i);
    //         bool   is_selected = (selected_manga_idx == i);
    //         if(ImGui::Selectable(cJSON_GetObjectItem(manga, "title")->valuestring, is_selected))
    //         {
    //             selected_manga_idx = i;
    //         }
    //         if(is_selected) ImGui::SetItemDefaultFocus();
    //     }
    //     ImGui::EndCombo();
    // }

    ImGui::End();

    ImGui::PopFont();
}

// showSelectManga
void
ImGui_Window_Select_Manga(bool* is_show)
{
    if(is_show && !*is_show) return;

    ImGui::PushFont(config.font_SmileySans_Oblique);

    // 首次出现时居中
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::Begin("Select Manga", is_show);

    cJSON* mangas = bookshelf.Get_json_root();
    cJSON* manga  = nullptr;

    if(ImGui::BeginCombo("Manga", config.manga_title.c_str()))
    {
        for(int i = 0; i < cJSON_GetArraySize(mangas); i++)
        {
            cJSON* manga       = cJSON_GetArrayItem(mangas, i);
            bool   is_selected = (config.manga_title == cJSON_GetObjectItem(manga, "title")->valuestring);
            if(ImGui::Selectable(cJSON_GetObjectItem(manga, "title")->valuestring, is_selected))
            {
                config.manga_title = cJSON_GetObjectItem(manga, "title")->valuestring;
                config.is_flashing = true;
            }
            if(is_selected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    ImGui::End();

    ImGui::PopFont();
}

void
ImGui_Window_Menu(bool* is_show)
{
    static bool showAboutWindow  = false;
    static bool showLoadResource = false;
    static bool showSelectManga  = false;

    if(is_show && !*is_show) return;

    // 菜单栏
    if(ImGui::BeginMainMenuBar())
    {
        if(ImGui::BeginMenu("File"))
        {
            if(ImGui::BeginMenu("Load", "Ctrl+L"))
            {
                if(ImGui::MenuItem("Load Resource"))
                {
                    showLoadResource = true;
                }
                if(ImGui::MenuItem("Load Config"))
                {
                }
                if(ImGui::MenuItem("Load Manga List"))
                {
                }

                ImGui::EndMenu();
            }
            if(ImGui::MenuItem("Save", "Ctrl+S"))
            {
                printf("Save\n");
            }
            if(ImGui::MenuItem("Exit", "Alt+F4"))
            {
                config.Stop_running();
            }
            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("Manga"))
        {
            showSelectManga = true;
            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("Settings"))
        {
            if(ImGui::MenuItem("config"))
            {
                config.is_show_config_window = true;
            }
            if(ImGui::MenuItem("Manga List"))
            {
                config.is_show_manga_list = true;
            }
            if(ImGui::MenuItem("Console"))
            {
                config.is_show_console_window = true;
            }
            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("Help"))
        {
            if(ImGui::MenuItem("About"))
            {
                showAboutWindow = true;
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    // 显示漫画列表
    ImGui_Window_Manga_list(&config.is_show_manga_list);

    // 显示配置窗口
    if(config.is_show_config_window) ImGui_Window_config(config.manga_page_zoom);

    // 显示控制台
    if(config.is_show_console_window) console.Draw("Console", &config.is_show_console_window);

    // 关于窗口
    if(showAboutWindow)
    {
        // 字体
        ImGui::PushFont(config.font_SmileySans_Oblique);

        ImGui::Begin("About", &showAboutWindow);
        ImGui::Text("Simple Application Menu Example");
        ImGui::Text("Powered by ImGui and SDL2");
        ImGui::Separator();
        ImGui::Text("Author: Kuuhaku Kazari");
        ImGui::End();

        ImGui::PopFont();
    }

    // 加载资源窗口
    ImGui_Window_Load_Resource(&showLoadResource);

    // 选择漫画窗口
    ImGui_Window_Select_Manga(&showSelectManga);
}
