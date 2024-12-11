
// main.cpp

#include <windows.h>

#include "config.h"

std::string path = "D:\\manga\\Bocchi The Rock\\DLRAW.TO_Bocchi The Rock vol 01-06\\DLRAW.TO_Bocchi The Rock v01";

int
main()
{
    // 设置控制台输出编码为 UTF-8
    SetConsoleOutputCP(CP_UTF8);

    Config& c = Config::Instance();
    c.Load();
    // c.Print();

    c.Load_manga_volume(0, 2, path);

    // c.Print_manga(0);

    c.Save();

    return 0;
}
