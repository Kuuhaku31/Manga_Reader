
// test_bookshelf/main.cpp

#define SDL_MAIN_HANDLED

#include "bookshelf.h"
#include "combin_page.h"

#include <filesystem>
#include <iostream>
#include <string>
#include <windows.h>

#include <SDL.h>
#include <SDL_image.h>

const char* json_path = "mangas.json";

const char* manga_FX_title       = "manga_FX";
const char* manga_FX_volume_path = "D:\\resources\\M\\FX\\[20210721][一般コミック][炭酸だいすき でむにゃん] FX戦士くるみちゃん 1 [MFコミックス フラッパーシリーズ][AVIF][DL版]";

const wchar_t* test_path = L"D:\\resources\\M\\FX\\[20210721][一般コミック][炭酸だいすき でむにゃん] FX戦士くるみちゃん 1 [MFコミックス フラッパーシリーズ][AVIF][DL版]_combined\\s.txt";

void
f()
{
    // 检测文件路径
    FILE* file = _wfopen(test_path, L"wb");
    if(!file)
    {
        printf("Error: %s\n", strerror(errno));
        return;
    }
    else
    {
        printf("Success\n");
    }
}

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

    // Setup SDL
    int init_flag = SDL_Init(SDL_INIT_EVERYTHING);
    if(init_flag != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return init_flag;
    }

    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

    // From 2.0.18: Enable native IME.
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

    uint32_t flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;

    SDL_Window* window = SDL_CreateWindow(
        "test_bookshelf",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600,
        flags

    );

    if(window == nullptr)
    {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        init_flag = -1;
        return init_flag;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if(renderer == nullptr)
    {
        SDL_Log("Error creating SDL_Renderer!");
        init_flag = -1;
        return init_flag;
    }

    MakeCombin(renderer, manga_FX_volume_path);
    // stb_test();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
