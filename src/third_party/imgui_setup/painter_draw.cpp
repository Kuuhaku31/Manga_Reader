
// painter_draw.cpp

#include "imgui_setup.h"

#include "base.h"

void
Painter::DrawLine(float A, float B, float C, const Color& color) const
{
    static float start_x = 0;
    static float start_y = 0;
    static float end_x   = 0;
    static float end_y   = 0;

    static float view_y1 = 0;
    static float view_y2 = 0;
    static float view_x1 = 0;
    static float view_x2 = 0;

    static float unit_size = 0;

    if(!painter_view) return;

    view_y1 = painter_view->Get_view_left_top_position().vy;
    view_y2 = painter_view->Get_view_right_bottom_position().vy;
    view_x1 = painter_view->Get_view_left_top_position().vx;
    view_x2 = painter_view->Get_view_right_bottom_position().vx;

    unit_size = painter_view->Get_unit_size();

    if(!A) // y = -C / B
    {
        start_y = -C / B;
        if(start_y < view_y1 || start_y > view_y2) return;

        start_x = view_x1;
        end_x   = view_x2;
        end_y   = start_y;
    }
    else if(!B) // x = -C / A
    {
        start_x = -C / A;
        if(start_x < view_x1 || start_x > view_x2) return;

        start_y = view_y1;
        end_y   = view_y2;
        end_x   = start_x;
    }
    else
    {
        start_x = view_x1;
        end_x   = view_x2;

        start_y = (-A * start_x - C) / B;
        if(start_y < view_y1)
        {
            start_y = view_y1;
            start_x = (-B * start_y - C) / A;
            if(start_x < view_x1 || start_x > view_x2) return;
        }
        else if(start_y > view_y2)
        {
            start_y = view_y2;
            start_x = (-B * start_y - C) / A;
            if(start_x < view_x1 || start_x > view_x2) return;
        }

        end_y = (-A * end_x - C) / B;
        if(end_y < view_y1)
        {
            end_y = view_y1;
            end_x = (-B * end_y - C) / A;
            if(end_x < view_x1 || end_x > view_x2) return;
        }
        else if(end_y > view_y2)
        {
            end_y = view_y2;
            end_x = (-B * end_y - C) / A;
            if(end_x < view_x1 || end_x > view_x2) return;
        }
    }

    lineRGBA(
        renderer,
        (start_x - view_x1) * unit_size,
        (start_y - view_y1) * unit_size,
        (end_x - view_x1) * unit_size,
        (end_y - view_y1) * unit_size,
        color.r,
        color.g,
        color.b,
        color.a

    );
}

void
Painter::DrawLine(float start_x, float start_y, float end_x, float end_y, const Color& color) const
{
    static float x1 = 0;
    static float y1 = 0;
    static float x2 = 0;
    static float y2 = 0;

    static float view_y1 = 0;
    static float view_y2 = 0;
    static float view_x1 = 0;
    static float view_x2 = 0;

    static float unit_size = 0;

    if(!painter_view) return;

    view_y1 = painter_view->Get_view_left_top_position().vy;
    view_y2 = painter_view->Get_view_right_bottom_position().vy;
    view_x1 = painter_view->Get_view_left_top_position().vx;
    view_x2 = painter_view->Get_view_right_bottom_position().vx;

    unit_size = painter_view->Get_unit_size();

    if(start_x > end_x) // 确保 start_x < end_x
    {
        swap<float>(start_x, end_x);
        swap<float>(start_y, end_y);
    }

    float A = end_y - start_y;
    float B = start_x - end_x;
    float C = -B * start_y - A * start_x;

    if(!A) // y = -C / B
    {
        y1 = -C / B;
        if(y1 < view_y1 || y1 > view_y2) return;
        if(end_x < view_x1 || start_x > view_x2) return;

        x1 = start_x < view_x1 ? view_x1 : start_x;
        x2 = end_x > view_x2 ? view_x2 : end_x;

        y2 = y1;
    }
    else if(!B) // x = -C / A
    {
        x1 = -C / A;
        if(x1 < view_x1 || x1 > view_x2) return;
        if(start_y > end_y) swap<float>(start_y, end_y);
        if(end_y < view_y1 || start_y > view_y2) return;

        y1 = start_y < view_y1 ? view_y1 : start_y;
        y2 = end_y > view_y2 ? view_y2 : end_y;

        x2 = x1;
    }
    else
    {
        if(end_x < view_x1 || start_x > view_x2) return;

        x1 = start_x < view_x1 ? view_x1 : start_x;
        x2 = end_x > view_x2 ? view_x2 : end_x;

        y1 = (-A * x1 - C) / B;
        if(y1 < view_y1)
        {
            y1 = view_y1;
            x1 = (-B * y1 - C) / A;
            if(x1 < view_x1 || x1 > view_x2) return;
        }
        else if(y1 > view_y2)
        {
            y1 = view_y2;
            x1 = (-B * y1 - C) / A;
            if(x1 < view_x1 || x1 > view_x2) return;
        }

        y2 = (-A * x2 - C) / B;
        if(y2 < view_y1)
        {
            y2 = view_y1;
            x2 = (-B * y2 - C) / A;
            if(x2 < view_x1 || x2 > view_x2) return;
        }
        else if(y2 > view_y2)
        {
            y2 = view_y2;
            x2 = (-B * y2 - C) / A;
            if(x2 < view_x1 || x2 > view_x2) return;
        }
    }

    lineRGBA(
        renderer,
        (x1 - view_x1) * unit_size,
        (y1 - view_y1) * unit_size,
        (x2 - view_x1) * unit_size,
        (y2 - view_y1) * unit_size,
        color.r,
        color.g,
        color.b,
        color.a

    );
}

