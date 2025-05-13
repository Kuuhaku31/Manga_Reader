
// view.cpp

#include "base.h"

void
View::Set_view_center_position(const Vector2& pos)
{
    view_center_position = pos;

    view_left_top_position     = view_center_position - view_size_half;
    view_right_bottom_position = view_center_position + view_size_half;
}

void
View::Set_view_size(const Vector2& size)
{
    view_size      = size;
    view_size_half = size / 2;

    view_left_top_position     = view_center_position - view_size_half;
    view_right_bottom_position = view_center_position + view_size_half;
}

void
View::Set_unit_size(float size)
{
    unit_size = size;
    if(unit_size <= 0) unit_size = 1.0f;
}

const Vector2&
View::Get_view_center_position() const
{
    return view_center_position;
}

const Vector2&
View::Get_view_size() const
{
    return view_size;
}

const Vector2&
View::Get_view_size_half() const
{
    return view_size_half;
}

const Vector2&
View::Get_view_left_top_position() const
{
    return view_left_top_position;
}

const Vector2&
View::Get_view_right_bottom_position() const
{
    return view_right_bottom_position;
}

const float&
View::Get_unit_size() const
{
    return unit_size;
}
