
#pragma once

int AvifTest();
int EasyXTest();

// 解码 AVIF 文件将其转换为32位 BGRA 格式的文本文件
// 返回值：0 成功，非0 失败
int DecodeAvif(const char* inputImagePath, const char* outputImagePath);
int DecodeAvifToPng(const char* inputImagePath, const char* outputImagePath);

// 通过easyx库编码png
int CodePngByEasyX(const char* binPath, int imageW, int imageH, const char* outputImagePath);
