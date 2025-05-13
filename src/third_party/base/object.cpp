
// object.cpp

#include "base.h"

#include "imgui_setup.h"

Object::Object(const Vector2& pos, const Vector2& vel, float mass, const Color& color)
    : movement_position(pos)
    , movement_velocity(vel)
    , movement_mass(mass)
    , object_color(color)
{
}

Object::Object(float radius, float mass, const Color& color)
    : object_radius(radius)
    , movement_mass(mass)
    , object_color(color)
{
}

void
Object::On_update(float delta_time)
{
    static float v_mod, f_mod = 0.0f;
    float        vel_x_before = movement_velocity.vx;

    if(v_mod = movement_velocity.module()) // 如果有速度，需要计算阻力
    {
        f_mod = 0.0f;
        if(is_use_air_resistance) f_mod += v_mod * v_mod * movement_air_resistance; // 计算空气阻力
        if(is_use_friction) f_mod += movement_friction;                             // 计算摩擦力
        if(f_mod) movement_acceleration -= (movement_velocity / v_mod) * f_mod;     // 计算阻力加速度
    }

    // 更新速度
    movement_velocity += (movement_acceleration * delta_time);

    // 如果速度方向改变，速度置0
    if(vel_x_before * movement_velocity.vx < 0) movement_velocity.vx = 0;

    // 更新位置
    movement_position += (movement_velocity * delta_time);

    // 更新加速度
    acceleration_last_frame = movement_acceleration;
    movement_acceleration.to_zero();
}

void
Object::On_render() const
{
    static const Painter& painter = Painter::Instance();

    painter.DrawCircle(
        movement_position.vx,
        movement_position.vy,
        object_radius,
        object_color,
        true

    );
}

void
Object::Force(const Vector2& force)
{
    if(movement_mass <= 0) return;
    movement_acceleration += (force / movement_mass);
}

void
Object::Move(const Vector2& move)
{
    movement_position += move;
}

void
Object::Move_to(const Vector2& dst)
{
    movement_position = dst;
}

void
Object::Move_to_x(float dst_x)
{
    movement_position.vx = dst_x;
}

void
Object::Move_to_y(float dst_y)
{
    movement_position.vy = dst_y;
}

void
Object::Stop_move()
{
    movement_acceleration.to_zero();
    movement_velocity.to_zero();
}

void
Object::Stop_move_x()
{
    movement_acceleration.vx = 0;
    movement_velocity.vx     = 0;
}

void
Object::Stop_move_y()
{
    movement_acceleration.vy = 0;
    movement_velocity.vy     = 0;
}

const Vector2&
Object::Get_position() const
{
    return movement_position;
}

const Vector2&
Object::Get_velocity() const
{
    return movement_velocity;
}

const Vector2&
Object::Get_acceleration() const
{
    return acceleration_last_frame;
}

float
Object::Get_mass() const
{
    return movement_mass;
}

float
Object::Get_radius() const
{
    return object_radius;
}

const Color&
Object::Get_color() const
{
    return object_color;
}

void
Object::Set_position(const Vector2& position)
{
    movement_position = position;
}

void
Object::Set_velocity(const Vector2& velocity)
{
    movement_velocity = velocity;
}

void
Object::Set_acceleration(const Vector2& acceleration)
{
    movement_acceleration = acceleration;
}

void
Object::Set_mass(float mass)
{
    movement_mass = mass;
}

void
Object::Set_radius(float radius)
{
    object_radius = radius;
}

void
Object::Set_color(const Color& color)
{
    object_color = color;
}
