
// combine_page.h

#pragma once

#include <cJSON.h>
#include <cstdint>

void
Combine(cJSON* page_list, uint32_t idx, bool is_single_page, bool is_right_to_left, const char* output_path);
