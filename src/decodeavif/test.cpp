
#include "header.h"
#include <avif/avif.h>
#include <filesystem>
#include <fstream>
#include <graphics.h>
#include <iostream>
#include <vector>

namespace fs = std::filesystem;

int
AvifTest()
{
    char inputImagePath[] = "D:\\Project\\FishTank\\Manga_Reader\\avifto\\images\\input.avif";

    std::cout << "Start to test AVIF" << std::endl;

    // 初始化 AVIF 库
    avifDecoder* decoder = avifDecoderCreate();
    if(!decoder)
    {
        std::cerr << "Failed to create AVIF decoder" << std::endl;
        return 1;
    }
    else
    {
        std::cout << "Success to create AVIF decoder" << std::endl;
    }

    // 创建 AVIF 图像对象
    avifImage* image = avifImageCreateEmpty();
    if(!image)
    {
        std::cerr << "Failed to create AVIF image" << std::endl;
        avifDecoderDestroy(decoder);
        return 1;
    }

    // 加载 AVIF 文件
    avifResult result = avifDecoderReadFile(decoder, image, inputImagePath);
    if(result != AVIF_RESULT_OK)
    {
        std::cerr << "Failed to load AVIF file: " << avifResultToString(result) << std::endl;
        avifImageDestroy(image);
        avifDecoderDestroy(decoder);
        return 1;
    }

    // 获取图像信息
    std::cout << "Loaded AVIF image: " << image->width << "x" << image->height << std::endl;

    // 清理
    avifImageDestroy(image);
    avifDecoderDestroy(decoder);
    return 0;
}

int
EasyXTest()
{
    initgraph(640, 480);
    setbkcolor(WHITE);
    cleardevice();
    return 0;
}

int
CodePngByEasyX(const char* binPath, int imageW, int imageH, const char* outputImagePath)
{
    // 读取二进制文件
    std::ifstream ifs(binPath, std::ios::binary);
    if(!ifs)
    {
        std::cerr << "Failed to open input file" << std::endl;
        return 1;
    }

    // 读取图像数据
    std::vector<unsigned int> buffer;
    buffer.resize(imageW * imageH);

    for(int i = 0; i < buffer.size(); i++)
    {
        ifs.read((char*)&buffer[i], sizeof(unsigned int));
    }

    ifs.close();

    // 创建图像并获取图像缓冲区
    IMAGE img;
    img.Resize(imageW, imageH);
    unsigned int* imgBuffer = (unsigned int*)GetImageBuffer(&img);

    // 将图像数据写入图像缓冲区
    for(int i = 0; i < buffer.size(); i++)
    {
        imgBuffer[i] = buffer[i];
    }

    // 保存图像
    saveimage(outputImagePath, &img);

    return 0;
}

// 利用easyx库编码png
void
saveimgbyEasyx(std::vector<unsigned int>& buffer, int imageW, int imageH, const char* outputImagePath)
{
    // 保存解码后的图像数据为png文件
    IMAGE img;
    img.Resize(imageW, imageH);
    unsigned int* p = (unsigned int*)GetImageBuffer(&img);

    for(int i = 0; i < buffer.size(); i++)
    {
        // 交换 R 和 B
        p[i] = BGR(buffer[i]);
    }

    saveimage(outputImagePath, &img);
}
