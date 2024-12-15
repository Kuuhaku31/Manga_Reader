
// imgui_windows.cpp

#include "imgui_windows.h"

#include "bookshelf.h"
#include "config.h"

static ImGui_setup& imgui     = ImGui_setup::Instance();
static Config&      config    = Config::Instance();
static Bookshelf&   bookshelf = Bookshelf::Instance();

void
ImGui_Window_Book(SDL_Texture* texture, PageOutputFlag flag) // 显示图片
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
