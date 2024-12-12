
// config.cpp

#include "config.h"

#include "imgui_setup.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>

static ImGui_setup& imgui = ImGui_setup::Instance();

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
    Load();

    cJSON* window_config = cJSON_GetObjectItem(json_root, "window_config");

    std::string window_title = cJSON_GetObjectItem(window_config, "window_title")->valuestring;

    Rect window_rect = {
        cJSON_GetObjectItem(window_config, "window_x")->valueint,
        cJSON_GetObjectItem(window_config, "window_y")->valueint,
        cJSON_GetObjectItem(window_config, "window_width")->valueint,
        cJSON_GetObjectItem(window_config, "window_height")->valueint
    };

    imgui.clear_color.r = cJSON_GetObjectItem(window_config, "clear_color_r")->valueint;
    imgui.clear_color.g = cJSON_GetObjectItem(window_config, "clear_color_g")->valueint;
    imgui.clear_color.b = cJSON_GetObjectItem(window_config, "clear_color_b")->valueint;
    imgui.clear_color.a = cJSON_GetObjectItem(window_config, "clear_color_a")->valueint;

    bool is_fullscreen = cJSON_IsTrue(cJSON_GetObjectItem(window_config, "window_fullscreen"));

    imgui.Init(window_title, window_rect, is_fullscreen);

    printf("Config init.\n");
}

void
Config::Quit()
{
    // 修改窗口配置
    cJSON* window_config = cJSON_GetObjectItem(json_root, "window_config");

    std::string window_title = SDL_GetWindowTitle(imgui.window);
    cJSON_ReplaceItemInObject(window_config, "window_title", cJSON_CreateString(window_title.c_str()));

    Rect window_rect;
    // 如果当前是全屏状态
    if(SDL_GetWindowFlags(imgui.window) & SDL_WINDOW_FULLSCREEN_DESKTOP)
    {
        window_rect = imgui.window_rect_before_fullscreen;

        cJSON_ReplaceItemInObject(window_config, "window_fullscreen", cJSON_CreateTrue());
    }
    else
    {
        SDL_GetWindowPosition(imgui.window, &window_rect.x, &window_rect.y);
        SDL_GetWindowSize(imgui.window, &window_rect.w, &window_rect.h);

        cJSON_ReplaceItemInObject(window_config, "window_fullscreen", cJSON_CreateFalse());
    }

    cJSON_ReplaceItemInObject(window_config, "window_x", cJSON_CreateNumber(window_rect.x));
    cJSON_ReplaceItemInObject(window_config, "window_y", cJSON_CreateNumber(window_rect.y));
    cJSON_ReplaceItemInObject(window_config, "window_width", cJSON_CreateNumber(window_rect.w));
    cJSON_ReplaceItemInObject(window_config, "window_height", cJSON_CreateNumber(window_rect.h));

    imgui.Quit();

    Save();

    cJSON_Delete(json_root);

    printf("Config quit.\n");
}

void
Config::Load()
{
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
Config::Print()
{
    // 打印配置文件
    char* s = cJSON_Print(json_root);
    printf("%s\n", s);
    free(s);
}

void
Config::Save()
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

    // 添加基本配置
    cJSON_AddStringToObject(json_root, "window_title", "窗口标题");
    cJSON_AddNumberToObject(json_root, "window_width", 800);
    cJSON_AddNumberToObject(json_root, "window_height", 600);
    cJSON_AddNumberToObject(json_root, "fps", 60);
    cJSON_AddNumberToObject(json_root, "volume", 50);
    cJSON_AddNumberToObject(json_root, "mute", 0);

    printf("Config initialized.\n");
}

