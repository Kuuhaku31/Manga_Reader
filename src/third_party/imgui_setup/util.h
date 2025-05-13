
// util.h

#pragma once

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <functional>
#include <stdint.h>

typedef SDL_Event    Event;
typedef SDL_Texture  Texture;
typedef SDL_Renderer Renderer;
typedef ImFont       Font;
typedef Mix_Chunk    Sound;
typedef Mix_Music    Music;

typedef std::function<void()>             Callback;
typedef std::function<void(const Event&)> EventCallback; // 事件回调函数

class AnimationTemplate;
