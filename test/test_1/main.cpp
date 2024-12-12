#include <SDL2/SDL.h>
#include <stdio.h>

int
main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO); // 初始化 SDL2 视频子系统
    SDL_SetHint(SDL_HINT_MOUSE_TOUCH_EVENTS, "1");

    SDL_Window*   window   = SDL_CreateWindow("SDL Touch Event", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Event e;
    SDL_bool  quit = SDL_FALSE;

    while(!quit)
    {
        while(SDL_PollEvent(&e))
        {
            switch(e.type)
            {
            case SDL_QUIT:
                quit = SDL_TRUE;
                break;

            case SDL_FINGERDOWN:
                printf("Finger down at (%.2f, %.2f) with finger id %d\n",
                    e.tfinger.x,
                    e.tfinger.y,
                    e.tfinger.fingerId);
                break;

            case SDL_FINGERUP:
                printf("Finger up at (%.2f, %.2f) with finger id %d\n",
                    e.tfinger.x,
                    e.tfinger.y,
                    e.tfinger.fingerId);
                break;

            case SDL_FINGERMOTION:
                printf("Finger moved to (%.2f, %.2f) with finger id %d\n",
                    e.tfinger.x,
                    e.tfinger.y,
                    e.tfinger.fingerId);
                break;

            // 处理鼠标滚轮事件
            case SDL_MOUSEWHEEL:
                printf("Mouse wheel event: x = %d, y = %d\n", e.wheel.x, e.wheel.y);
                break;

            // 处理鼠标点击事件
            case SDL_MOUSEBUTTONDOWN:
                printf("Mouse button down at (%d, %d) button %d\n", e.button.x, e.button.y, e.button.button);
                break;

            default:
                break;
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
