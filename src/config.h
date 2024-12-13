
// config.h

#pragma once

#include <cJSON.h>
#include <string>

// 单例模式
class Config
{
public:
    static Config& Instance();

public:
    void Init(); // 初始化
    void Quit(); // 退出

public:
    void Load();  // 加载配置文件
    void Print(); // 打印配置文件
    void Save();  // 保存配置文件

    void Init_config(); // 初始化配置文件

public:
    bool Get_manga_page(std::string* page_path, int manga_index, int volume_index, int page_index) const; // 获取漫画页

    void Load_manga_volume(int manga_index, int volume_index, std::string& volume_path); // 加载漫画卷
    void Print_manga(int index);                                                         // 打印漫画信息

public:
    bool  is_running = true;
    float page_zoom  = 1.0f;

private:
    cJSON* json_root = nullptr; // JSON根节点

private:
    Config()                         = default;
    ~Config()                        = default;
    Config(const Config&)            = delete;
    Config& operator=(const Config&) = delete;

    static Config* instance;
};
