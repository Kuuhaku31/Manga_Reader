
// imgui_windows.h

#pragma once

#include <SDL2/SDL.h>

// 显示图片
void
ImGui_Window_Book(SDL_Texture* texture);

// 显示配置窗口
void
ImGui_Window_config(float& zoom);

// 显示漫画列表
void
ImGui_Window_Manga_list(bool* is_show = nullptr);

// 显示菜单
void
ImGui_Window_Menu(bool* is_show = nullptr);
