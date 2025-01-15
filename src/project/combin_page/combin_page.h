
// combin_page.h

#pragma once

#include <SDL.h>
#include <cJSON.h>
#include <cstdint>

void
stb_test();

void
Combine(SDL_Renderer* renderer, cJSON* page_list, uint32_t idx, bool is_single_page, bool is_right_to_left, const char* output_path);
