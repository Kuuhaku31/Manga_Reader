
// console.cpp

#include "console.h"

#include <cstdarg>

Console* Console::instance = nullptr;
Console&
Console::Instance()
{
    if(instance == nullptr) instance = new Console();
    return *instance;
}

void
Console::ConsoleAddLog(const char* fmt, ...)
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
Console::ScrollToBottom()
{
    scroll_to_bottom = true;
}
