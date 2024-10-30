
#pragma once

#include <conio.h>
#include <filesystem>
#include <graphics.h>
#include <iostream>
#include <shlobj.h>
#include <vector>

// 一组漫画图片的结构体
struct couple
{
    int id = 0;

    std::string imgA_path = "";
    std::string imgB_path = "";

    // 交换两张图片
    void
    swap()
    {
        std::string temp = imgA_path;

        imgA_path = imgB_path;
        imgB_path = temp;
    }
};

int Combine(std::string root_path, int* ignore_pages = nullptr, int ignore_pages_size = 0);

// 传入文件夹路径，加载文件夹下的所有文件路径（不包括子文件夹）
// 进行分组
// 将结果存储到 couples 中
int SetGroup(std::vector<couple>* couples, std::string root_path, int* ignore_pages = nullptr, int ignore_pages_size = 0);

int  LoadPath(std::string path);
void Set_couple(std::vector<couple>* couples, int* ignore_pages, int ignore_pages_size);
