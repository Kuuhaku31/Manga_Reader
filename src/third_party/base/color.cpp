
// color.cpp

#include "base.h"

void
color_to_float4(const Color& color, float* f4)
{
    if(!f4) return;
    f4[0] = color.r / 255.0f;
    f4[1] = color.g / 255.0f;
    f4[2] = color.b / 255.0f;
    f4[3] = color.a / 255.0f;
}

void
float4_to_color(const float* f4, Color& color)
{
    if(!f4) return;
    color.r = f4[0] * 255;
    color.g = f4[1] * 255;
    color.b = f4[2] * 255;
    color.a = f4[3] * 255;
}
