
#pragma once

#include <graphics.h>

struct Input
{
    ExMessage msg;

    // 按键状态
    bool key_W = false;
    bool key_S = false;
    bool key_A = false;
    bool key_D = false;

    bool key_I = false;
    bool key_K = false;
    bool key_J = false;
    bool key_L = false;

    bool key_Q = false;
    bool key_E = false;
    bool key_R = false;
    bool key_F = false;

    bool arr_U = false;
    bool arr_D = false;
    bool arr_L = false;
    bool arr_R = false;

    bool space = false;
    bool shift = false;
    bool esc__ = false;
    bool enter = false;

    // 鼠标状态
    int mouse_X = 0;
    int mouse_Y = 0;
    bool mouse_L = false;
    bool mouse_R = false;
    bool mouse_M = false;
    int mouse_W = 0;

    // 拉取一个消息
    // 返回值为 false 表示没有消息
    bool Input_Peekmessage();

    // 获取输入队列中的所有消息
    void Input_Get_Messagelist();
};