bool
Config::Get_manga_page(std::string* page_path, int manga_index, int volume_index, int page_index) const
{
    cJSON* mangas  = cJSON_GetObjectItem(json_root, "mangas");
    cJSON* manga   = cJSON_GetArrayItem(mangas, manga_index);
    cJSON* volumes = cJSON_GetObjectItem(manga, "volumes");

    // 如果volumes不是数组
    if(!cJSON_IsArray(volumes))
    {
        printf("Volumes not found.\n");
        return false;
    }

    // 遍历volumes数组
    for(int i = 0; i < cJSON_GetArraySize(volumes); i++)
    {
        // 获取 "volume" 的值
        int volume = cJSON_GetObjectItem(cJSON_GetArrayItem(volumes, i), "volume")->valueint;

        // 如果卷号相同
        if(volume == volume_index)
        {
            cJSON* pages = cJSON_GetObjectItem(cJSON_GetArrayItem(volumes, i), "pages");

            // 如果pages不是数组
            if(!cJSON_IsArray(pages))
            {
                printf("Pages not found.\n");
                return false;
            }

            // 遍历pages数组
            for(int j = 0; j < cJSON_GetArraySize(pages); j++)
            {
                // 获取 "page" 的值
                int page = cJSON_GetObjectItem(cJSON_GetArrayItem(pages, j), "page")->valueint;

                // 如果页号相同
                if(page == page_index)
                {
                    *page_path = cJSON_GetObjectItem(cJSON_GetArrayItem(pages, j), "path")->valuestring;
                    return true;
                }
            }
        }
    }

    return false;
}

void
Config::Load_manga_volume(int manga_index, int volume_index, std::string& volume_path)
{
    cJSON* mangas  = cJSON_GetObjectItem(json_root, "mangas");
    cJSON* manga   = cJSON_GetArrayItem(mangas, manga_index);
    cJSON* volumes = cJSON_GetObjectItem(manga, "volumes");

    // 如果volumes不是数组
    if(!cJSON_IsArray(volumes))
    {
        printf("Volumes not found.\n");
        return;
    }

    // 遍历volumes数组
    for(int i = 0; i < cJSON_GetArraySize(volumes); i++)
    {
        // 获取 "volume" 的值
        int volume = cJSON_GetObjectItem(cJSON_GetArrayItem(volumes, i), "volume")->valueint;

        // 如果卷号相同
        if(volume == volume_index)
        {
            cJSON_DeleteItemFromArray(volumes, i);
        }
    }

    // 创建新卷
    cJSON* new_volume = cJSON_CreateObject();
    cJSON_AddItemToArray(volumes, new_volume);

    // 添加卷号
    cJSON_AddNumberToObject(new_volume, "volume", volume_index);
    // 添加描述
    std::string description = "第" + std::to_string(volume_index) + "卷";
    cJSON_AddStringToObject(new_volume, "description", description.c_str());
    // 添加阅读方向
    cJSON_AddStringToObject(new_volume, "reading_direction", "right-to-left");
    // 创建页数组
    cJSON* pages = cJSON_CreateArray();
    cJSON_AddItemToObject(new_volume, "pages", pages);


    // 加载卷
    namespace fs = std::filesystem;
    if(!fs::exists(volume_path)) // 如果路径不存在
    {
        printf("Volume path not exists: %s\n", volume_path.c_str());
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
            if(extension == ".jpg" || extension == ".png" || extension == ".jpeg" || extension == ".bmp" || extension == ".gif")
            {
                // 将图片路径存储到 volume_pages 容器中
                volume_pages_paths.push_back(file_path);
            }
        }
    }

    // 如果 volume_pages 为空，表示加载失败
    if(volume_pages_paths.empty())
    {
        printf("Volume pages empty: %s\n", volume_path.c_str());
        return;
    }

    // 重小到大排序
    std::sort(volume_pages_paths.begin(), volume_pages_paths.end());

    // 添加到 pages 数组
    for(int i = 0; i < volume_pages_paths.size(); i++)
    {
        cJSON* page = cJSON_CreateObject();
        cJSON_AddItemToArray(pages, page);

        cJSON_AddNumberToObject(page, "page", i + 1);
        cJSON_AddStringToObject(page, "path", volume_pages_paths[i].c_str());
    }

    // 添加页数
    cJSON_AddNumberToObject(new_volume, "page_count", volume_pages_paths.size());

    // 将第一页设置为封面
    cJSON* cover = cJSON_GetArrayItem(pages, 0);
    cJSON_AddStringToObject(new_volume, "cover", cJSON_GetObjectItem(cover, "path")->valuestring);

    printf("Volume %d loaded: %d pages\n", volume_index, volume_pages_paths.size());
}

void
Config::Print_manga(int index)
{
    cJSON* mangas = cJSON_GetObjectItem(json_root, "mangas");
    cJSON* manga  = cJSON_GetArrayItem(mangas, index);

    if(!manga)
    {
        printf("Manga not found.\n");
    }
    else
    {
        char* s = cJSON_Print(manga);
        printf("%s\n", s);
        free(s);
    }
}
