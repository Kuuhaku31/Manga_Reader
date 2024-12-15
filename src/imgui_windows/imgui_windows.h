
// imgui_windows.h

#pragma once

#include <SDL2/SDL.h>

enum class PageOutputFlag
{
    None,
    Center,
    LeftTop,
    NormalSize,
    CenterInWindow
};

// 显示图片
void
ImGui_Window_Book(SDL_Texture* texture, PageOutputFlag flag = PageOutputFlag::None);

// 显示配置窗口
void
ImGui_Window_config(bool& is_running, float& zoom);

// 显示漫画列表
void
ImGui_Window_Manga_list(bool* is_show = nullptr);
