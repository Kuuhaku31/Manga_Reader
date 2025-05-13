
// base.h

#pragma once

#include "util.h"

struct IRect
{
    int32_t x = 0;
    int32_t y = 0;
    int32_t w = 0;
    int32_t h = 0;
};

struct FRect
{
    float x = 0;
    float y = 0;
    float w = 0;
    float h = 0;
};

struct Point
{
    int32_t px = 0;
    int32_t py = 0;

    Point& operator=(const Point& p)
    {
        px = p.px;
        py = p.py;

        return *this;
    }
};
#define ORIGIN_POINT Point{ 0, 0 }

struct Color
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    Color&
    operator=(const Color& color)
    {
        r = color.r;
        g = color.g;
        b = color.b;
        a = color.a;

        return *this;
    }
};

void
color_to_float4(const Color& color, float* f4);
void
float4_to_color(const float* f4, Color& color);

#define COLOR_BLACK Color{ 0x00, 0x00, 0x00, 0xFF }       // 黑色
#define COLOR_WHITE Color{ 0xFF, 0xFF, 0xFF, 0xFF }       // 白色
#define COLOR_RED Color{ 0xFF, 0x00, 0x00, 0xFF }         // 红色
#define COLOR_GREEN Color{ 0x00, 0xFF, 0x00, 0xFF }       // 绿色
#define COLOR_BLUE Color{ 0x00, 0x00, 0xFF, 0xFF }        // 蓝色
#define COLOR_YELLOW Color{ 0xFF, 0xFF, 0x00, 0xFF }      // 黄色
#define COLOR_CYAN Color{ 0x00, 0xFF, 0xFF, 0xFF }        // 青色
#define COLOR_PURPLE Color{ 0xFF, 0x00, 0xFF, 0xFF }      // 紫色
#define COLOR_ORANGE Color{ 0xFF, 0xA5, 0x00, 0xFF }      // 橙色
#define COLOR_GRAY Color{ 0x80, 0x80, 0x80, 0xFF }        // 灰色
#define COLOR_DARK_GRAY Color{ 0x40, 0x40, 0x40, 0xFF }   // 深灰色
#define COLOR_LIGHT_GRAY Color{ 0xC0, 0xC0, 0xC0, 0xFF }  // 浅灰色
#define COLOR_TRANSPARENT Color{ 0x00, 0x00, 0x00, 0x00 } // 透明色


class Vector2
{
public:
    float vx = 0;
    float vy = 0;

public:
    Vector2() = default;
    Vector2(float x, float y);
    ~Vector2() = default;

public:
    Vector2 operator+(const Vector2& v) const;
    void    operator+=(const Vector2& v);
    Vector2 operator-(const Vector2& v) const;
    void    operator-=(const Vector2& v);
    float   operator*(const Vector2& v) const;
    Vector2 operator*(float f) const;
    void    operator*=(float f);
    Vector2 operator/(float f) const;
    void    operator/=(float f);

    bool operator==(const Vector2& v) const;
    bool operator!=(const Vector2& v) const;
    bool operator>(const Vector2& v) const;
    bool operator<(const Vector2& v) const;

    operator float*(); // 定义强制转换运算符，将 Vector2 转换为 float*
    operator Point() const;

    void rotate(float angle);
    void to_unit();
    void to_zero();

    float module() const;

    bool approx_zero() const;
};
#define VECTOR2_ZERO Vector2{ 0.0f, 0.0f }
#define VECTOR2_UNIT_UP Vector2{ 0.0f, -1.0f }
#define VECTOR2_UNIT_DOWN Vector2{ 0.0f, 1.0f }
#define VECTOR2_UNIT_LEFT Vector2{ -1.0f, 0.0f }
#define VECTOR2_UNIT_RIGHT Vector2{ 1.0f, 0.0f }


class Timer
{
public:
    Timer()  = default;
    ~Timer() = default;

public:
    void On_update(float dt);
    void Restart(float t = 0);
    void Shot(); // 立即触发

public:
    bool is_paused   = false; // 计时器是否暂停
    bool is_one_shot = false; // 计时器是否为一次性触发

    void Set_wait_time(float t) { t > 0.0f ? wait_time = t : wait_time = 0.0f; }
    void Set_wait_time_add(float t) { t > 0.0f ? wait_time += t : wait_time = 0.0f; }
    void Set_wait_time_mul(float t) { t > 0.0f ? wait_time *= t : wait_time = 0.0f; }
    void Set_on_timeout(Callback f) { on_timeout = f; }

