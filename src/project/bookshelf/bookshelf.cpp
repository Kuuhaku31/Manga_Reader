
// bookshelf.cpp

#include "bookshelf.h"

#include <cstring>
#include <filesystem>
#include <fstream>

cJSON* Bookshelf::mangas = nullptr;

void
Bookshelf::LoadMangas(const char* json_file)
{

    cJSON_Delete(mangas); // 释放之前的 mangas

    std::stringstream str_stream; // 字符串流

    std::ifstream file(json_file); // 读取配置文件
    if(!file.good())
    {
        printf("Mangas file not found, creating new one.\n");

        // 创建新的配置文件
        mangas = cJSON_CreateArray();
        return;
    }
    str_stream << file.rdbuf(); // 读取文件内容
    file.close();

    mangas = cJSON_Parse(str_stream.str().c_str()); // 解析JSON

    if(!mangas) // 解析失败
    {
        printf("Mangas file parse failed, creating new one.\n");

        // 创建新的配置文件
        mangas = cJSON_CreateArray();
        return;
    }

    printf("Mangas loaded.\n");
}

void
Bookshelf::SaveMangas(const char* json_file)
{
    // 保存配置文件
    char* s = cJSON_Print(mangas);
    FILE* f = fopen(json_file, "w");
    fwrite(s, strlen(s), 1, f);
    fclose(f);
    free(s);

    printf("Mangas saved.\n");
}

const char*
Bookshelf::Get_manga_page(const char* manga_title, uint32_t target_volume_index, uint32_t target_page_index)
{
    return find_page(find_volume(find_manga(manga_title), target_volume_index), target_page_index); // 查找页
}

const char*
Bookshelf::Get_reading_direction(const char* manga_title, uint32_t target_volume_index)
{
    cJSON* str = cJSON_GetObjectItem(find_volume(find_manga(manga_title), target_volume_index), "reading_direction"); // 获取阅读方向
    return cJSON_IsString(str) ? str->valuestring : nullptr;                                                          // 返回阅读方向
}

uint32_t
Bookshelf::Get_volume_page_count(const char* manga_title, uint32_t target_volume_index)
{
    cJSON* pages = cJSON_GetObjectItem(find_volume(find_manga(manga_title), target_volume_index), "pages_paths"); // 获取页数组
    return cJSON_IsArray(pages) ? cJSON_GetArraySize(pages) : 0;                                                  // 返回页数
}
