
// input.cpp

#include "input.h"

#include "config.h"

Input* Input::instance = nullptr;
Input&
Input::Instance()
{
    if(instance == nullptr) instance = new Input();
    return *instance;
}

void
Input::Process_input()
{
    static Config&      config = Config::Instance();
    static ImGui_setup& imgui  = ImGui_setup::Instance();

    for(auto& key : clicked_keys) *key = false; // 清空点击标志

    SDL_Event e;
    while(SDL_PollEvent(&e))
    {
        ImGui_ImplSDL2_ProcessEvent(&e);

        switch(e.type)
        {
        case SDL_QUIT:
        {
            config.Stop_running();
            break;
        }
        case SDL_WINDOWEVENT:
        {
            if(e.window.event == SDL_WINDOWEVENT_CLOSE && e.window.windowID == imgui.Get_window_id())
            {
                config.Stop_running();
            }
            break;
        }
        case SDL_KEYDOWN:
        {
            switch(e.key.keysym.sym)
            {
            case SDLK_ESCAPE:
            {
                clicked_keys.push_back(&is_escape_clicked);
                is_escape_clicked = true;
                is_escape_pressed = true;
                break;
            }
            case SDLK_F11:
            {
                clicked_keys.push_back(&is_F11_clicked);
                is_F11_clicked = true;
                is_F11_pressed = true;
                break;
            }
            case SDLK_MINUS: // "-"
            {
                clicked_keys.push_back(&is_minus_clicked);
                is_minus_clicked = true;
                is_minus_pressed = true;
                break;
            }
            case SDLK_EQUALS: // "="
            {
                clicked_keys.push_back(&is_equals_clicked);
                is_equals_clicked = true;
                is_equals_pressed = true;
                break;
            }
            case SDLK_w: //"w"
            {
                clicked_keys.push_back(&is_key_w_clicked);
                is_key_w_clicked = true;
                is_key_w_pressed = true;
                break;
            }
            case SDLK_s: //"s"
            {
                clicked_keys.push_back(&is_key_s_clicked);
                is_key_s_clicked = true;
                is_key_s_pressed = true;
                break;
            }
            case SDLK_a: //"a"
            {
                clicked_keys.push_back(&is_key_a_clicked);
                is_key_a_clicked = true;
                is_key_a_pressed = true;
                break;
            }
            case SDLK_d: //"d"
            {
                clicked_keys.push_back(&is_key_d_clicked);
                is_key_d_clicked = true;
                is_key_d_pressed = true;
                break;
            }
            case SDLK_c: //"c"
            {
                clicked_keys.push_back(&is_key_c_clicked);
                is_key_c_clicked = true;
                is_key_c_pressed = true;
                break;
            }
            case SDLK_v: //"v"
            {
                clicked_keys.push_back(&is_key_v_clicked);
                is_key_v_clicked = true;
                is_key_v_pressed = true;
                break;
            }
            case SDLK_m: //"m"
            {
                clicked_keys.push_back(&is_key_m_clicked);
                is_key_m_clicked = true;
                is_key_m_pressed = true;
                break;
            }
            case SDLK_UP: // 向上
            {
                clicked_keys.push_back(&is_arrow_up_clicked);
                is_arrow_up_clicked = true;
                is_arrow_up_pressed = true;
                break;
            }
            case SDLK_DOWN: // 向下
            {
                clicked_keys.push_back(&is_arrow_down_clicked);
                is_arrow_down_clicked = true;
                is_arrow_down_pressed = true;
                break;
            }
            case SDLK_LEFT: // 向左
            {
                clicked_keys.push_back(&is_arrow_left_clicked);
                is_arrow_left_clicked = true;
                is_arrow_left_pressed = true;
                break;
            }
            case SDLK_RIGHT: // 向右
            {
                clicked_keys.push_back(&is_arrow_right_clicked);
                is_arrow_right_clicked = true;
                is_arrow_right_pressed = true;
                break;
            }
            case SDLK_COMMA: // ","
            {
                clicked_keys.push_back(&is_comma_clicked);
                is_comma_clicked = true;
                is_comma_pressed = true;
                break;
            }
            case SDLK_PERIOD: //"."
            {
                clicked_keys.push_back(&is_period_clicked);
                is_period_clicked = true;
                is_period_pressed = true;
                break;
            }
            case SDLK_1: //"1"
            {
                clicked_keys.push_back(&is_key_1_clicked);
                is_key_1_clicked = true;
                is_key_1_pressed = true;
                break;
            }
            case SDLK_2: //"2"
            {
                clicked_keys.push_back(&is_key_2_clicked);
                is_key_2_clicked = true;
                is_key_2_pressed = true;
                break;
            }
            case SDLK_3: //"3"
            {
                clicked_keys.push_back(&is_key_3_clicked);
                is_key_3_clicked = true;
                is_key_3_pressed = true;
                break;
            }
            case SDLK_4: //"4"
            {
                clicked_keys.push_back(&is_key_4_clicked);
                is_key_4_clicked = true;
                is_key_4_pressed = true;
                break;
            }
            case SDLK_5: //"5"
            {
                clicked_keys.push_back(&is_key_5_clicked);
                is_key_5_clicked = true;
                is_key_5_pressed = true;
                break;
            }
            case SDLK_6: //"6"
            {
                clicked_keys.push_back(&is_key_6_clicked);
                is_key_6_clicked = true;
                is_key_6_pressed = true;
                break;
            }
            case SDLK_7: //"7"
            {
                clicked_keys.push_back(&is_key_7_clicked);
                is_key_7_clicked = true;
                is_key_7_pressed = true;
                break;
            }
            case SDLK_8: //"8"
            {
                clicked_keys.push_back(&is_key_8_clicked);
                is_key_8_clicked = true;
                is_key_8_pressed = true;
                break;
            }
            case SDLK_9: //"9"
            {
                clicked_keys.push_back(&is_key_9_clicked);
                is_key_9_clicked = true;
                is_key_9_pressed = true;
                break;
            }
            case SDLK_0: //"0"
            {
                clicked_keys.push_back(&is_key_0_clicked);
                is_key_0_clicked = true;
                is_key_0_pressed = true;
                break;
            }
            default: break;
            };
            break;
        }
        case SDL_KEYUP:
        {
            switch(e.key.keysym.sym)
            {
            case SDLK_ESCAPE: is_escape_pressed = false; break;
            case SDLK_F11: is_F11_pressed = false; break;
            case SDLK_w: is_key_w_pressed = false; break;
            case SDLK_s: is_key_s_pressed = false; break;
            case SDLK_a: is_key_a_pressed = false; break;
            case SDLK_d: is_key_d_pressed = false; break;
            case SDLK_c: is_key_c_pressed = false; break;
            case SDLK_v: is_key_v_pressed = false; break;
            case SDLK_m: is_key_m_pressed = false; break;
            case SDLK_UP: is_arrow_up_pressed = false; break;
            case SDLK_DOWN: is_arrow_down_pressed = false; break;
            case SDLK_LEFT: is_arrow_left_pressed = false; break;
            case SDLK_RIGHT: is_arrow_right_pressed = false; break;
            case SDLK_COMMA: is_comma_pressed = false; break;
            case SDLK_PERIOD: is_period_pressed = false; break;
            case SDLK_1: is_key_1_pressed = false; break;
            case SDLK_2: is_key_2_pressed = false; break;
            case SDLK_3: is_key_3_pressed = false; break;
            case SDLK_4: is_key_4_pressed = false; break;
            case SDLK_5: is_key_5_pressed = false; break;
            case SDLK_6: is_key_6_pressed = false; break;
            case SDLK_7: is_key_7_pressed = false; break;
            case SDLK_8: is_key_8_pressed = false; break;
            case SDLK_9: is_key_9_pressed = false; break;
            case SDLK_0: is_key_0_pressed = false; break;
            default: break;
            };
            break;
        }
        case SDL_MOUSEWHEEL:
        {
            break;
        }
        default:
        {
            break;
        }
        }
    }
}
