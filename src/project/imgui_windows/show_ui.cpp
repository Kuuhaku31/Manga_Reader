
// show_ui.cpp

#include "imgui_windows.h"

#include "config.h"

static Config& config = Config::Instance();

void
ImGuiWin_UI(bool* is_show)
{
    if(is_show && !*is_show) return;

    ImGuiWin_TopMenu(&config.is_show_top_menu);
    ImGuiWin_Config(&config.is_show_config_window);
    ImGuiWin_Console(&config.is_show_console_window);
    ImGuiWin_SelectManga(&config.is_show_select_manga);
    ImGuiWin_LoadResources(&config.is_show_load_resources);
    ImGuiWin_MangaList(&config.is_show_manga_list);
}
