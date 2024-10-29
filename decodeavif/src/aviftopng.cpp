
#include "header.h"
#include <avif/avif.h>
#include <iostream>
#include <png.h>
#include <vector>

// AVIF 解码器和图像对象
struct avifset
{
    int          result  = 0;
    avifDecoder* decoder = nullptr;
    avifImage*   image   = nullptr;
};

// 初始化
avifset
init(const char* inputImagePath)
{
    avifset set;

    // 初始化 AVIF 库
    avifDecoder* decoder = avifDecoderCreate();
    if(!decoder)
    {
        std::cerr << "Failed to create AVIF decoder" << std::endl;
        set.result = 1;
    }

    // 创建 AVIF 图像对象
    avifImage* image = avifImageCreateEmpty();
    if(!image)
    {
        std::cerr << "Failed to create AVIF image" << std::endl;
        avifDecoderDestroy(decoder);
        set.result = 1;
    }

    // 加载 AVIF 文件
    avifResult result = avifDecoderReadFile(decoder, image, inputImagePath);
    if(result != AVIF_RESULT_OK)
    {
        std::cerr << "Failed to load AVIF file: " << avifResultToString(result) << std::endl;
        avifImageDestroy(image);
        avifDecoderDestroy(decoder);
        set.result = 1;
    }

    set.decoder = decoder;
    set.image   = image;

    return set;
}

// 清理
void
clear(avifset set)
{
    avifImageDestroy(set.image);
    avifDecoderDestroy(set.decoder);
}

// 解码为pngbyte
int
decodeToPng_byte(avifset set, std::vector<png_byte>& buffer)
{
    // 为解码后的图像数据分配内存
    int imageW = set.image->width;
    int imageH = set.image->height;
    buffer.resize(imageW * imageH * 4);

    avifRGBImage rgb;
    avifRGBImageSetDefaults(&rgb, set.image);
    rgb.format   = AVIF_RGB_FORMAT_RGBA;
    rgb.depth    = 8;
    rgb.pixels   = buffer.data();
    rgb.rowBytes = imageW * 4;

    avifResult result = avifImageYUVToRGB(set.image, &rgb);
    if(set.result != AVIF_RESULT_OK)
    {
        std::cerr << "Failed to convert AVIF image to RGB: " << avifResultToString(result) << std::endl;
        avifImageDestroy(set.image);
        avifDecoderDestroy(set.decoder);
        return 1;
    }
    else
    {
        return 0;
    }
}

// 获取图像信息
void
printAvifInfo(avifset set)
{
    std::cout << "Loaded AVIF image: " << set.image->width << "x" << set.image->height << std::endl;
    std::cout << "Image depth: " << set.image->depth << std::endl;
}

int
DecodeAvifToPng(const char* inputImagePath, const char* outputImagePath)
{
    std::cout << "DecodeAvifToPng" << std::endl;

    // 初始化
    avifset set = init(inputImagePath);

    // 获取图像信息
    printAvifInfo(set);

    // 储存解码后的图像数据
    std::vector<png_byte> buffer;

    // 解码 每个像素由 4 个字节组成，分别是 RGBA
    decodeToPng_byte(set, buffer);

    // 转换为png
    FILE* fp = fopen(outputImagePath, "wb");
    if(!fp)
    {
        std::cerr << "Failed to open output file" << std::endl;
        avifImageDestroy(set.image);
        avifDecoderDestroy(set.decoder);
        return 1;
    }

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if(!png)
    {
        std::cerr << "Failed to create PNG write struct" << std::endl;
        fclose(fp);
        avifImageDestroy(set.image);
        avifDecoderDestroy(set.decoder);
        return 1;
    }

    png_infop info = png_create_info_struct(png);
    if(!info)
    {
        std::cerr << "Failed to create PNG info struct" << std::endl;
        png_destroy_write_struct(&png, nullptr);
        fclose(fp);
        avifImageDestroy(set.image);
        avifDecoderDestroy(set.decoder);
        return 1;
    }

    if(setjmp(png_jmpbuf(png)))
    {
        std::cerr << "Failed to set PNG jump buffer" << std::endl;
        png_destroy_write_struct(&png, &info);
        fclose(fp);
        avifImageDestroy(set.image);
        avifDecoderDestroy(set.decoder);
        return 1;
    }

    png_init_io(png, fp);

    png_set_IHDR(
        png,
        info,
        set.image->width,
        set.image->height,
        8,
        PNG_COLOR_TYPE_RGBA,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT);

    png_write_info(png, info);

    for(int y = 0; y < set.image->height; y++)
    {
        png_write_row(png, &buffer[y * set.image->width * 4]);
    }

    png_write_end(png, nullptr);

    png_destroy_write_struct(&png, &info);
    fclose(fp);

    // 清理
    clear(set);

    return 0;
}