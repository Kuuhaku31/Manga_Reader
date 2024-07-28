
#include "Input.hpp"

// 更新输入状态
inline void
updateInput(Input& input, ExMessage& msg)
{
    switch(msg.message)
    {
    case 0x100:
        switch(msg.vkcode)
        {
        case 0x57: input.key_W = true; break;
        case 0x53: input.key_S = true; break;
        case 0x41: input.key_A = true; break;
        case 0x44: input.key_D = true; break;

        case 0x49: input.key_I = true; break;
        case 0x4B: input.key_K = true; break;
        case 0x4A: input.key_J = true; break;
        case 0x4C: input.key_L = true; break;

        case 0x51: input.key_Q = true; break;
        case 0x45: input.key_E = true; break;
        case 0x52: input.key_R = true; break;
        case 0x46: input.key_F = true; break;

        case 0x26: input.arr_U = true; break;
        case 0x28: input.arr_D = true; break;
        case 0x25: input.arr_L = true; break;
        case 0x27: input.arr_R = true; break;

        case 0x20: input.space = true; break;
        case 0x10: input.shift = true; break;
        case 0x1B: input.esc__ = true; break;
        case 0x0D: input.enter = true; break;
        }
        break;

    case 0x101:
        switch(msg.vkcode)
        {
        case 0x57: input.key_W = false; break;
        case 0x53: input.key_S = false; break;
        case 0x41: input.key_A = false; break;
        case 0x44: input.key_D = false; break;

        case 0x49: input.key_I = false; break;
        case 0x4B: input.key_K = false; break;
        case 0x4A: input.key_J = false; break;
        case 0x4C: input.key_L = false; break;

        case 0x51: input.key_Q = false; break;
        case 0x45: input.key_E = false; break;
        case 0x52: input.key_R = false; break;
        case 0x46: input.key_F = false; break;

        case 0x26: input.arr_U = false; break;
        case 0x28: input.arr_D = false; break;
        case 0x25: input.arr_L = false; break;
        case 0x27: input.arr_R = false; break;

        case 0x20: input.space = false; break;
        case 0x10: input.shift = false; break;
        case 0x1B: input.esc__ = false; break;
        case 0x0D: input.enter = false; break;
        }
        break;

    case 0x200:
        input.mouse_X = msg.x;
        input.mouse_Y = msg.y;
        break;
    case 0x201: input.mouse_L = true; break;
    case 0x202: input.mouse_L = false; break;
    case 0x204: input.mouse_R = true; break;
    case 0x205: input.mouse_R = false; break;
    case 0x207: input.mouse_M = true; break;
    case 0x208: input.mouse_M = false; break;
    case 0x20a: input.mouse_W = msg.wheel; break;
    default: break;
    }
}

bool
Input::Input_Peekmessage()
{
    if(peekmessage(&msg))
    {
        updateInput(*this, msg);
        return true;
    }
    else
    {
        return false;
    }
}

void
Input::Input_Get_Messagelist()
{
    while(peekmessage(&msg)) { updateInput(*this, msg); }
}
