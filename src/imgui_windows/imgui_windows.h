
// imgui_windows.h

#pragma once

void // 显示UI
ImGuiWin_UI(bool* is_show = nullptr);

void // 显示图片
ImGuiWin_Book(bool* is_show = nullptr);

void // 显示顶部菜单
ImGuiWin_TopMenu(bool* is_show = nullptr);

void // 选择当前漫画
ImGuiWin_SelectManga(bool* is_show = nullptr);

void // 显示漫画列表
ImGuiWin_MangaList(bool* is_show = nullptr);

void // 显示配置窗口
ImGuiWin_Config(bool* is_show = nullptr);

void // 加载资源窗口
ImGuiWin_LoadResources(bool* is_show = nullptr);

void // 显示控制台
ImGuiWin_Console(bool* is_show = nullptr);