void
Painter::DrawLine_(float start_x, float start_y, float end_x, float end_y, const Color& color) const
{
    if(!painter_view) return;

    float view_left_top_position_y     = painter_view->Get_view_left_top_position().vy;
    float view_right_bottom_position_y = painter_view->Get_view_right_bottom_position().vy;

    float view_left_top_position_x     = painter_view->Get_view_left_top_position().vx;
    float view_right_bottom_position_x = painter_view->Get_view_right_bottom_position().vx;

    float unit_size = painter_view->Get_unit_size();

    float dx = end_x - start_x;
    if(dx)
    {
        float k = (end_y - start_y) / dx;

        if(start_x < view_left_top_position_x)
        {
            start_y += k * (view_left_top_position_x - start_x);
            start_x = view_left_top_position_x;
        }
        else if(start_x > view_right_bottom_position_x)
        {
            start_y += k * (view_right_bottom_position_x - start_x);
            start_x = view_right_bottom_position_x;
        }

        if(end_x < view_left_top_position_x)
        {
            end_y += k * (view_left_top_position_x - end_x);
            end_x = view_left_top_position_x;
        }
        else if(end_x > view_right_bottom_position_x)
        {
            end_y += k * (view_right_bottom_position_x - end_x);
            end_x = view_right_bottom_position_x;
        }

        if(start_y < view_left_top_position_y)
        {
            start_x += (view_left_top_position_y - start_y) / k;
            start_y = view_left_top_position_y;
        }
        else if(start_y > view_right_bottom_position_y)
        {
            start_x += (view_right_bottom_position_y - start_y) / k;
            start_y = view_right_bottom_position_y;
        }

        if(start_x < view_left_top_position_x || start_x > view_right_bottom_position_x) return;

        if(end_y < view_left_top_position_y)
        {
            end_x += (view_left_top_position_y - end_y) / k;
            end_y = view_left_top_position_y;
        }
        else if(end_y > view_right_bottom_position_y)
        {
            end_x += (view_right_bottom_position_y - end_y) / k;
            end_y = view_right_bottom_position_y;
        }

        if(end_x < view_left_top_position_x || end_x > view_right_bottom_position_x) return;
    }
    else
    {
        if(start_x < view_left_top_position_x || start_x > view_right_bottom_position_x) return;

        if(start_y < view_left_top_position_y)
        {
            start_y = view_left_top_position_y;
        }
        else if(start_y > view_right_bottom_position_y)
        {
            start_y = view_right_bottom_position_y;
        }
    }

    lineRGBA(
        renderer,
        (start_x - view_left_top_position_x) * unit_size,
        (start_y - view_left_top_position_y) * unit_size,
        (end_x - view_left_top_position_x) * unit_size,
        (end_y - view_left_top_position_y) * unit_size,
        color.r,
        color.g,
        color.b,
        color.a

    );
}

