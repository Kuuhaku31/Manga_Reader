
// console.h

#pragma once

#include <imgui.h>
#include <string>
#include <vector>

// 日志控制台类
// 单例模式
class Console
{
public:
    static Console& Instance();

public:
    void AddLog(const char* fmt, ...) IM_FMTARGS(2);

    void Draw(const char* title, bool* p_open = NULL);

    void ScrollToBottom();

private:
    std::vector<std::string> logs;

    bool scroll_to_bottom = false;

private:
    Console()                          = default;
    ~Console()                         = default;
    Console(const Console&)            = delete;
    Console& operator=(const Console&) = delete;

    static Console* instance;
};
