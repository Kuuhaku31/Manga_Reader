
// bookshelf.cpp

#include "bookshelf.h"

#include <algorithm>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <vector>

void
load_volume_pages_paths(cJSON* pages_paths, const char* volume_path)
{
    // 加载卷
    namespace fs = std::filesystem;
    if(!fs::exists(volume_path)) // 如果路径不存在
    {
        printf("Volume path not exists: %s\n", volume_path);
        return;
    }

    std::vector<std::string> volume_pages_paths;
    // 遍历目录中的所有文件
    for(const auto& entry : fs::directory_iterator(volume_path))
    {
        // 如果是文件
        if(entry.is_regular_file())
        {
            // 获取文件路径和扩展名
            std::string file_path = entry.path().string();
            std::string extension = entry.path().extension().string();

            // 检查文件扩展名是否为图片格式
            if(extension == ".jpg" || extension == ".png" || extension == ".jpeg" || extension == ".bmp" || extension == ".gif")
            {
                // 将图片路径存储到 volume_pages 容器中
                volume_pages_paths.push_back(file_path);
            }
        }
    }

    // 如果 volume_pages 为空，表示加载失败
    if(volume_pages_paths.empty())
    {
        printf("Volume pages empty: %s\n", volume_path);
        return;
    }

    // 重小到大排序
    std::sort(volume_pages_paths.begin(), volume_pages_paths.end());

    // 添加到 pages 数组
    for(int i = 0; i < volume_pages_paths.size(); i++)
    {
        cJSON_AddItemToArray(pages_paths, cJSON_CreateString(volume_pages_paths[i].c_str()));
    }
}

Bookshelf* Bookshelf::instance = nullptr;
Bookshelf&
Bookshelf::Instance()
{
    if(instance == nullptr) instance = new Bookshelf();
    return *instance;
}