void
Painter::DrawArc(float center_x, float center_y, float radius, float start_angle, float end_angle, const Color& color) const
{
    if(!painter_view) return;

    float view_left_top_position_x = painter_view->Get_view_left_top_position().vx;
    float view_left_top_position_y = painter_view->Get_view_left_top_position().vy;

    float unit_size = painter_view->Get_unit_size();

    arcRGBA(
        renderer,
        (center_x - view_left_top_position_x) * unit_size,
        (center_y - view_left_top_position_y) * unit_size,
        radius * unit_size,
        start_angle,
        end_angle,
        color.r,
        color.g,
        color.b,
        color.a

    );
}

inline void
DrawPartialCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int screenWidth, int screenHeight)
{
    int x   = radius;
    int y   = 0;
    int err = 0;

    while(x >= y)
    {
        // 绘制八个对称点，只绘制在画布范围内的点
        if(centerX + x >= 0 && centerX + x < screenWidth && centerY + y >= 0 && centerY + y < screenHeight)
            pixelRGBA(renderer, centerX + x, centerY + y, r, g, b, a);
        if(centerX + y >= 0 && centerX + y < screenWidth && centerY + x >= 0 && centerY + x < screenHeight)
            pixelRGBA(renderer, centerX + y, centerY + x, r, g, b, a);
        if(centerX - y >= 0 && centerX - y < screenWidth && centerY + x >= 0 && centerY + x < screenHeight)
            pixelRGBA(renderer, centerX - y, centerY + x, r, g, b, a);
        if(centerX - x >= 0 && centerX - x < screenWidth && centerY + y >= 0 && centerY + y < screenHeight)
            pixelRGBA(renderer, centerX - x, centerY + y, r, g, b, a);
        if(centerX - x >= 0 && centerX - x < screenWidth && centerY - y >= 0 && centerY - y < screenHeight)
            pixelRGBA(renderer, centerX - x, centerY - y, r, g, b, a);
        if(centerX - y >= 0 && centerX - y < screenWidth && centerY - x >= 0 && centerY - x < screenHeight)
            pixelRGBA(renderer, centerX - y, centerY - x, r, g, b, a);
        if(centerX + y >= 0 && centerX + y < screenWidth && centerY - x >= 0 && centerY - x < screenHeight)
            pixelRGBA(renderer, centerX + y, centerY - x, r, g, b, a);
        if(centerX + x >= 0 && centerX + x < screenWidth && centerY - y >= 0 && centerY - y < screenHeight)
            pixelRGBA(renderer, centerX + x, centerY - y, r, g, b, a);

        if(err <= 0) // 计算下一个点
        {
            y += 1;
            err += 2 * y + 1;
        }
        if(err > 0)
        {
            x -= 1;
            err -= 2 * x + 1;
        }
    }
}

void
Painter::DrawCircle(float center_x, float center_y, float radius, const Color& color, bool is_solid) const
{
    if(!painter_view) return;

    float view_left_top_position_x = painter_view->Get_view_left_top_position().vx;
    float view_left_top_position_y = painter_view->Get_view_left_top_position().vy;

    float unit_size = painter_view->Get_unit_size();


    if(is_solid)
    {
        float dx = center_x - painter_view->Get_view_center_position().vx;
        float dy = center_y - painter_view->Get_view_center_position().vy;

        float h_wide = painter_view->Get_view_size_half().vx;
        float h_high = painter_view->Get_view_size_half().vy;

        if(dx < 0) dx = -dx;
        if(dy < 0) dy = -dy;

        float u_x = dx - h_wide;
        float u_y = dy - h_high;

        if(u_x < 0) u_x = 0;
        if(u_y < 0) u_y = 0;

        if(u_x * u_x + u_y * u_y >= radius * radius)
        {
            DrawPartialCircle(
                renderer,
                (center_x - view_left_top_position_x) * unit_size,
                (center_y - view_left_top_position_y) * unit_size,
                radius * unit_size,
                color.r,
                color.g,
                color.b,
                color.a,
                painter_view->Get_view_size().vx * unit_size,
                painter_view->Get_view_size().vy * unit_size

            );
            return;
        }

        if(painter_view->Get_view_size().module() / radius > 0.07)
        {
            DrawPartialCircle(
                renderer,
                (center_x - view_left_top_position_x) * unit_size,
                (center_y - view_left_top_position_y) * unit_size,
                radius * unit_size,
                color.r,
                color.g,
                color.b,
                color.a,
                painter_view->Get_view_size().vx * unit_size,
                painter_view->Get_view_size().vy * unit_size

            );
        }
        else
        {
            Vector2 dir = painter_view->Get_view_center_position() - Vector2{ center_x, center_y };

            dir.to_unit();
            dir *= radius;

            DrawLine(
                dir.vx,
                dir.vy,
                -radius * radius - center_x * (dir.vx - center_x) - center_y * (dir.vy - center_y),
                color

            );
        }
    }
    else
    {
        filledCircleRGBA(
            renderer,
            (center_x - view_left_top_position_x) * unit_size,
            (center_y - view_left_top_position_y) * unit_size,
            radius * unit_size,
            color.r,
            color.g,
            color.b,
            color.a

        );
    }
}

