
// show_console.cpp

#include "config.h"
#include "console.h"

static ImGui_setup& imgui   = ImGui_setup::Instance();
static Config&      config  = Config::Instance();
static Console&     console = Console::Instance();

void
ImGuiWin_Console(bool* is_show)
{
    if(is_show && !*is_show) return;

    ImGui::Begin("Console", is_show);

    if(ImGui::Button("Clear")) console.ClearLogs();

    ImGui::SameLine();
    bool copy_to_clipboard = ImGui::Button("Copy");

    ImGui::SameLine();
    if(ImGui::Button("Scroll to bottom")) console.ScrollToBottom();

    ImGui::Separator(); // 分割线

    ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
    if(copy_to_clipboard)
    {
        ImGui::LogToClipboard();
    }
    for(const auto& log : console.GetLogs())
    {
        ImGui::TextUnformatted(log.c_str());
    }

    if(console.scroll_to_bottom)
    {
        console.scroll_to_bottom = false;

        ImGui::SetScrollHereY(1.0f);
    }

    ImGui::EndChild();

    ImGui::End();
}
