
#pragma once

// 测试函数
int AvifTest();
int EasyXTest();

// 解码 AVIF 文件

// 通过png库编码
int DecodeAvifToPng(const char* inputImagePath, const char* outputImagePath);

// 通过easyx库编码png
int CodePngByEasyX(const char* binPath, int imageW, int imageH, const char* outputImagePath);
