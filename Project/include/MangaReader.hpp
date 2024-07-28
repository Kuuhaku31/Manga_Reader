
#pragma once

#include <iostream>
#include <filesystem>
#include <ShlObj.h>
#include <string>
#include <vector>

#include "Input.hpp"

#define PI 3.14159265358979323846264338

#define KH_BK_COLOR 0x333333

#define WINDOW_WIDE GetSystemMetrics(SM_CXSCREEN)
#define WINDOW_HIGH GetSystemMetrics(SM_CYSCREEN)

// 卷
struct Volume
{
    std::string                volume_path;
    std::vector< std::string > volume_pages;

    // 当前页
    int page_index = 0;

    // 加载路径下的所有图片路径
    // 存储到 volume_pages
    bool Load(std::string);

    // 获取页
    bool
    Page_Get(std::string*, int = -1);

    // 重载运算符
    bool
    operator==(const Volume&);
    bool
    operator!=(const Volume&);
    bool
    operator<(const Volume&);
    bool
    operator>(const Volume&);
};

// 书
class MangaBook
{
public:
    MangaBook();
    ~MangaBook();

    // 加载卷
    // 加载成功返回 true
    // 加载失败返回 false
    bool Load_Volume(std::string);

    void // 设置当前卷
    Volume_Next();
    void
    Volume_Last();
    void
    Volume_Set(int);

    // 当前卷翻页
    void
    Page_Turn_Left();
    void
    Page_Turn_Right();

    // 移动书签
    void
    Page_Index_Move(int);

    // 是否合并两页
    void
    Combine_Page();

    // 返回漫画图像
    bool
    Get_Image(IMAGE*);

    // 返回当前书页
    std::string
    Index_Get();

private:
    // 储存卷
    std::vector< Volume > volumes;
    int                   volume_index;

    bool is_combine;
};

// 输入输出
class IO : public Input
{
public:
    IO();
    ~IO();

    // 加载窗口
    void
    Init_Graph();
    // 关闭窗口
    void
    Close_Graph();

    // 输出图像
    void
    Print_img(IMAGE*);

    // 设置图像大小
    void
    Size_Set(float);
    void
    Size_Set_d(float);

    // 设置旋转
    void
    Revolve_Set(int);
    void
    Revolve_Set_d(int);

private:
    // 输出图像
    float size    = 1.0f;
    int   revolve = 0;

    // 窗口参数
    HWND graph_HWND;
    HDC  graph_HDC;
};

class Reader
{
public:
    Reader();
    ~Reader();

    void
    READ();

private:
    MangaBook book;
    IO        io;

    IMAGE output_img;

    // 主更新函数false则退出循环
    bool
    update();

    // 更新图像
    void
    updateGraph();
};
