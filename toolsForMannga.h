
#pragma once

#include <iostream>
#include <array>
#include <string>

#include <graphics.h>
#include <conio.h>

//#include "HiEasyX.h"

extern const std::array<int, 2> WINDOW_SIZE_USER;
extern const std::array<int, 2> PAGE_SIZE;

#define PATH_1 "./Mannga/少女Null v01s DL-Raw.Se/少女Null v01s/P - "
#define PATH_2 "./Mannga/Test/P - "

extern std::string path;

int getMiddleOfHight(int height);
int getMiddleOfWidth(int width, bool isLeftOrRight);

void drawLine();
