
// show_load_resources.cpp

#include "imgui_windows.h"

#include "bookshelf.h"
#include "config.h"

static ImGui_setup& imgui     = ImGui_setup::Instance();
static Config&      config    = Config::Instance();
static Bookshelf&   bookshelf = Bookshelf::Instance();

// Load Resource
void
ImGuiWin_LoadResources(bool* is_show)
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
