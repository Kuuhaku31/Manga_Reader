
// config.cpp

#include "config.h"

#include "bookshelf.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>


static ImGui_setup& imgui     = ImGui_setup::Instance();
static Bookshelf&   bookshelf = Bookshelf::Instance();

Config* Config::instance = nullptr;

Config&
Config::Instance()
{
    if(instance == nullptr) instance = new Config();
    return *instance;
}

void
Config::Init()
{
    // 加载配置文件
    LoadConfig();
    bookshelf.LoadMangas();

    cJSON* window_config = cJSON_GetObjectItem(json_root, "window_config");

    // 设置窗口标题
    std::string window_title = cJSON_GetObjectItem(window_config, "window_title")->valuestring;

    // 设置窗口位置和大小
    Rect window_rect = {
        cJSON_GetObjectItem(window_config, "window_x")->valueint,
        cJSON_GetObjectItem(window_config, "window_y")->valueint,
        cJSON_GetObjectItem(window_config, "window_width")->valueint,
        cJSON_GetObjectItem(window_config, "window_height")->valueint
    };

    // 设置清空颜色
    clear_color.r = cJSON_GetObjectItem(window_config, "clear_color_r")->valueint;
    clear_color.g = cJSON_GetObjectItem(window_config, "clear_color_g")->valueint;
    clear_color.b = cJSON_GetObjectItem(window_config, "clear_color_b")->valueint;
    clear_color.a = cJSON_GetObjectItem(window_config, "clear_color_a")->valueint;

    // 设置窗口是否全屏
    is_fullscreen = cJSON_IsTrue(cJSON_GetObjectItem(window_config, "window_fullscreen"));

    // 初始化窗口
    imgui.Init(window_title, window_rect, is_fullscreen);

    // 加载字体
    // 默认字体
    font_default = font_default = ImGui::GetIO().Fonts->AddFontDefault();
    // 加载支持中文的字体文件
    font_SmileySans_Oblique = ImGui::GetIO().Fonts->AddFontFromFileTTF("SmileySans-Oblique.ttf", 18.0f, nullptr, ImGui::GetIO().Fonts->GetGlyphRangesChineseFull());

    // 加载漫画配置
    cJSON* manga_config = cJSON_GetObjectItem(json_root, "manga_config");
    // 漫画标题、卷、页
    manga_title      = cJSON_GetObjectItem(manga_config, "manga_title")->valuestring;
    manga_volume_idx = cJSON_GetObjectItem(manga_config, "manga_volume_idx")->valueint;
    manga_page_idx   = cJSON_GetObjectItem(manga_config, "manga_page_idx")->valueint;
    // 漫画位置
    cJSON* page_pos = cJSON_GetObjectItem(manga_config, "manga_page_pos");
    manga_page_pos  = ImVec2(cJSON_GetObjectItem(page_pos, "x")->valuedouble, cJSON_GetObjectItem(page_pos, "y")->valuedouble);
    // 漫画缩放
    manga_page_zoom = cJSON_GetObjectItem(manga_config, "manga_page_zoom")->valuedouble;

    printf("Config init.\n");
}

