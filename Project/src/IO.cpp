
#include "MangaReader.hpp"

IO::IO() {}
IO::~IO() {}

void
IO::Init_Graph()
{
    // 初始化图形窗口
    graph_HWND = initgraph(WINDOW_WIDE, WINDOW_HIGH);
    BeginBatchDraw(); // 开始批量绘图
    setbkcolor(KH_BK_COLOR);
    clearcliprgn();

    graph_HDC = GetImageHDC(NULL);                  // 获取主窗口的 DC
    SetStretchBltMode(graph_HDC, STRETCH_HALFTONE); // 设置拉伸贴图模式为抗锯齿

    // 全屏
    SetWindowLong(graph_HWND, GWL_STYLE, GetWindowLong(graph_HWND, GWL_STYLE) - WS_CAPTION);
    SetWindowPos(graph_HWND, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);
}

void
IO::Close_Graph()
{
    EndBatchDraw(); // 结束批量绘图
    closegraph();
}

void
IO::Print_img(IMAGE* output_img)
{
    // 旋转图像
    switch(revolve)
    {
    case -2:
    case +2:
        rotateimage(output_img, output_img, PI, KH_BK_COLOR, true, false);
        break;

    case +1:
    case -3:
        rotateimage(output_img, output_img, PI / 2, KH_BK_COLOR, true, false);
        break;

    case +3:
    case -1:
        rotateimage(output_img, output_img, -PI / 2, KH_BK_COLOR, true, false);
        break;

    default:
        revolve = 0;
        break;
    }

    int wide = output_img->getwidth() * size;
    int high = output_img->getheight() * size;

    StretchBlt(
        graph_HDC,
        (WINDOW_WIDE - wide) / 2,
        (WINDOW_HIGH - high) / 2,
        wide,
        high,
        GetImageHDC(output_img),
        0,
        0,
        output_img->getwidth(),
        output_img->getheight(),
        SRCCOPY);

    FlushBatchDraw();
}

void
IO::Size_Set(float s)
{
    size = s;
}

void
IO::Size_Set_d(float s)
{
    size += s;
}

void
IO::Revolve_Set(int r)
{
    revolve = r;
}

void
IO::Revolve_Set_d(int r)
{
    revolve += r;
}
