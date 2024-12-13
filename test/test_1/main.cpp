
#include "bookshelf.h"
#include "config.h"

const char  manga_title[] = "ぼっち・ざ・ろっく！";
const char* paths[]       = {
    "D:\\manga\\Bocchi The Rock\\DLRAW.TO_Bocchi The Rock vol 01-06\\DLRAW.TO_Bocchi The Rock v01",
    "D:\\manga\\Bocchi The Rock\\DLRAW.TO_Bocchi The Rock vol 01-06\\DLRAW.TO_Bocchi The Rock v02",
    "D:\\manga\\Bocchi The Rock\\DLRAW.TO_Bocchi The Rock vol 01-06\\DLRAW.TO_Bocchi The Rock v03",
    "D:\\manga\\Bocchi The Rock\\DLRAW.TO_Bocchi The Rock vol 01-06\\DLRAW.TO_Bocchi The Rock v04",
    "D:\\manga\\Bocchi The Rock\\DLRAW.TO_Bocchi The Rock vol 01-06\\DLRAW.TO_Bocchi The Rock v05",
    "D:\\manga\\Bocchi The Rock\\DLRAW.TO_Bocchi The Rock vol 01-06\\DLRAW.TO_Bocchi The Rock v06"
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

    // for(int i = 0; i < 6; i++)
    // {
    //     bookshelf.Add_manga(manga_title);
    //     bookshelf.Add_manga_volume(manga_title, i, "right-to-left", paths[i]);
    // }

    bookshelf.ReLoad_manga(manga_title);

    bookshelf.SaveMangas();

    return 0;
}
