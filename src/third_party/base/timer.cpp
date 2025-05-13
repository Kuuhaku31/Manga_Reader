
// timer.h

#include "base.h"

// 重启计时器
void
Timer::Restart(float t)
{
    if(t > 0.0f) wait_time = t;
    pass_time  = 0.0f;
    is_shotted = false;
    is_paused  = false;
}

// 更新计时器
void
Timer::On_update(float dt)
{
    // 如果计时器暂停，则不更新
    if(is_paused) return;

    // 更新计时器
    pass_time += dt;

    // 如果计时器超时
    if(pass_time >= wait_time)
    {
        pass_time -= wait_time;

        // 在保证有回调函数的情况下：
        // 只有 is_one_shot 和 is_shotted 都为 true 时
        // 即计时器为一次性触发且已经触发过一次时
        // 不再触发回调函数
        // 否则触发回调函数
        if(!(is_one_shot && is_shotted) && on_timeout) is_shotted = true, on_timeout();
    }
}

// 立即触发
void
Timer::Shot()
{
    is_shotted = true;
    on_timeout();
}
