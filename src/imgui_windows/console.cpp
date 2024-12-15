
// console.cpp

#include "console.h"

Console* Console::instance = nullptr;
Console&
Console::Instance()
{
    if(instance == nullptr) instance = new Console();
    return *instance;
}

void
Console::AddLog(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char buf[1024];
    vsnprintf(buf, sizeof(buf), fmt, args);
    buf[sizeof(buf) - 1] = 0;
    va_end(args);
    logs.push_back(std::string(buf));
}

void
Console::Draw(const char* title, bool* p_open)
{
    ImGui::Begin(title, p_open);

    if(ImGui::Button("Clear")) logs.clear();

    ImGui::SameLine();
    bool copy_to_clipboard = ImGui::Button("Copy");

    ImGui::SameLine();
    scroll_to_bottom = ImGui::Button("Scroll to bottom");

    ImGui::Separator(); // 分割线

    ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
    if(copy_to_clipboard)
    {
        ImGui::LogToClipboard();
    }
    for(const auto& log : logs)
    {
        ImGui::TextUnformatted(log.c_str());
    }

    if(scroll_to_bottom) ImGui::SetScrollHereY(1.0f);
    scroll_to_bottom = false;

    ImGui::EndChild();

    ImGui::End();
}

void
Console::ScrollToBottom()
{
    scroll_to_bottom = true;
}