    const float& Get_pass_time() const { return pass_time; }
    const bool&  Get_is_shotted() const { return is_shotted; }

private:
    float pass_time  = 0;     // 计时器已经过去的时间
    float wait_time  = 0;     // 计时器等待的时间
    bool  is_shotted = false; // 计时器是否已经触发过一次

    Callback on_timeout = nullptr; // 超时回调函数
};


// 最基础的图形类，用四个字节的数组表示一个矩阵，每个字节表示一个像素点的颜色
class Shape
{
public:
    Shape() = default;
    ~Shape(); // **记得释放内存**

public:
    // 重新设置形状
    void Shape_reset(int = 0, int = 0, int = 0);
    void Shape_reset(const int*, int, int);

    // 合并两个形状
    void Shape_merge(Shape*, std::function<void(int&, int&)> f, int dx = 0, int dy = 0);

    // 获取形状的宽高、缓冲区大小
    int Shape_wide() const;
    int Shape_high() const;
    int Shape_long() const;

    int* Shape_buffer() const;

    // 获取形状某个点的值
    int Shape_in(int) const;
    int Shape_in(int, int) const;

    // 绘制圆形
    void Shape_draw_point(int, int, int = 0);
    void Shape_draw_line(int, int, int, int, int = 0);
    void Shape_draw_rectangle(int, int, int, int, int = 0);
    void Shape_draw_circle(int, int, int, int = 0);

    // 复制形状
    void Shape_copy(Shape*);
    // 设置所有单位的值
    void Shape_clear(int = 0);

protected:
    int* shape_buffer = nullptr;
    int  shape_wide   = 0;
    int  shape_high   = 0;
    int  shape_long   = 0;
};


// Object
class Object
{
public:
    Object() = default;
    Object(const Vector2& pos, const Vector2& vel = VECTOR2_ZERO, float mass = 0, const Color& color = COLOR_BLACK);
    Object(float radius, float mass, const Color& color);
    ~Object() = default;

public:
    virtual void On_update(float delta_time);
    virtual void On_render() const;

public:
    bool is_use_friction       = false;
    bool is_use_air_resistance = false;

    float movement_friction       = 0.0f;
    float movement_air_resistance = 0.0f;

    void Force(const Vector2& force);
    void Move(const Vector2& move);
    void Move_to(const Vector2& dst);
    void Move_to_x(float dst_x);
    void Move_to_y(float dst_y);

    void Stop_move();
    void Stop_move_x();
    void Stop_move_y();

    const Vector2& Get_position() const;
    const Vector2& Get_velocity() const;
    const Vector2& Get_acceleration() const;
    float          Get_mass() const;
    float          Get_radius() const;
    const Color&   Get_color() const;

    void Set_position(const Vector2& position);
    void Set_velocity(const Vector2& velocity);
    void Set_acceleration(const Vector2& acceleration);
    void Set_mass(float mass);
    void Set_radius(float radius);
    void Set_color(const Color& color);

protected:
    Vector2 movement_position;     // 位置
    Vector2 movement_velocity;     // 速度
    Vector2 movement_acceleration; // 加速度
    float   movement_mass = 0;     // 质量（为0时视为质量无穷大）
    float   object_radius = 0;     // 半径
    Color   object_color;          // 颜色

    Vector2 acceleration_last_frame; // 上一帧的加速度
};


// View
class View
{
public:
    void Set_view_center_position(const Vector2& pos);
    void Set_view_size(const Vector2& size);
    void Set_unit_size(float size);

    const Vector2& Get_view_center_position() const;
    const Vector2& Get_view_size() const;
    const Vector2& Get_view_size_half() const;
    const Vector2& Get_view_left_top_position() const;
    const Vector2& Get_view_right_bottom_position() const;

    const float& Get_unit_size() const;

private:
    Vector2 view_center_position;       // 视野中心位置
    Vector2 view_size;                  // 视野宽高
    Vector2 view_size_half;             // 视野宽高的一半
    Vector2 view_left_top_position;     // 视野左上角位置
    Vector2 view_right_bottom_position; // 视野右下角位置

    float unit_size = 1.0f; // 一个单位长度的大小（像素）
};


template<typename Tem>
void
swap(Tem& a, Tem& b)
{
    static Tem c;

    c = a;
    a = b;
    b = c;
}

bool
operator==(const Point& a, const Point& b);

bool
operator!=(const Point& a, const Point& b);

bool
operator<(const Point& a, const Point& b);

bool
operator>(const Point& a, const Point& b);

bool
operator<=(const Point& a, const Point& b);

bool
operator>=(const Point& a, const Point& b);

Point
operator+(const Point& a, const Point& b);

Point
operator-(const Point& a, const Point& b);
