
#pragma once

#include <conio.h>
#include <filesystem>
#include <graphics.h>
#include <iostream>
#include <shlobj.h>
#include <vector>

int Combine(std::string root_path, int* ignore_pages = nullptr, int ignore_pages_size = 0);
