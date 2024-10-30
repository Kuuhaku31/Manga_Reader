
#include "header.h"
#include <avif/avif.h>
#include <filesystem>
#include <iostream>
#include <png.h>
#include <vector>

using namespace std;
namespace fs = std::filesystem;

// 生成png图片
inline int
buildpng(png_bytep* row_pointers, int w, int h, string outputPath, string fileName)
{
    // 创建 PNG 文件
    FILE* fp = fopen((outputPath + fileName + ".png").c_str(), "wb");
    if(!fp)
    {
        std::cerr << "Failed to open output file" << std::endl;
        return 1;
    }
    // 创建 PNG 结构体
    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    // 创建 PNG 信息结构体
    png_infop info = png_create_info_struct(png);
    // 设置错误处理
    setjmp(png_jmpbuf(png));
    // 初始化 PNG 文件
    png_init_io(png, fp);
    // 设置 PNG 文件信息
    png_set_IHDR(
        png,
        info,
        w,
        h,
        8,
        PNG_COLOR_TYPE_RGBA,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT);
    // 写入 PNG 文件信息
    png_write_info(png, info);
    // 写入 PNG 文件数据
    png_write_image(png, row_pointers);
    // 写入 PNG 文件结束
    png_write_end(png, nullptr);

    // 释放内存
    png_destroy_write_struct(&png, &info);
    // 关闭文件
    fclose(fp);

    return 0;
}

int
CombineAvifToPng(string imgAPath, string imgBPath, string outputPath, string fileName)
{
    cout << "CombineAvifToPng start." << endl;

    // 初始化 AVIF 库
    avifDecoder* decoderA = avifDecoderCreate();
    avifDecoder* decoderB = avifDecoderCreate();

    // 创建 AVIF 图像对象
    avifImage* imageA = avifImageCreateEmpty();
    avifImage* imageB = avifImageCreateEmpty();

    if(!decoderA || !decoderB || !imageA || !imageB)
    {
        std::cerr << "Failed to create AVIF decoder or image" << std::endl;
        return 1;
    }

    // 加载 AVIF 文件
    avifResult resultA = avifDecoderReadFile(decoderA, imageA, imgAPath.c_str());
    avifResult resultB = avifDecoderReadFile(decoderB, imageB, imgBPath.c_str());

    if(resultA != AVIF_RESULT_OK || resultB != AVIF_RESULT_OK)
    {
        std::cerr << "Failed to load AVIF file" << std::endl;
        avifImageDestroy(imageA);
        avifImageDestroy(imageB);
        avifDecoderDestroy(decoderA);
        avifDecoderDestroy(decoderB);
        return 1;
    }

    // 读取文件数据

    // 获取两张图片的宽高
    int wA = imageA->width;
    int hA = imageA->height;
    int wB = imageB->width;
    int hB = imageB->height;

    // avifRGBImage rgbImageA;
    // avifRGBImageSetDefaults(&rgbImageA, imageA);


    // 创建新的图片
    int wC = wA + wB;
    int hC = hA > hB ? hA : hB;

    png_bytep* row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * hC);
    for(int y = 0; y < hC; y++)
    {
        // 一行的数据
        row_pointers[y] = (png_byte*)malloc(wC * 4);
    }

    // 将A的数据拷贝到C的左边
    for(int y = 0; y < hA; y++)
    {
        for(int x = 0; x < wA; x++)
        {
            std::vector<png_byte> buffer;

            avifRGBImage rgbImage;
            avifRGBImageSetDefaults(&rgbImage, imageA);
            rgbImage.format   = AVIF_RGB_FORMAT_RGBA;
            rgbImage.depth    = 8;
            rgbImage.pixels   = buffer.data();
            rgbImage.rowBytes = wA * 4;
            avifResult res    = avifImageYUVToRGB(imageA, &rgbImage);
            if(res != AVIF_RESULT_OK)
            {
                std::cerr << "Failed to convert AVIF image to RGB -A  x: " << x << " y: " << y << std::endl;
                return 1;
            }

            row_pointers[y][x * 4 + 0] = rgbImage.pixels[(y * wA + x) * 4 + 0];
            row_pointers[y][x * 4 + 1] = rgbImage.pixels[(y * wA + x) * 4 + 1];
            row_pointers[y][x * 4 + 2] = rgbImage.pixels[(y * wA + x) * 4 + 2];
            row_pointers[y][x * 4 + 3] = 255;
        }
    }
    // 将B的数据拷贝到C的右边
    for(int y = 0; y < hB; y++)
    {
        for(int x = 0; x < wB; x++)
        {
            avifRGBImage rgbImage;
            avifRGBImageSetDefaults(&rgbImage, imageB);
            avifResult res = avifImageYUVToRGB(imageB, &rgbImage);
            if(res != AVIF_RESULT_OK)
            {
                std::cerr << "Failed to convert AVIF image to RGB -B  x: " << x << " y: " << y << std::endl;
                return 1;
            }

            row_pointers[y][x * 4 + 0 + wA * 4] = rgbImage.pixels[(y * wB + x) * 4 + 0];
            row_pointers[y][x * 4 + 1 + wA * 4] = rgbImage.pixels[(y * wB + x) * 4 + 1];
            row_pointers[y][x * 4 + 2 + wA * 4] = rgbImage.pixels[(y * wB + x) * 4 + 2];
            row_pointers[y][x * 4 + 3 + wA * 4] = 255;
        }
    }

    // 生成png图片
    int res = buildpng(row_pointers, wC, hC, outputPath, fileName);
    if(res)
    {
        std::cerr << "Failed to build png" << std::endl;
        return 1;
    }

    // 释放内存
    for(int y = 0; y < hC; y++)
    {
        free(row_pointers[y]);
    }
    free(row_pointers);

    // 清理
    avifImageDestroy(imageA);
    avifImageDestroy(imageB);
    avifDecoderDestroy(decoderA);
    avifDecoderDestroy(decoderB);

    cout << "CombineAvifToPng end." << endl;

    return 0;
}
