
// config.h

#pragma once

#include "imgui_setup.h"

#include <cJSON.h>

// 单例模式
class Config
{
public:
    static Config& Instance();

public:
    void Init(); // 初始化
    void Quit(); // 退出

public:
    void LoadConfig();  // 加载配置文件
    void PrintConfig(); // 打印配置文件
    void SaveConfig();  // 保存配置文件

    void Init_config(); // 初始化配置文件

public:
    bool  is_running = true;
    float page_zoom  = 1.0f;

    Color clear_color = { 0xcc, 0xcc, 0xcc, 0xff };

private:
    bool is_fullscreen = false;

private:
    cJSON* json_root = nullptr; // JSON根节点

private:
    Config()                         = default;
    ~Config()                        = default;
    Config(const Config&)            = delete;
    Config& operator=(const Config&) = delete;

    static Config* instance;
};
