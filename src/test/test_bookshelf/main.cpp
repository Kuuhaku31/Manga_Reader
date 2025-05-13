
// test_bookshelf/main.cpp

#include "bookshelf.h"
#include "combin_page.h"
#include "imgui_setup.h"

#include <filesystem>
#include <iostream>
#include <string>
#include <windows.h>

const char* json_path = "mangas.json";

const char* manga_FX_title       = "manga_FX";
const char* manga_FX_volume_path = "D:\\resources\\M\\FX\\[20220121][一般コミック][炭酸だいすき でむにゃん] FX戦士くるみちゃん 2 [MFコミックス フラッパーシリーズ][AVIF][DL版]";

void
MakeCombin(SDL_Renderer* renderer, std::string volume_path)
{
    cJSON* pages = cJSON_CreateArray();
    Bookshelf::load_volume_pages_paths(pages, volume_path.c_str());

    std::string output_path = volume_path + "_combined";
    std::filesystem::create_directory(output_path);

    Combine(renderer, pages, 0, true, true, output_path.c_str());
    cJSON_Delete(pages);
}

int
main()
{
    SetConsoleOutputCP(CP_UTF8);

    static Painter& painter = Painter::Instance();

    painter.Init("test_bookshelf", IRect{ 0, 0, 1280, 720 });

    MakeCombin(painter.renderer, manga_FX_volume_path);

    painter.Quit();

    return 0;
}
