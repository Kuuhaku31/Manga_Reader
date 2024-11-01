
#pragma once

#include <string>

struct test
{
    int a;
    int b;
};

// 测试函数
int AvifTest();
int EasyXTest();

// 解码 AVIF 文件
// 通过png库编码
int DecodeAvifToPng(const char* inputImagePath, const char* outputImagePath);

// 通过easyx库编码png
int CodePngByEasyX(const char* binPath, int imageW, int imageH, const char* outputImagePath);

// 传入两张图片的路径，合并为一张png图片
// 将png图片保存到outputPath路径
// fileName为保存的文件名
int CombineAvifToPng(std::string imgAPath, std::string imgBPath, std::string outputPath, std::string fileName);

// Encode AVIF
int EncodeAvif(int argc, char* argv[]);
