
// bookshelf.h

#pragma once

#include <cJSON.h>
#include <stdint.h>
#include <string>
#include <vector>

class MangaVolume
{
private:
    std::vector<std::string> pages_paths;
};

class MangaBook
{
};

// 单例模式
class Bookshelf
{
public:
    static void LoadMangas(const char* json_file); // 加载漫画
    static void SaveMangas(const char* json_file); // 保存漫画

public:
    static const char* Get_manga_page(const char* manga_title, uint32_t target_volume_index, uint32_t target_page_index); // 获取漫画页
    static const char* Get_reading_direction(const char* manga_title, uint32_t target_volume_index);                      // 获取阅读方向
    static uint32_t    Get_volume_page_count(const char* manga_title, uint32_t target_volume_index);                      // 获取卷页数

    static const cJSON* Get_json_root() { return mangas; }

public:
    static void Add_manga(const char* manga_title);
    static void Del_manga(const char* manga_title);                                                                                       // 添加漫画
    static void Add_manga_volume(const char* manga_title, uint32_t volume_index, const char* reading_direction, const char* volume_path); // 加载漫画卷                                                                                             // 打印漫画信息
    static void Del_manga_volume(const char* manga_title, uint32_t volume_index);                                                         // 删除漫画卷

private:
    static int32_t find_manga_idx(const char* manga_title);                           // 查找漫画索引
    static int32_t find_volume_idx(const cJSON* manga, uint32_t target_volume_index); // 查找卷索引

    static const cJSON* find_manga(const char* manga_title);                           // 查找漫画
    static const cJSON* find_volume(const cJSON* manga, uint32_t target_volume_index); // 查找卷
    static const char*  find_page(const cJSON* volume, uint32_t target_page_index);    // 查找页

public:
    static void load_volume_pages_paths(cJSON* pages_paths, const char* volume_path); // 加载卷页路径

private:
    static cJSON* mangas;

private:
    Bookshelf()                            = default;
    ~Bookshelf()                           = default;
    Bookshelf(const Bookshelf&)            = delete;
    Bookshelf& operator=(const Bookshelf&) = delete;
};