void
Painter::DrawRect(float x, float y, float w, float h, const Color& color, bool is_solid) const
{
    if(!painter_view) return;

    float view_left_top_position_x = painter_view->Get_view_left_top_position().vx;
    float view_left_top_position_y = painter_view->Get_view_left_top_position().vy;

    float unit_size = painter_view->Get_unit_size();

    if(is_solid)
    {
        // 使用 SDL2_gfx 绘制矩形边框
        // rectangleRGBA(
        //     renderer,
        //     (x - view_left_top_position_x) * unit_size,
        //     (y - view_left_top_position_y) * unit_size,
        //     (x + w - view_left_top_position_x) * unit_size,
        //     (y + h - view_left_top_position_y) * unit_size,
        //     color >> 24 & 0xff,
        //     color >> 16 & 0xff,
        //     color >> 8 & 0xff,
        //     color & 0xff

        // );

        DrawLine(x, y, x + w, y, color);
        DrawLine(x + w, y, x + w, y + h, color);
        DrawLine(x + w, y + h, x, y + h, color);
        DrawLine(x, y + h, x, y, color);
    }
    else
    {
        // 使用 SDL2_gfx 绘制填充矩形
        boxRGBA(
            renderer,
            (x - view_left_top_position_x) * unit_size,
            (y - view_left_top_position_y) * unit_size,
            (x + w - view_left_top_position_x) * unit_size,
            (y + h - view_left_top_position_y) * unit_size,
            color.r,
            color.g,
            color.b,
            color.a

        );
    }
}

void
Painter::DrawTriangle(float a_x, float a_y, float b_x, float b_y, float c_x, float c_y, const Color& color, bool is_solid) const
{
    if(!painter_view) return;

    float view_left_top_position_x = painter_view->Get_view_left_top_position().vx;
    float view_left_top_position_y = painter_view->Get_view_left_top_position().vy;

    float unit_size = painter_view->Get_unit_size();

    if(is_solid)
    {
        aatrigonRGBA(
            renderer,
            (a_x - view_left_top_position_x) * unit_size,
            (a_y - view_left_top_position_y) * unit_size,
            (b_x - view_left_top_position_x) * unit_size,
            (b_y - view_left_top_position_y) * unit_size,
            (c_x - view_left_top_position_x) * unit_size,
            (c_y - view_left_top_position_y) * unit_size,
            color.r,
            color.g,
            color.b,
            color.a

        );
    }
    else
    {
        filledTrigonRGBA(
            renderer,
            (a_x - view_left_top_position_x) * unit_size,
            (a_y - view_left_top_position_y) * unit_size,
            (b_x - view_left_top_position_x) * unit_size,
            (b_y - view_left_top_position_y) * unit_size,
            (c_x - view_left_top_position_x) * unit_size,
            (c_y - view_left_top_position_y) * unit_size,
            color.r,
            color.g,
            color.b,
            color.a

        );
    }
}

// tex_size
void
Painter::DrawTexture(Texture* texture, const IRect& rect_src, const FRect& rect_dst, float angle) const
{
    if(!painter_view) return;

    float view_left_top_position_x = painter_view->Get_view_left_top_position().vx;
    float view_left_top_position_y = painter_view->Get_view_left_top_position().vy;

    float unit_size = painter_view->Get_unit_size();

    static SDL_Rect src;
    static SDL_Rect dst;

    src = { rect_src.x, rect_src.y, rect_src.w, rect_src.h };

    dst.x = (rect_dst.x - view_left_top_position_x) * unit_size;
    dst.y = (rect_dst.y - view_left_top_position_y) * unit_size;
    dst.w = rect_dst.w * unit_size;
    dst.h = rect_dst.h * unit_size;

    SDL_RenderCopyEx(
        renderer,
        texture,
        &src,
        &dst,
        angle,
        nullptr,
        SDL_FLIP_NONE

    );
}
