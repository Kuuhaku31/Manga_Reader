
// vector2.cpp

#include "base.h"

#include <cmath>

Vector2::Vector2(float x, float y)
    : vx(x)
    , vy(y)
{
}

Vector2
Vector2::operator+(const Vector2& v) const
{
    return Vector2(vx + v.vx, vy + v.vy);
}

void
Vector2::operator+=(const Vector2& v)
{
    vx += v.vx;
    vy += v.vy;
}

Vector2
Vector2::operator-(const Vector2& v) const
{
    return Vector2(vx - v.vx, vy - v.vy);
}

void
Vector2::operator-=(const Vector2& v)
{
    vx -= v.vx;
    vy -= v.vy;
}

float
Vector2::operator*(const Vector2& v) const
{
    return vx * v.vx + vy * v.vy;
}

Vector2
Vector2::operator*(float f) const
{
    return Vector2(vx * f, vy * f);
}

void
Vector2::operator*=(float f)
{
    vx *= f;
    vy *= f;
}

Vector2
Vector2::operator/(float f) const
{
    return Vector2(vx / f, vy / f);
}

void
Vector2::operator/=(float f)
{
    vx /= f;
    vy /= f;
}

bool
Vector2::operator==(const Vector2& v) const
{
    return vx == v.vx && vy == v.vy;
}

bool
Vector2::operator!=(const Vector2& v) const
{
    return vx != v.vx || vy != v.vy;
}

bool
Vector2::operator>(const Vector2& v) const
{
    return module() > v.module();
}

bool
Vector2::operator<(const Vector2& v) const
{
    return module() < v.module();
}

Vector2::operator float*()
{
    return &vx;
}

Vector2::operator Point() const
{
    return { (int)std::floor(vx), (int)std::floor(vy) };
}

void
Vector2::rotate(float angle)
{
    vx = vx * cos(angle) - vy * sin(angle);
    vy = vx * sin(angle) + vy * cos(angle);
}

void
Vector2::to_unit()
{
    float m = module();

    if(m != 0)
    {
        vx /= m;
        vy /= m;
    }
}

void
Vector2::to_zero()
{
    vx = 0;
    vy = 0;
}

float
Vector2::module() const
{
    return sqrt(vx * vx + vy * vy);
}

bool
Vector2::approx_zero() const
{
    return module() < 1e-4;
}
