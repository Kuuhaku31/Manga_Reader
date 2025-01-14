
#include "bookshelf.h"
#include "combin_page.h"

#include <stdio.h>
#include <windows.h>

int
main()
{
    SetConsoleOutputCP(CP_UTF8);

    printf("Hello, Test Combine Pages!\n");

    Bookshelf& bookshelf = Bookshelf::Instance();

    bookshelf.LoadMangas();

    cJSON* mangas      = bookshelf.Get_json_root();
    cJSON* manga       = cJSON_GetArrayItem(mangas, 1);
    cJSON* volumes     = cJSON_GetObjectItem(manga, "volumes");
    cJSON* volume      = cJSON_GetArrayItem(volumes, 0);
    cJSON* pages_paths = cJSON_GetObjectItem(volume, "pages_paths");

    Combine(pages_paths, 168, true, true, "fx");

    return 0;
}
