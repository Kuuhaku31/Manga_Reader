
// test_bookshelf/main.cpp

#include "bookshelf.h"

#include <stdio.h>

const char* json_path = "mangas.json";

const char* manga_FX_title       = "manga_FX";
const char* manga_FX_volume_path = "D:\\resources\\M\\FX\\[20210721][一般コミック][炭酸だいすき でむにゃん] FX戦士くるみちゃん 1 [MFコミックス フラッパーシリーズ][AVIF][DL版]";

int
main()
{
    printf("Hello, Test Bookshelf!\n");

    Bookshelf::LoadMangas(json_path);

    Bookshelf::Add_manga_volume(manga_FX_title, 0, "right_to_left", manga_FX_volume_path);

    Bookshelf::SaveMangas(json_path);

    return 0;
}
