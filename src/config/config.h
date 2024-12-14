
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
    bool is_show_config_window  = false;
    bool is_show_demo_window    = false;
    bool is_show_console_window = false;

public:
    bool is_running = true;

    std::string manga_title      = "ぼっち・ざ・ろっく！";
    int         manga_volume_idx = 0;
    int         manga_page_idx   = 0;
    ImVec2      manga_page_pos;         // 纹理位置
    float       manga_page_zoom = 1.0f; // 纹理缩放

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