void
Bookshelf::LoadMangas()
{
    // 释放之前的 mangas
    cJSON_Delete(mangas);

    // 读取配置文件
    std::ifstream file("mangas.json");
    if(!file.good())
    {
        printf("Mangas file not found, creating new one.\n");

        // 创建新的配置文件
        mangas = cJSON_CreateArray();
        return;
    }

    std::stringstream str_stream; // 字符串流
    str_stream << file.rdbuf();   // 读取文件内容
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
Bookshelf::SaveMangas()
{
    // 保存配置文件
    char* s = cJSON_Print(mangas);
    FILE* f = fopen("mangas.json", "w");
    fwrite(s, strlen(s), 1, f);
    fclose(f);
    free(s);

    printf("Mangas saved.\n");
}

const char*
Bookshelf::Get_manga_page(const char* manga_title, int target_volume_index, int target_page_index) const
{
    cJSON* manga = find_manga(manga_title); // 查找漫画
    if(!manga) return nullptr;              // 如果漫画不存在

    cJSON* volume = find_volume(manga, target_volume_index); // 查找卷
    if(!volume) return nullptr;                              // 如果卷号不存在

    return find_page(volume, target_page_index); // 查找页
}

const char*
Bookshelf::Get_reading_direction(const char* manga_title, int target_volume_index) const
{
    cJSON* manga = find_manga(manga_title); // 查找漫画
    if(!manga) return nullptr;              // 如果漫画不存在

    cJSON* volume = find_volume(manga, target_volume_index); // 查找卷
    if(!volume) return nullptr;                              // 如果卷号不存在

    return cJSON_GetObjectItem(volume, "reading_direction")->valuestring; // 获取阅读方向
}

int
Bookshelf::Get_volume_page_count(const char* manga_title, int target_volume_index) const
{
    cJSON* manga = find_manga(manga_title); // 查找漫画
    if(!manga) return 0;                    // 如果漫画不存在

    cJSON* volume = find_volume(manga, target_volume_index); // 查找卷
    if(!volume) return 0;                                    // 如果卷号不存在

    cJSON* pages = cJSON_GetObjectItem(volume, "pages_paths"); // 获取页数组
    return cJSON_GetArraySize(pages);                          // 返回页数
}

void
Bookshelf::Add_manga(const char* manga_title)
{
    printf("Adding manga: %s\n", manga_title);

    // 检查漫画是否已存在
    cJSON* new_manga = nullptr;
    cJSON_ArrayForEach(new_manga, mangas)
    {
        // 获取 "title" 的值
        const char* t = cJSON_GetObjectItem(new_manga, "title")->valuestring;

        // 如果标题相同
        if(strcmp(t, manga_title) == 0)
        {
            printf("Manga already exists: %s\n", manga_title);
            return;
        }
    }

    // 创建新漫画
    new_manga = cJSON_CreateObject();
    cJSON_AddItemToArray(mangas, new_manga);

    // 添加标题
    cJSON_AddStringToObject(new_manga, "title", manga_title);
    // 添加卷数组
    cJSON_AddItemToObject(new_manga, "volumes", cJSON_CreateArray());

    printf("Manga added: %s\n", manga_title);
}

void
Bookshelf::ReLoad_manga(const char* manga_title)
{
    printf("Reloading manga: %s\n", manga_title);

    cJSON* manga = find_manga(manga_title); // 查找漫画
    if(!manga) return;                      // 如果漫画不存在

    cJSON* volumes = cJSON_GetObjectItem(manga, "volumes"); // 获取卷数组
    for(int i = 0; i < cJSON_GetArraySize(volumes); i++)
    {
        ReLoad_manga_volume(manga_title, i); // 重新加载卷
    }
}

void
Bookshelf::Del_manga(const char* manga_title)
{
    printf("Deleting manga: %s\n", manga_title);

    // 遍历mangas数组
    for(int i = 0; i < cJSON_GetArraySize(mangas); i++)
    {
        // 获取当前漫画的对象
        cJSON* manga = cJSON_GetArrayItem(mangas, i);
        cJSON* title = cJSON_GetObjectItem(manga, "title");

        // 如果标题相同
        if(strcmp(title->valuestring, manga_title) == 0)
        {
            cJSON_DeleteItemFromArray(mangas, i);
            printf("Manga deleted: %s\n", manga_title);
            return;
        }
    }

    printf("Manga not found: %s\n", manga_title);
}

void
Bookshelf::Add_manga_volume(const char* manga_title, int volume_index, const char* reading_direction, const char* volume_path)
{
    cJSON* manga = nullptr;

    // 遍历mangas数组
    bool manga_not_found = true;
    cJSON_ArrayForEach(manga, mangas)
    {
        // 获取 "title" 的值
        const char* title = cJSON_GetObjectItem(manga, "title")->valuestring;

        // 如果标题相同
        if(strcmp(title, manga_title) == 0)
        {
            manga_not_found = false;
            break;
        }
    }

    // 如果漫画不存在
    if(manga_not_found)
    {
        printf("Manga not found: %s\n", manga_title);
        return;
    }

    cJSON* volumes     = cJSON_GetObjectItem(manga, "volumes");
    cJSON* new_volume  = cJSON_CreateObject();
    bool   is_inserted = false;

    // 遍历 volumes 数组
    for(int i = 0; i < cJSON_GetArraySize(volumes); i++)
    {
        // 获取当前卷的对象
        cJSON* volume          = cJSON_GetArrayItem(volumes, i);
        cJSON* volume_num_item = cJSON_GetObjectItem(volume, "volume");

        // 如果 "volume" 字段不是数字
        if(!cJSON_IsNumber(volume_num_item))
        {
            printf("Volume number not found.\n");
            return;
        }

        int volume_num = volume_num_item->valueint;

        // 如果卷号相同，更新卷
        if(volume_num == volume_index)
        {
            printf("Volume %d already exists, updating...\n", volume_index);

            // 删除现有卷
            cJSON_DeleteItemFromArray(volumes, i);

            // 插入新卷
            cJSON_InsertItemInArray(volumes, i, new_volume);
            is_inserted = true;
            break;
        }
        // 如果当前卷号大于目标卷号，则插入新卷
        else if(volume_num > volume_index)
        {
            printf("Inserting volume %d before volume %d...\n", volume_index, volume_num);

            // 插入新卷
            cJSON_InsertItemInArray(volumes, i, new_volume);
            is_inserted = true;
            break;
        }
    }

    // 如果没有插入新卷，直接添加到数组末尾
    if(!is_inserted)
    {
        printf("Volume %d doesn't exist, adding to the end...\n", volume_index);
        cJSON_AddItemToArray(volumes, new_volume);
    }

    // 添加卷号
    cJSON_AddNumberToObject(new_volume, "volume", volume_index);
    // 添加路径
    cJSON_AddStringToObject(new_volume, "volume_path", volume_path);
    // 添加描述
    char description[256];
    sprintf(description, "第%d卷", volume_index + 1);
    cJSON_AddStringToObject(new_volume, "description", description);
    // 添加阅读方向
    cJSON_AddStringToObject(new_volume, "reading_direction", reading_direction);
    // 创建页数组
    cJSON* pages_paths = cJSON_CreateArray();
    cJSON_AddItemToObject(new_volume, "pages_paths", pages_paths);

    // 加载卷页路径
    load_volume_pages_paths(pages_paths, volume_path);

    // 如果 pages 为空，表示加载失败
    if(cJSON_GetArraySize(pages_paths) == 0)
    {
        printf("Volume pages empty: %s\n", volume_path);
        return;
    }

    // 将第一页设置为封面
    const char* cover_path = cJSON_GetArrayItem(pages_paths, 0)->valuestring;
    cJSON_AddStringToObject(new_volume, "cover", cover_path);

    printf("Volume %d (%s) loaded: %d pages.\n", volume_index, description, cJSON_GetArraySize(pages_paths));
}

void
Bookshelf::ReLoad_manga_volume(const char* manga_title, int volume_index)
{
    printf("Reloading volume %d in manga: %s\n", volume_index, manga_title);

    cJSON* manga = find_manga(manga_title); // 查找漫画
    if(!manga) return;                      // 如果漫画不存在

    cJSON* volume = find_volume(manga, volume_index); // 查找卷
    if(!volume) return;                               // 如果卷号不存在

    // 获取卷路径
    const char* volume_path       = cJSON_GetStringValue(cJSON_GetObjectItem(volume, "volume_path"));
    const char* reading_direction = cJSON_GetStringValue(cJSON_GetObjectItem(volume, "reading_direction"));

    char* volume_path_new       = new char[strlen(volume_path) + 1];
    char* reading_direction_new = new char[strlen(reading_direction) + 1];
    strcpy(volume_path_new, volume_path);
    strcpy(reading_direction_new, reading_direction);

    Add_manga_volume(manga_title, volume_index, reading_direction_new, volume_path_new);

    delete[] volume_path_new;
    delete[] reading_direction_new;
}

void
Bookshelf::Del_manga_volume(const char* manga_title, int volume_index)
{
    printf("Deleting volume %d in manga: %s\n", volume_index, manga_title);

    cJSON* manga = find_manga(manga_title); // 查找漫画
    if(!manga) return;                      // 如果漫画不存在

    // 遍历 volumes 数组
    cJSON* volumes = cJSON_GetObjectItem(manga, "volumes");
    for(int i = 0; i < cJSON_GetArraySize(volumes); i++)
    {
        // 获取当前卷的对象
        cJSON* volume          = cJSON_GetArrayItem(volumes, i);
        cJSON* volume_num_item = cJSON_GetObjectItem(volume, "volume");

        // 如果 "volume" 字段不是数字
        if(!cJSON_IsNumber(volume_num_item))
        {
            printf("Volume number not found.\n");
            return;
        }

        int volume_num = volume_num_item->valueint;

        // 如果卷号相同
        if(volume_num == volume_index)
        {
            cJSON_DeleteItemFromArray(volumes, i);
            printf("Volume %d deleted.\n", volume_index);
            return;
        }
        else if(volume_num > volume_index)
        {
            printf("Volume %d not found.\n", volume_index);
            return;
        }
    }
}

cJSON*
Bookshelf::find_manga(const char* manga_title) const
{
    printf("Finding manga: %s\n", manga_title);

    // 遍历mangas数组
    bool   manga_not_found = true;
    cJSON* manga           = nullptr;
    cJSON_ArrayForEach(manga, mangas)
    {
        // 获取 "title" 的值
        const char* title = cJSON_GetObjectItem(manga, "title")->valuestring;

        // 如果标题相同
        if(strcmp(title, title) == 0)
        {
            manga_not_found = false;
            break;
        }
    }

    if(manga_not_found) // 如果漫画不存在
    {
        printf("Manga not found: %s\n", manga_title);
        return nullptr;
    }
    else
    {
        printf("Manga found: %s\n", manga_title);
        return manga;
    }
}

cJSON*
Bookshelf::find_volume(cJSON* manga, int target_volume_index) const
{
    printf("Finding volume: %d in manga: %s\n", target_volume_index, cJSON_GetObjectItem(manga, "title")->valuestring);

    // 遍历volumes数组
    bool   volume_not_found = true;
    cJSON* volumes          = cJSON_GetObjectItem(manga, "volumes");
    cJSON* volume           = nullptr;
    cJSON_ArrayForEach(volume, volumes)
    {
        // 获取 "volume" 的值
        int volume_num = cJSON_GetObjectItem(volume, "volume")->valueint;

        // 如果卷号相同
        if(volume_num == target_volume_index)
        {
            volume_not_found = false;
            break;
        }
    }

    // 如果卷号不存在
    if(volume_not_found)
    {
        printf("Volume not found: %d\n", target_volume_index);
        return nullptr;
    }
    else
    {
        printf("Volume found: %d\n", target_volume_index);
        return volume;
    }
}

const char*
Bookshelf::find_page(cJSON* volume, int target_page_index) const
{
    printf("Finding page: %d in volume: %d\n", target_page_index, cJSON_GetObjectItem(volume, "volume")->valueint);

    // 获取pages数组
    cJSON* pages = cJSON_GetObjectItem(volume, "pages_paths");
    if(target_page_index >= cJSON_GetArraySize(pages) || target_page_index < 0)
    {
        // 如果target_page_index大于页数，或者小于0
        printf("Page not found: %d\n", target_page_index);
        return nullptr;
    }
    else // 如果target_page_index在页数范围内
    {
        printf("Page found: %d\n", target_page_index);
        return cJSON_GetArrayItem(pages, target_page_index)->valuestring;
    }
}
