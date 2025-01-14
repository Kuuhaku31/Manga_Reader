
// show_select_manga.cpp

#include "imgui_windows.h"

#include "bookshelf.h"
#include "config.h"

static Config&    config    = Config::Instance();
static Bookshelf& bookshelf = Bookshelf::Instance();

// showSelectManga
void
ImGuiWin_SelectManga(bool* is_show)
{
    if(is_show && !*is_show) return;

    // 首次出现时居中
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::Begin("Select Manga", is_show);

    cJSON* mangas = bookshelf.Get_json_root();
    cJSON* manga  = nullptr;

    ImGui::PushFont(config.font_msmincho);
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
    ImGui::PopFont();

    ImGui::End();
}
