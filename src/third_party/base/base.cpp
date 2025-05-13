
// base.cpp

#include "base.h"


bool
operator==(const Point& a, const Point& b)
{
    return a.px == b.px && a.py == b.py;
}

bool
operator!=(const Point& a, const Point& b)
{
    return a.px != b.px || a.py != b.py;
}

bool
operator<(const Point& a, const Point& b)
{
    return a.px < b.px && a.py < b.py;
}

bool
operator>(const Point& a, const Point& b)
{
    return a.px > b.px && a.py > b.py;
}

bool
operator<=(const Point& a, const Point& b)
{
    return a.px <= b.px && a.py <= b.py;
}

bool
operator>=(const Point& a, const Point& b)
{
    return a.px >= b.px && a.py >= b.py;
}

Point
operator+(const Point& a, const Point& b)
{
    return { a.px + b.px, a.py + b.py };
}

Point
operator-(const Point& a, const Point& b)
{
    return { a.px - b.px, a.py - b.py };
}
