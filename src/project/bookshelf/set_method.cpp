
// set_method.cpp

#include "bookshelf.h"

#include <stdio.h>

void
Bookshelf::Add_manga(const char* manga_title)
{
    // 检查漫画是否已存在
    if(find_manga(manga_title)) return;

    cJSON* new_manga = cJSON_CreateObject();
    cJSON_AddItemToArray(mangas, new_manga);                          // 创建新漫画
    cJSON_AddStringToObject(new_manga, "title", manga_title);         // 添加标题
    cJSON_AddItemToObject(new_manga, "volumes", cJSON_CreateArray()); // 添加卷数组
}

void
Bookshelf::Del_manga(const char* manga_title)
{
    int32_t idx = find_manga_idx(manga_title); // 查找漫画索引
    if(idx < 0) return;

    cJSON_DeleteItemFromArray(mangas, idx); // 删除漫画
}

void
Bookshelf::Add_manga_volume(const char* manga_title, uint32_t volume_index, const char* reading_direction, const char* volume_path)
{
    // 获取漫画卷列表
    cJSON* volumes = cJSON_GetObjectItem(find_manga(manga_title), "volumes");
    if(!cJSON_IsArray(volumes)) return;

    // 插入新卷
    cJSON* new_volume = cJSON_CreateObject();
    {
        bool is_inserted = false;
        for(uint32_t i = 0; i < cJSON_GetArraySize(volumes); i++) // 遍历 volumes 数组
        {
            // 获取当前卷的索引
            cJSON* volume_num_item = cJSON_GetObjectItem(cJSON_GetArrayItem(volumes, i), "volume");
            if(!cJSON_IsNumber(volume_num_item)) continue;

            if(volume_num_item->valueint == volume_index) // 如果卷号相同，更新卷
            {
                cJSON_DeleteItemFromArray(volumes, i);           // 删除现有卷
                cJSON_InsertItemInArray(volumes, i, new_volume); // 插入新卷
                is_inserted = true;
                break;
            }
            else if(volume_num_item->valueint > volume_index) // 如果当前卷号大于目标卷号，则插入新卷
            {
                cJSON_InsertItemInArray(volumes, i, new_volume); // 插入新卷
                is_inserted = true;
                break;
            }
        }
        if(!is_inserted) // 如果没有插入新卷，直接添加到数组末尾
        {
            cJSON_AddItemToArray(volumes, new_volume);
        }
    }

    // 编辑新卷
    {
        cJSON_AddNumberToObject(new_volume, "volume", volume_index);     // 添加卷号
        cJSON_AddStringToObject(new_volume, "volume_path", volume_path); // 添加路径

        // 添加描述
        char description[256];
        sprintf(description, "第%d卷", volume_index + 1);
        cJSON_AddStringToObject(new_volume, "description", description);

        cJSON_AddStringToObject(new_volume, "reading_direction", reading_direction); // 添加阅读方向

        // 加载卷页路径
        cJSON* pages_paths = cJSON_CreateArray();
        load_volume_pages_paths(pages_paths, volume_path);
        cJSON_AddItemToObject(new_volume, "pages_paths", pages_paths);

        // 将第一页设置为封面
        cJSON* path_0 = cJSON_GetArrayItem(pages_paths, 0);
        cJSON_AddStringToObject(new_volume, "cover", cJSON_IsString(path_0) ? path_0->valuestring : "");
    }
}

void
Bookshelf::Del_manga_volume(const char* manga_title, uint32_t volume_index)
{
    cJSON* volumes = cJSON_GetObjectItem(find_manga(manga_title), "volumes");
    if(!cJSON_IsArray(volumes)) return;

    // 遍历 volumes 数组
    for(uint32_t i = 0; i < cJSON_GetArraySize(volumes); i++)
    {
        // 获取当前卷的对象
        cJSON* volume_num_item = cJSON_GetObjectItem(cJSON_GetArrayItem(volumes, i), "volume");
        if(!cJSON_IsNumber(volume_num_item)) continue; // 如果 "volume" 字段不是数字

        // 如果卷号相同
        if(volume_num_item->valueint == volume_index)
        {
            cJSON_DeleteItemFromArray(volumes, i);
            return;
        }
        else if(volume_num_item->valueint > volume_index)
        {
            return;
        }
    }
}
