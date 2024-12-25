
// show_ui.cpp

#include "imgui_windows.h"

#include "config.h"

static Config& config = Config::Instance();

void
ImGuiWin_UI(bool* is_show)
{
    if(is_show && !*is_show) return;

    ImGuiWin_TopMenu(&config.is_show_menu);
    ImGuiWin_Book();
    ImGuiWin_SelectManga(&config.is_show_manga_list);
    ImGuiWin_Config(&config.is_show_config_window);
    ImGuiWin_LoadResources();
    ImGuiWin_Console(&config.is_show_console_window);
    ImGuiWin_MangaList(&config.is_show_manga_list);
}
