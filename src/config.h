
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
    void Load();  // 加载配置文件
    void Print(); // 打印配置文件
    void Save();  // 保存配置文件

    void Init_config(); // 初始化配置文件

public:
    void Load_manga_volume(int manga_index, int volume_index, std::string& volume_path); // 加载漫画卷
    void Print_manga(int index);                                                         // 打印漫画信息

private:
    cJSON* json_root = nullptr; // JSON根节点

private:
    Config();
    ~Config();
    Config(const Config&)            = delete;
    Config& operator=(const Config&) = delete;

    static Config* instance;
};
