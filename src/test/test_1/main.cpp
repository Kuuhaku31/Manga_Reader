
#include "bookshelf.h"
#include "config.h"

const char  manga_title_b[] = "ぼっち・ざ・ろっく！";
const char* paths_b[]       = {
    "D:\\manga\\Bocchi The Rock\\DLRAW.TO_Bocchi The Rock vol 01-06\\DLRAW.TO_Bocchi The Rock v01",
    "D:\\manga\\Bocchi The Rock\\DLRAW.TO_Bocchi The Rock vol 01-06\\DLRAW.TO_Bocchi The Rock v02",
    "D:\\manga\\Bocchi The Rock\\DLRAW.TO_Bocchi The Rock vol 01-06\\DLRAW.TO_Bocchi The Rock v03",
    "D:\\manga\\Bocchi The Rock\\DLRAW.TO_Bocchi The Rock vol 01-06\\DLRAW.TO_Bocchi The Rock v04",
    "D:\\manga\\Bocchi The Rock\\DLRAW.TO_Bocchi The Rock vol 01-06\\DLRAW.TO_Bocchi The Rock v05",
    "D:\\manga\\Bocchi The Rock\\DLRAW.TO_Bocchi The Rock vol 01-06\\DLRAW.TO_Bocchi The Rock v06"
};

const char  manga_title_FX[] = "FX戦士くるみちゃん";
const char* path_FX[]{
    "D:\\manga\\FX\\[20210721][一般コミック][炭酸だいすき でむにゃん] FX戦士くるみちゃん 1 [MFコミックス フラッパーシリーズ][AVIF][DL版]",
    "D:\\manga\\FX\\[20220121][一般コミック][炭酸だいすき でむにゃん] FX戦士くるみちゃん 2 [MFコミックス フラッパーシリーズ][AVIF][DL版]",
    "D:\\manga\\FX\\[20220823][一般コミック][炭酸だいすき] FX戦士くるみちゃん 3 [MFコミックス フラッパーシリーズ][AVIF][DL版]",
    "D:\\manga\\FX\\[20230221][一般コミック][炭酸だいすき] FX戦士くるみちゃん 4 [MFコミックス フラッパーシリーズ][AVIF][DL版]",
    "D:\\manga\\FX\\[20230722][一般コミック][炭酸だいすき] FX戦士くるみちゃん 5 [MFコミックス フラッパーシリーズ][AVIF][DL版]",
    "D:\\manga\\FX\\[20240323][一般コミック][炭酸だいすき] FX戦士くるみちゃん 6 [MFコミックス フラッパーシリーズ][AVIF][DL版]"
};

int
main()
{
    // utf-8
    SetConsoleOutputCP(CP_UTF8);

    printf("Hello, Test_1!\n");

    Config&    config    = Config::Instance();
    Bookshelf& bookshelf = Bookshelf::Instance();

    bookshelf.LoadMangas();

    bookshelf.Add_manga(manga_title_FX);

    // bookshelf.Add_manga_volume(manga_title_FX, 0, "right-to-left", "D:\\manga\\FX\\0");
    for(int i = 0; i < sizeof(path_FX) / sizeof(path_FX[0]); i++)
    {
        printf("\n添加 volume %d: %s\n", i, path_FX[i]);
        bookshelf.Add_manga_volume(manga_title_FX, i, "right-to-left", path_FX[i]);
    }

    bookshelf.SaveMangas();

    return 0;
}