void
Config::Quit()
{
    // 保存窗口配置
    cJSON* window_config = cJSON_GetObjectItem(json_root, "window_config");

    // 保存窗口标题
    std::string window_title = SDL_GetWindowTitle(imgui.window);
    cJSON_ReplaceItemInObject(window_config, "window_title", cJSON_CreateString(window_title.c_str()));

    // 保存窗口位置和大小
    Rect window_rect;
    // 如果当前是全屏状态
    if(SDL_GetWindowFlags(imgui.window) & SDL_WINDOW_FULLSCREEN_DESKTOP)
    {
        window_rect = imgui.window_rect_before_fullscreen;

        cJSON_ReplaceItemInObject(window_config, "window_fullscreen", cJSON_CreateTrue());
    }
    else // 如果当前是窗口状态
    {
        SDL_GetWindowPosition(imgui.window, &window_rect.x, &window_rect.y);
        SDL_GetWindowSize(imgui.window, &window_rect.w, &window_rect.h);

        cJSON_ReplaceItemInObject(window_config, "window_fullscreen", cJSON_CreateFalse());
    }
    cJSON_ReplaceItemInObject(window_config, "window_x", cJSON_CreateNumber(window_rect.x));
    cJSON_ReplaceItemInObject(window_config, "window_y", cJSON_CreateNumber(window_rect.y));
    cJSON_ReplaceItemInObject(window_config, "window_width", cJSON_CreateNumber(window_rect.w));
    cJSON_ReplaceItemInObject(window_config, "window_height", cJSON_CreateNumber(window_rect.h));

    // 保存颜色参数
    cJSON_ReplaceItemInObject(window_config, "clear_color_r", cJSON_CreateNumber(clear_color.r));
    cJSON_ReplaceItemInObject(window_config, "clear_color_g", cJSON_CreateNumber(clear_color.g));
    cJSON_ReplaceItemInObject(window_config, "clear_color_b", cJSON_CreateNumber(clear_color.b));
    cJSON_ReplaceItemInObject(window_config, "clear_color_a", cJSON_CreateNumber(clear_color.a));

    // 保存漫画配置
    cJSON* manga_config = cJSON_GetObjectItem(json_root, "manga_config");
    // 保存漫画标题、卷、页
    cJSON_ReplaceItemInObject(manga_config, "manga_title", cJSON_CreateString(manga_title.c_str()));
    cJSON_ReplaceItemInObject(manga_config, "manga_volume_idx", cJSON_CreateNumber(manga_volume_idx));
    cJSON_ReplaceItemInObject(manga_config, "manga_page_idx", cJSON_CreateNumber(manga_page_idx));
    // 保存漫画位置
    cJSON* page_pos = cJSON_GetObjectItem(manga_config, "manga_page_pos");
    cJSON_ReplaceItemInObject(page_pos, "x", cJSON_CreateNumber(manga_page_pos.x));
    cJSON_ReplaceItemInObject(page_pos, "y", cJSON_CreateNumber(manga_page_pos.y));
    // 保存漫画缩放
    cJSON_ReplaceItemInObject(manga_config, "manga_page_zoom", cJSON_CreateNumber(manga_page_zoom));

    // 退出窗口
    imgui.Quit();

    // 保存配置文件
    SaveConfig();
    bookshelf.SaveMangas();

    cJSON_Delete(json_root);
    json_root = nullptr;

    printf("Config quit.\n");
}

void
Config::LoadConfig()
{
    // 释放之前的配置
    cJSON_Delete(json_root);

    // 读取配置文件
    std::ifstream file("config.json");
    if(!file.good())
    {
        Init_config();
        return;
    }

    std::stringstream str_stream; // 字符串流
    str_stream << file.rdbuf();   // 读取文件内容
    file.close();

    json_root = cJSON_Parse(str_stream.str().c_str()); // 解析JSON

    if(!json_root) // 解析失败
    {
        Init_config();
        return;
    }

    printf("Config loaded.\n");
}

void
Config::PrintConfig()
{
    // 打印配置文件
    char* s = cJSON_Print(json_root);
    printf("%s\n", s);
    free(s);
}

void
Config::SaveConfig()
{
    // 保存配置文件
    char* s = cJSON_Print(json_root);
    FILE* f = fopen("config.json", "w");
    fwrite(s, strlen(s), 1, f);
    fclose(f);
    free(s);

    printf("Config saved.\n");
}

void
Config::Init_config()
{
    cJSON_Delete(json_root);
    json_root = cJSON_CreateObject();

    // 添加 window_config 对象
    cJSON* window_config = cJSON_CreateObject();
    cJSON_AddItemToObject(json_root, "window_config", window_config);

    // 添加基本配置
    cJSON_AddStringToObject(window_config, "window_title", "窗口标题");
    cJSON_AddNumberToObject(window_config, "window_x", 30);
    cJSON_AddNumberToObject(window_config, "window_y", 40);
    cJSON_AddNumberToObject(window_config, "window_width", 800);
    cJSON_AddNumberToObject(window_config, "window_height", 600);
    cJSON_AddNumberToObject(window_config, "clear_color_r", 0xcc);
    cJSON_AddNumberToObject(window_config, "clear_color_g", 0xcc);
    cJSON_AddNumberToObject(window_config, "clear_color_b", 0xcc);
    cJSON_AddNumberToObject(window_config, "clear_color_a", 0xff);
    cJSON_AddFalseToObject(window_config, "window_fullscreen");

    // 添加 manga_config 对象
    cJSON* manga_config = cJSON_CreateObject();
    cJSON_AddItemToObject(json_root, "manga_config", manga_config);

    // 添加基本配置
    cJSON_AddStringToObject(manga_config, "manga_title", "漫画标题");
    cJSON_AddNumberToObject(manga_config, "manga_volume_idx", 0);
    cJSON_AddNumberToObject(manga_config, "manga_page_idx", 0);
    cJSON* page_pos = cJSON_CreateObject();
    cJSON_AddItemToObject(manga_config, "manga_page_pos", page_pos);
    cJSON_AddNumberToObject(page_pos, "x", 0);
    cJSON_AddNumberToObject(page_pos, "y", 0);
    cJSON_AddNumberToObject(manga_config, "manga_page_zoom", 1.0);

    printf("Config initialized.\n");
}

void
Config::Make_fullscreen(bool fullscreen)
{
    is_fullscreen = fullscreen;
    SDL_SetWindowFullscreen(imgui.window, is_fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
}
