
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

    cJSON* manga        = bookshelf.Get_json_root();
    cJSON* manga_Bocchi = cJSON_GetArrayItem(manga, 0);
    cJSON* volumes      = cJSON_GetObjectItem(manga_Bocchi, "volumes");
    cJSON* volume_0     = cJSON_GetArrayItem(volumes, 0);
    cJSON* pages_paths  = cJSON_GetObjectItem(volume_0, "pages_paths");

    printf("page_paths: %s\n", cJSON_Print(pages_paths));

    Combine(pages_paths, 0, true, true, "output");

    return 0;
}
