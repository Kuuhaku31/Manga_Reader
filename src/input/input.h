
// input.h

#pragma once

#include <vector>

// 输入（单例）
class Input
{
public:
    static Input& Instance();

public:
    void Process_input();

public:
    // '-' '='
    bool is_minus_pressed  = false;
    bool is_equals_pressed = false;

    bool is_minus_clicked  = false;
    bool is_equals_clicked = false;

    // ',' '.'
    bool is_comma_pressed  = false;
    bool is_period_pressed = false;

    bool is_comma_clicked  = false;
    bool is_period_clicked = false;

    // 'w' 's' 'a' 'd'
    bool is_key_w_pressed = false;
    bool is_key_s_pressed = false;
    bool is_key_a_pressed = false;
    bool is_key_d_pressed = false;

    bool is_key_w_clicked = false;
    bool is_key_s_clicked = false;
    bool is_key_a_clicked = false;
    bool is_key_d_clicked = false;

    bool is_key_c_pressed = false;
    bool is_key_c_clicked = false;

    bool is_key_v_pressed = false;
    bool is_key_v_clicked = false;

    // 'UP' 'DOWN' 'LEFT' 'RIGHT'
    bool is_arrow_up_pressed    = false;
    bool is_arrow_down_pressed  = false;
    bool is_arrow_left_pressed  = false;
    bool is_arrow_right_pressed = false;

    bool is_arrow_up_clicked    = false;
    bool is_arrow_down_clicked  = false;
    bool is_arrow_left_clicked  = false;
    bool is_arrow_right_clicked = false;

    // '1' '2' '3' '4' '5' '6' '7' '8' '9' '0'
    bool is_key_1_pressed = false;
    bool is_key_2_pressed = false;
    bool is_key_3_pressed = false;
    bool is_key_4_pressed = false;
    bool is_key_5_pressed = false;
    bool is_key_6_pressed = false;
    bool is_key_7_pressed = false;
    bool is_key_8_pressed = false;
    bool is_key_9_pressed = false;
    bool is_key_0_pressed = false;

    bool is_key_1_clicked = false;
    bool is_key_2_clicked = false;
    bool is_key_3_clicked = false;
    bool is_key_4_clicked = false;
    bool is_key_5_clicked = false;
    bool is_key_6_clicked = false;
    bool is_key_7_clicked = false;
    bool is_key_8_clicked = false;
    bool is_key_9_clicked = false;
    bool is_key_0_clicked = false;

private:
    std::vector<bool*> clicked_keys;

private:
    Input()                        = default;
    ~Input()                       = default;
    Input(const Input&)            = delete;
    Input& operator=(const Input&) = delete;

    static Input* instance;
};
