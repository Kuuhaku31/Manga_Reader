
#include "decodeavif.h"

#include <avif/avif.h>
#include <filesystem>
#include <iostream>
#include <memory.h>
#include <png.h>
#include <vector>

using namespace std;
namespace fs = std::filesystem;

// 生成avif图片
inline int
buildavif(png_bytep* row_pointers, int w, int h, string outputFilename)
{
    // 创建image
    avifImage* image = avifImageCreate(w, h, 8, AVIF_PIXEL_FORMAT_YUV444);
    if(!image)
    {
        std::cerr << "Failed to create AVIF image" << std::endl;
        return 1;
    }
    else
    {
        cout << "Create AVIF image success." << endl;
    }

    // 写入图片数据

    // 创建avifRGBImage
    avifRGBImage rgbImage;
    memset(&rgbImage, 0, sizeof(rgbImage));
    avifRGBImageSetDefaults(&rgbImage, image);
    avifResult allocationResult = avifRGBImageAllocatePixels(&rgbImage);
    if(allocationResult != AVIF_RESULT_OK)
    {
        fprintf(stderr, "Allocation of RGB samples failed: %s\n", avifResultToString(allocationResult));
        return 1;
    }
    else
    {
        cout << "Allocation of RGB samples success." << endl;
    }

    // 将png数据拷贝到avif数据
    for(int y = 0; y < h; y++)
    {
        for(int x = 0; x < w; x++)
        {
            rgbImage.pixels[(y * w + x) * 4 + 0] = row_pointers[y][x * 4 + 0];
            rgbImage.pixels[(y * w + x) * 4 + 1] = row_pointers[y][x * 4 + 1];
            rgbImage.pixels[(y * w + x) * 4 + 2] = row_pointers[y][x * 4 + 2];
            rgbImage.pixels[(y * w + x) * 4 + 3] = 255;
        }
    }

    // 转换图片数据
    avifResult convertResult = avifImageRGBToYUV(image, &rgbImage);
    if(convertResult != AVIF_RESULT_OK)
    {
        fprintf(stderr, "Failed to convert to YUV(A): %s\n", avifResultToString(convertResult));
        return 1;
    }
    else
    {
        cout << "Convert to YUV(A) success." << endl;
    }

    avifEncoder* encoder = avifEncoderCreate();
    if(!encoder)
    {
        std::cerr << "Failed to create AVIF encoder" << std::endl;
        return 1;
    }
    else
    {
        cout << "Create AVIF encoder success." << endl;
    }
    encoder->quality      = 60;
    encoder->qualityAlpha = AVIF_QUALITY_LOSSLESS;

    // 添加图片
    avifResult addImageResult = avifEncoderAddImage(encoder, image, 1, AVIF_ADD_IMAGE_FLAG_SINGLE);
    if(addImageResult != AVIF_RESULT_OK)
    {
        fprintf(stderr, "Failed to add image to encoder: %s\n", avifResultToString(addImageResult));
        return 1;
    }

    // 定义输出数据
    avifRWData avifOutput   = AVIF_DATA_EMPTY;
    avifResult finishResult = avifEncoderFinish(encoder, &avifOutput);
    if(finishResult != AVIF_RESULT_OK)
    {
        fprintf(stderr, "Failed to finish encode: %s\n", avifResultToString(finishResult));
        return 1;
    }

    printf("Encode success: %zu total bytes\n", avifOutput.size);

    // 写入文件
    FILE*  f            = fopen(outputFilename.c_str(), "wb");
    size_t bytesWritten = fwrite(avifOutput.data, 1, avifOutput.size, f);
    fclose(f);
    if(bytesWritten != avifOutput.size)
    {
        fprintf(stderr, "Failed to write %zu bytes\n", avifOutput.size);
    }
    printf("Wrote: %s\n", outputFilename);

    // 释放内存
    free(rgbImage.pixels);
    avifImageDestroy(image);
    avifEncoderDestroy(encoder);

    cout << "Build avif success." << endl;

    return 0;
}

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
    else
    {
        cout << "Open output file success." << endl;
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
    //

    // 设置压缩级别（0-9，0为无压缩，9为最高压缩）
    png_set_compression_level(png, 9);

    // 设置过滤器（PNG_FILTER_NONE, PNG_FILTER_SUB, PNG_FILTER_UP, PNG_FILTER_AVG, PNG_FILTER_PAETH, PNG_ALL_FILTERS）
    png_set_filter(png, 0, PNG_ALL_FILTERS);

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

    cout << "Build png success." << endl;

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
    else
    {
        cout << "Create AVIF decoder and image success." << endl;
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
    else
    {
        cout << "Load AVIF file success." << endl;
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

    // 分配内存
    // png_bytep 是一个指向一个字节的指针
    png_bytep* row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * hC);
    for(int y = 0; y < hC; y++)
    {
        // 分配一行内存
        row_pointers[y] = (png_byte*)malloc(wC * 4);
        // 初始化为0
        memset(row_pointers[y], 0, wC * 4);
    }

    // 创建avifRGBImage
    avifRGBImage rgbImageA;
    avifRGBImage rgbImageB;
    avifRGBImageSetDefaults(&rgbImageA, imageA);
    rgbImageA.format   = AVIF_RGB_FORMAT_RGBA;
    rgbImageA.depth    = 8;
    rgbImageA.pixels   = (uint8_t*)malloc(wA * hA * 4);
    rgbImageA.rowBytes = wA * 4;
    avifRGBImageSetDefaults(&rgbImageB, imageB);
    rgbImageB.format   = AVIF_RGB_FORMAT_RGBA;
    rgbImageB.depth    = 8;
    rgbImageB.pixels   = (uint8_t*)malloc(wB * hB * 4);
    rgbImageB.rowBytes = wB * 4;

    avifResult resA = avifImageYUVToRGB(imageA, &rgbImageA);
    avifResult resB = avifImageYUVToRGB(imageB, &rgbImageB);
    if(resA != AVIF_RESULT_OK || resB != AVIF_RESULT_OK)
    {
        std::cerr << "Failed to convert AVIF image to RGB" << std::endl;
        return 1;
    }
    else
    {
        cout << "Convert AVIF image to RGB success." << endl;
    }


    // 将A的数据拷贝到C的左边
    for(int y = 0; y < hA; y++)
    {
        for(int x = 0; x < wA; x++)
        {
            row_pointers[y][x * 4 + 0] = rgbImageA.pixels[(y * wA + x) * 4 + 0];
            row_pointers[y][x * 4 + 1] = rgbImageA.pixels[(y * wA + x) * 4 + 1];
            row_pointers[y][x * 4 + 2] = rgbImageA.pixels[(y * wA + x) * 4 + 2];
            row_pointers[y][x * 4 + 3] = 255;
        }
    }
    // 将B的数据拷贝到C的右边
    for(int y = 0; y < hB; y++)
    {
        for(int x = 0; x < wB; x++)
        {
            row_pointers[y][x * 4 + 0 + wA * 4] = rgbImageB.pixels[(y * wB + x) * 4 + 0];
            row_pointers[y][x * 4 + 1 + wA * 4] = rgbImageB.pixels[(y * wB + x) * 4 + 1];
            row_pointers[y][x * 4 + 2 + wA * 4] = rgbImageB.pixels[(y * wB + x) * 4 + 2];
            row_pointers[y][x * 4 + 3 + wA * 4] = 255;
        }
    }

    // 生成png图片
    int res = buildpng(row_pointers, wC, hC, outputPath, fileName);
    // int res = buildavif(row_pointers, wC, hC, outputPath + fileName + ".avif");
    if(res)
    {
        std::cerr << "Failed to build png" << std::endl;
        return 1;
    }
    else
    {
        cout << "Build png success." << endl;
    }

    // 释放内存
    free(rgbImageA.pixels);
    free(rgbImageB.pixels);
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
