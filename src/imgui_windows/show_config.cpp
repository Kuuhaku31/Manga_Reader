
// show_config.cpp

#include "imgui_windows.h"

#include "bookshelf.h"
#include "config.h"
#include "console.h"

static ImGui_setup& imgui     = ImGui_setup::Instance();
static Config&      config    = Config::Instance();
static Bookshelf&   bookshelf = Bookshelf::Instance();
static Console&     console   = Console::Instance();

void
ImGuiWin_Config(bool* is_show) // 显示配置窗口
{
    if(is_show && !*is_show) return;

    ImGui::Begin("window data", &config.is_show_config_window);
    float color[4] = { config.clear_color.r / 255.0f, config.clear_color.g / 255.0f, config.clear_color.b / 255.0f, config.clear_color.a / 255.0f };
    ImGui::ColorEdit4("Clear Color", color);
    config.clear_color.r = color[0] * 255;
    config.clear_color.g = color[1] * 255;
    config.clear_color.b = color[2] * 255;
    config.clear_color.a = color[3] * 255;

    ImGui::DragFloat("Zoom", &config.manga_page_zoom, 0.01f, 0.1f, 10.0f);

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
