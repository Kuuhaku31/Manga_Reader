
// show_managa_list.cpp

#include "bookshelf.h"
#include "config.h"

static ImGui_setup& imgui     = ImGui_setup::Instance();
static Config&      config    = Config::Instance();
static Bookshelf&   bookshelf = Bookshelf::Instance();

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
ImGuiWin_MangaList(bool* is_show) // 显示漫画列表
{
    if(is_show && !*is_show) return;

    ImGui::PushFont(config.font_SmileySans_Oblique);

    ImGui::Begin("Manga List", is_show);

    cJSON* manga = bookshelf.Get_json_root();

    DisplayJSONNode(manga);

    ImGui::End();

    ImGui::PopFont();
}
