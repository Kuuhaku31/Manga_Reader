
// console.h

#pragma once

#include <string>
#include <vector>

// 日志控制台类
// 单例模式
class Console
{
public:
    typedef std::vector<std::string> Logs;

public:
    bool scroll_to_bottom = false;

public:
    void ConsoleAddLog(const char* fmt, ...);

    void ScrollToBottom();

public:
    void ClearLogs() { logs.clear(); }

    const Logs& GetLogs() const { return logs; }

private:
    Logs logs;


public:
    static Console& Instance();

private:
    Console()                          = default;
    ~Console()                         = default;
    Console(const Console&)            = delete;
    Console& operator=(const Console&) = delete;

    static Console* instance;
};
