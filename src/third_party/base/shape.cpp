
// shape.cpp

#include "base.h"

Shape::~Shape()
{
    Shape_reset();
}

void
Shape::Shape_reset(int w, int h, int v)
{
    if(shape_buffer) delete[] shape_buffer;

    if(w > 0 && h > 0)
    {
        shape_buffer = new int[w * h];
        shape_wide   = w;
        shape_high   = h;
        shape_long   = w * h;

        for(int i = 0; i < shape_long; i++)
        {
            shape_buffer[i] = v;
        }
    }
    else
    {
        shape_buffer = nullptr;
        shape_wide   = 0;
        shape_high   = 0;
        shape_long   = 0;
    }
}

void
Shape::Shape_reset(const int* b, int w, int h)
{
    if(shape_buffer) delete[] shape_buffer;

    if(!b || 0 >= w || 0 >= h)
    {
        shape_buffer = nullptr;
        shape_wide   = 0;
        shape_high   = 0;
        shape_long   = 0;
    }
    else
    {
        shape_buffer = new int[w * h];
        shape_wide   = w;
        shape_high   = h;
        shape_long   = w * h;

        for(int i = 0; i < shape_long; i++)
        {
            shape_buffer[i] = b[i];
        }
    }
}

void
Shape::Shape_merge(Shape* s, std::function<void(int&, int&)> f, int dx, int dy)
{
    int* b1 = shape_buffer;
    int* b2 = s->shape_buffer;

    int A_start = 0;
    int A_skip  = 0;
    int B_start = 0;
    int B_skip  = 0;
    int wide    = s->shape_wide;
    int high    = s->shape_high;

    int R = dx + s->shape_wide - shape_wide;
    int L = dx;
    int T = dy;
    int B = dy + s->shape_high - shape_high;

    if(R > 0)
    {
        wide -= R;
        B_skip += R;
    }
    if(R < 0)
    {
        A_skip -= R;
    }
    if(L > 0)
    {
        A_skip += L;
        A_start += L;
    }
    if(L < 0)
    {
        wide += L;
        B_skip -= L;
        B_start -= L;
    }
    if(B > 0)
    {
        high -= B;
    }
    if(T > 0)
    {
        A_start += shape_wide * T;
    }
    if(T < 0)
    {
        high += T;
        B_start -= s->shape_wide * T;
    }

    for(int i = 0; i < high; i++)
    {
        for(int j = 0; j < wide; j++)
        {
            f(b1[A_start++], b2[B_start++]);
        }
        A_start += A_skip;
        B_start += B_skip;
    }
}

int
Shape::Shape_wide() const
{
    return shape_wide;
}

int
Shape::Shape_high() const
{
    return shape_high;
}

int
Shape::Shape_long() const
{
    return shape_long;
}

int*
Shape::Shape_buffer() const
{
    return shape_buffer;
}

int
Shape::Shape_in(int n) const
{
    if(n >= 0 && n < shape_long)
    {
        return shape_buffer[n];
    }
    else
    {
        return 0;
    }
}

int
Shape::Shape_in(int x, int y) const
{
    return Shape_in(y * shape_wide + x);
}
