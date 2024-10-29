
#include "MangaReader.hpp"


void
ImgPrinter::Print_img(IMAGE* output_img, HDC graph_HDC)
{
    // 旋转图像
#define PI 3.14159265358979323846264338
    switch(revolve)
    {
    case not_rotate:
        break;

    case rotate_90:
        rotateimage(output_img, output_img, PI / 2, KH_BK_COLOR, true, false);
        break;

    case rotate_180:
        rotateimage(output_img, output_img, PI, KH_BK_COLOR, true, false);
        break;

    case rotate_270:
        rotateimage(output_img, output_img, -PI / 2, KH_BK_COLOR, true, false);
        break;

    default:
        break;
    }
#undef PI

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
    //
}

void
ImgPrinter::Size_Set(float s)
{
    size = s;
}

void
ImgPrinter::Size_Set_d(float s)
{
    size += s;
}

void
ImgPrinter::Revolve_clockwise()
{
    if(revolve == rotate_270)
    {
        revolve = not_rotate;
    }
    else
    {
        revolve = (img_revolve_type)(revolve + 1);
    }
}

void
ImgPrinter::Revolve_counterclockwise()
{
    if(revolve == not_rotate)
    {
        revolve = rotate_270;
    }
    else
    {
        revolve = (img_revolve_type)(revolve - 1);
    }
}
