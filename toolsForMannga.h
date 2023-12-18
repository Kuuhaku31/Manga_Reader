
#pragma once

#include <iostream>
#include <array>
#include <string>

#include <graphics.h>
#include <conio.h>

//#include "HiEasyX.h"

const std::array<int, 2> WINDOW_SIZE = { GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
//const std::array<int, 2> PAGE_SIZE = { WINDOW_SIZE[0] / 2, WINDOW_SIZE[1] };

const std::string PATH_1 = "./Mannga/少女Null v01s DL-Raw.Se/少女Null v01s/P - ";
const std::string PATH_2 = "./Mannga/Test/P - ";

int getMiddleOfHight(int height);
int getMiddleOfWidth(int width, bool isLeftOrRight);

void drawLine();
