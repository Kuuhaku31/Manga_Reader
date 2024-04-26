
#pragma once

#include <graphics.h>

#include "DATA.h"

class Camera
{   public:
    Camera()
    {
        null_img.Resize(500, 500);
    }
    ~Camera() {}

    void
    Print_page_R(IMAGE* img)
    {
        if(img == nullptr) { img = &null_img; }

        int wide = img->getwidth() * size;
        int high = img->getheight() * size;

        StretchBlt
        (
            GetImageHDC()
            , WINDOW_WIDE / 2 - wide
            , (WINDOW_HIGH - high) / 2
            , wide
            , high
            , GetImageHDC(img)
            , 0
            , 0
            , img->getwidth()
            , img->getheight()
            , SRCCOPY
        );
    }

    void
    Print_page_L(IMAGE* img)
    {
        if(img == nullptr) { img = &null_img; }

        int wide = img->getwidth() * size;
        int high = img->getheight() * size;

        StretchBlt
        (
            GetImageHDC()
            , WINDOW_WIDE / 2
            , (WINDOW_HIGH - high) / 2
            , wide
            , high
            , GetImageHDC(img)
            , 0
            , 0
            , img->getwidth()
            , img->getheight()
            , SRCCOPY
        );
    }

    void
    Print_page_M(IMAGE* img_)
    {
        if(img_ == nullptr) { img_ = &null_img; }

        IMAGE img = *img_;
        switch(revolve)
        {
        case -2:
        case +2:
            rotateimage(&img, img_, PI, KH_BK_COLOR, true, false);
            break;

        case +1:
        case -3:
            rotateimage(&img, img_, PI / 2, KH_BK_COLOR, true, false);
            break;

        case +3:
        case -1:
            rotateimage(&img, img_, -PI / 2, KH_BK_COLOR, true, false);
            break;

        default:
            revolve = 0;
            break;
        }

        int wide = img.getwidth() * size;
        int high = img.getheight() * size;

        StretchBlt
        (
            GetImageHDC()
            , (WINDOW_WIDE - wide) / 2
            , (WINDOW_HIGH - high) / 2
            , wide
            , high
            , GetImageHDC(&img)
            , 0
            , 0
            , img.getwidth()
            , img.getheight()
            , SRCCOPY
        );
    }

    void Set_size(float s) { size = s; }
    void Size_pp() { size += 0.1; }
    void Size_mm() { size -= 0.1; }
    void Set_revolve(int r) { revolve = r; }
    void Revolve_pp() { revolve++; }
    void Revolve_mm() { revolve--; }

private:

    float size = 1.0f;
    int revolve = 0;

    IMAGE null_img;
};
