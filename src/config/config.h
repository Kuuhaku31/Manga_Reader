
// config.h

#pragma once

#include "imgui_setup.h"

#include <cJSON.h>

enum class PageOutputFlag
{
    None,
    Center,
    LeftTop,
    NormalSize,
    CenterInWindow
};

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

    cJSON* Get_json_root() const { return json_root; }

public:
    ImFont* font_default            = nullptr;
    ImFont* font_SmileySans_Oblique = nullptr;

public:
    bool is_show_menu           = false; // 显示菜单
    bool is_show_config_window  = false;
    bool is_show_console_window = false;
    bool is_show_manga_list     = false;

    bool is_show_demo_window = false;

public:
    bool Is_fullscreen() const { return is_fullscreen; }
    void Make_fullscreen(bool fullscreen);

public:
    bool is_flashing = true;

public:
    std::string manga_title      = "ぼっち・ざ・ろっく！";
    int         manga_volume_idx = -1;
    int         manga_page_idx   = -1;
    ImVec2      manga_page_pos;         // 纹理位置
    float       manga_page_zoom = 1.0f; // 纹理缩放

    Color clear_color = { 0xcc, 0xcc, 0xcc, 0xff };

public:
    SDL_Texture*   tex_page        = nullptr;
    PageOutputFlag page_outpt_flag = PageOutputFlag::None;


private:
    bool is_fullscreen = false;

private:
    cJSON* json_root = nullptr; // JSON根节点

public:
    bool Is_running() const { return is_running; }
    void Stop_running() { is_running = false; }

private:
    bool is_running = true;

private:
    Config()                         = default;
    ~Config()                        = default;
    Config(const Config&)            = delete;
    Config& operator=(const Config&) = delete;

    static Config* instance;
};
