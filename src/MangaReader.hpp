
#pragma once

#include <ShlObj.h>
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include <graphics.h>

#define KH_BK_COLOR 0x333333

#define WINDOW_WIDE GetSystemMetrics(SM_CXSCREEN)
#define WINDOW_HIGH GetSystemMetrics(SM_CYSCREEN)

// 卷
class Volume
{
public:
    // 加载路径下的所有图片路径，存储到 volume_pages
    // 加载成功返回 true
    bool Load(std::string, bool = true);


    int         Get_Index(); // 获取当前书签
    std::string Get_Page();  // 获取当前页路径
    std::string Get_Path();  // 获取卷路径

    // 当前卷翻页
    void Page_Turn_Left();
    void Page_Turn_Right();

    // 移动书签
    void Page_Index_Move_Left();
    void Page_Index_Move_Right();

    // 返回漫画图像
    // 获取成功返回 true
    bool Get_Image(IMAGE*);

    // 重载运算符
    // 根据 volume_path 比较
    bool operator==(const Volume&);
    bool operator!=(const Volume&);
    bool operator<(const Volume&);
    bool operator>(const Volume&);

    // 是否合并两页
    bool is_combine = false;

private:
    std::string              volume_path;
    std::vector<std::string> volume_pages;
    bool                     is_read_right_to_left = true;

    // 当前页
    int page_index = 0;
};

// 书
class MangaBook
{
public:
    // 添加卷
    void Add_Volume(Volume);
    // 删除卷，-1表示删除当前卷
    void Del_Volume(int = -1);

    // 移动卷
    void Volume_Next();
    void Volume_Last();
    void Volume_Set(int);

    // 获取当前卷
    bool                 Get_Volume(Volume**);
    std::vector<Volume>* Get_Volumes();

    // 返回当前卷和书页
    std::string Get_Volume_Page_Index();

private:
    // 储存卷
    std::vector<Volume> volumes;
    int                 volume_index = 0;
};

// 输入输出
class ImgPrinter
{
public:
    // 输出图像
    void Print_img(IMAGE*, HDC);

    void Size_Set(float);   // 设置图像输出大小
    void Size_Set_d(float); // 调整图像输出大小

    // 设置旋转
    void Revolve_clockwise();
    void Revolve_counterclockwise();

private:
    float size = 1.0f;

    enum img_revolve_type
    {
        not_rotate = 0,
        rotate_90  = 1,
        rotate_180 = 2,
        rotate_270 = 3
    };
    img_revolve_type revolve = not_rotate;
};

class Reader
{
public:
    // 调用Reader类的READ函数，即可开始阅读漫画
    void READ();

private:
    MangaBook   book;
    ImgPrinter  imgPrinter;
    IMAGE       output_img;                   // 输出图像
    std::string root_path;                    // 路径
    HWND        graph_HWND;                   // 图像窗口句柄
    HDC         graph_HDC;                    // 图像窗口HDC
    ExMessage   msg;                          // 消息
    bool        is_read_right_to_left = true; // 是否为日式排版
    bool        is_print_index        = true; // 是否打印页码
    bool        is_update_graph       = true; // 是否更新图像
    bool        is_continue           = true; // 是否继续循环

    void initBook();             // 初始化书
    void processInputs(); // 处理输入
    void initGraph();            // 初始化图像窗口
    void updateGraph();          // 更新图像
    void update();               // 主更新函数false则退出循环
    void exitGraph();            // 关闭图像窗口

    // 指令
    void magnify();    // 放大
    void shrink();     // 缩小
    void turnLeft();   // 向左翻页
    void turnRight();  // 向右翻页
    void movLeft();    // 向左一页
    void movRight();   // 向右一页
    void nextVolume(); // 下一卷
    void lastVolume(); // 上一卷
    void rotate_cw();  // 顺时针旋转
    void rotate_ccw(); // 逆时针旋转
    void combine();    // 合并两页
    void showIndex();  // 显示页码
    void exit();       // 退出
};
