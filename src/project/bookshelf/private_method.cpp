
// private_method.cpp

#include "bookshelf.h"

#include <algorithm>
#include <cstring>
#include <filesystem>
#include <stdio.h>
#include <vector>

int32_t
Bookshelf::find_manga_idx(const char* manga_title)
{
    // 遍历mangas数组
    for(uint32_t i = 0; i < cJSON_GetArraySize(mangas); i++)
    {
        // 获取当前漫画的对象
        cJSON* title = cJSON_GetObjectItem(cJSON_GetArrayItem(mangas, i), "title");
        if(!cJSON_IsString(title)) continue;                       // 如果标题不是字符串
        if(strcmp(title->valuestring, manga_title) == 0) return i; // 如果标题相同
    }
    return -1;
}

int32_t
Bookshelf::find_volume_idx(const cJSON* manga, uint32_t target_volume_index)
{
    // 遍历volumes数组
    cJSON* volumes = cJSON_GetObjectItem(manga, "volumes");
    for(uint32_t i = 0; i < cJSON_GetArraySize(volumes); i++)
    {
        // 获取当前卷的对象
        cJSON* volume_num_item = cJSON_GetObjectItem(cJSON_GetArrayItem(volumes, i), "volume");
        if(!cJSON_IsNumber(volume_num_item)) continue;                 // 如果 "volume" 字段不是数字
        if(volume_num_item->valueint == target_volume_index) return i; // 如果卷号相同
    }
    return -1;
}

const cJSON*
Bookshelf::find_manga(const char* manga_title)
{
    int32_t idx = find_manga_idx(manga_title);
    return idx >= 0 ? cJSON_GetArrayItem(mangas, idx) : nullptr;
}

const cJSON*
Bookshelf::find_volume(const cJSON* manga, uint32_t target_volume_index)
{
    if(!cJSON_IsObject(manga)) return nullptr;

    cJSON* volumes = cJSON_GetObjectItem(manga, "volumes");
    if(!cJSON_IsArray(volumes)) return nullptr;

    int32_t idx = find_volume_idx(manga, target_volume_index);
    return idx >= 0 ? cJSON_GetArrayItem(volumes, idx) : nullptr;
}

const char*
Bookshelf::find_page(const cJSON* volume, uint32_t target_page_index)
{
    if(!cJSON_IsObject(volume)) return nullptr;

    cJSON* pages = cJSON_GetObjectItem(volume, "pages_paths");
    if(!cJSON_IsArray(pages)) return nullptr;

    return target_page_index >= cJSON_GetArraySize(pages) ? nullptr : cJSON_GetArrayItem(pages, target_page_index)->valuestring;
}

void
Bookshelf::load_volume_pages_paths(cJSON* pages_paths, const char* volume_path)
{
    if(!volume_path)
    {
        printf("Volume path is nullptr.\n");
        return;
    }

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
            if(extension == ".avif" || extension == ".jpg" || extension == ".png" || extension == ".jpeg" || extension == ".bmp" || extension == ".gif")
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
