
// console.h

#pragma once

#include <imgui.h>
#include <string>
#include <vector>

// 日志控制台类
class Console
{
public:
    void AddLog(const char* fmt, ...) IM_FMTARGS(2)
    {
        va_list args;
        va_start(args, fmt);
        char buf[1024];
        vsnprintf(buf, sizeof(buf), fmt, args);
        buf[sizeof(buf) - 1] = 0;
        va_end(args);
        logs.push_back(std::string(buf));
    }

    void Draw(const char* title, bool* p_open = NULL)
    {
        ImGui::Begin(title, p_open);
        if(ImGui::Button("Clear"))
        {
            logs.clear();
        }
        ImGui::SameLine();
        bool copy_to_clipboard = ImGui::Button("Copy");
        ImGui::Separator();

        ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
        if(copy_to_clipboard)
        {
            ImGui::LogToClipboard();
        }
        for(const auto& log : logs)
        {
            ImGui::TextUnformatted(log.c_str());
        }
        if(scroll_to_bottom)
        {
            ImGui::SetScrollHereY(1.0f);
        }
        scroll_to_bottom = false;
        ImGui::EndChild();
        ImGui::End();
    }

    void ScrollToBottom()
    {
        scroll_to_bottom = true;
    }

private:
    std::vector<std::string> logs;
    bool                     scroll_to_bottom = false;
};
