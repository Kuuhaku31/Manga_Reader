
// show_top_menu.cpp

#include "imgui_windows.h"

#include "bookshelf.h"
#include "config.h"
#include "console.h"

static ImGui_setup& imgui     = ImGui_setup::Instance();
static Config&      config    = Config::Instance();
static Bookshelf&   bookshelf = Bookshelf::Instance();
static Console&     console   = Console::Instance();

void
ImGuiWin_TopMenu(bool* is_show)
{
    static bool showAboutWindow = false;

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
                    config.is_show_load_resources = true;
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
            if(ImGui::MenuItem("Select Manga"))
            {
                config.is_show_select_manga = true;
            }
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
}
