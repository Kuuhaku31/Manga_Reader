
// bookshelf.h

#pragma once

#include <cJSON.h>

// 单例模式
class Bookshelf
{
public:
    static Bookshelf& Instance();

public:
    void LoadMangas(); // 加载漫画
    void SaveMangas(); // 保存漫画

public:
    const char* Get_manga_page(const char* manga_title, int target_volume_index, int target_page_index) const; // 获取漫画页
    const char* Get_reading_direction(const char* manga_title, int target_volume_index) const;                 // 获取阅读方向
    int         Get_volume_page_count(const char* manga_title, int target_volume_index) const;                 // 获取卷页数

    cJSON* Get_json_root() const { return mangas; }

public:
    void Add_manga(const char* manga_title);
    void ReLoad_manga(const char* manga_title);
    void Del_manga(const char* manga_title);                                                                                  // 添加漫画
    void Add_manga_volume(const char* manga_title, int volume_index, const char* reading_direction, const char* volume_path); // 加载漫画卷                                                                                             // 打印漫画信息
    void ReLoad_manga_volume(const char* manga_title, int volume_index);                                                      // 重新加载漫画卷
    void Del_manga_volume(const char* manga_title, int volume_index);                                                         // 删除漫画卷

private:
    int find_manga_idx(const char* manga_title) const;                // 查找漫画索引
    int find_volume_idx(cJSON* manga, int target_volume_index) const; // 查找卷索引

    cJSON*      find_manga(const char* manga_title) const;                // 查找漫画
    cJSON*      find_volume(cJSON* manga, int target_volume_index) const; // 查找卷
    const char* find_page(cJSON* volume, int target_page_index) const;    // 查找页

private:
    cJSON* mangas = nullptr;

private:
    Bookshelf()                            = default;
    ~Bookshelf()                           = default;
    Bookshelf(const Bookshelf&)            = delete;
    Bookshelf& operator=(const Bookshelf&) = delete;

    static Bookshelf* instance;
};
