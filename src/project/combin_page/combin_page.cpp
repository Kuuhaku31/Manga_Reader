
// combine_page.cpp

#include "combin_page.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <avif.h>

#include <SDL.h>
#include <SDL_image.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

#define DEFAULT_OUTPUT_PATH "output"

void
stb_test()
{
    int width = 256, height = 256;

    std::vector<unsigned char> image(width * height * 3); // RGB 图像

    // 填充图像数据（生成简单的渐变图像）
    for(int y = 0; y < height; ++y)
    {
        for(int x = 0; x < width; ++x)
        {
            int index        = (y * width + x) * 3;
            image[index]     = static_cast<unsigned char>(x); // 红色分量
            image[index + 1] = static_cast<unsigned char>(y); // 绿色分量
            image[index + 2] = 128;                           // 蓝色分量
        }
    }

    // 保存为 PNG 文件
    if(!stbi_write_png("output.png", width, height, 3, image.data(), width * 3))
    {
        printf("Error: stbi_write_png failed.\n");
        return;
    }
}

void
convert_BGR_to_RGB(unsigned char* data, int width, int height, int channels)
{
    if(channels < 3) return; // 如果通道数小于3，不需要转换
    for(int i = 0; i < width * height; ++i)
    {
        std::swap(data[i * channels + 0], data[i * channels + 2]); // 交换B和R
    }
}

void
write_file(const char* filename, const uint8_t* data, size_t size)
{
    namespace fs = std::filesystem;

    fs::path filepath = filename;

    std::ofstream ofs(filepath, std::ios::binary);
    if(!ofs)
    {
        std::cerr << "Error opening file: " << filepath << std::endl;
        return;
    }

    ofs.write(reinterpret_cast<const char*>(data), size);
    ofs.close();

    std::cout << "File '" << std::filesystem::path(filepath).string() << "' created successfully." << std::endl;
}

// 保存 SDL_Surface 为 AVIF 文件
void
SaveSurfaceAsAVIF(SDL_Surface* surface, const char* filename, int quality)
{
    if(!surface)
    {
        std::cerr << "Surface is null, cannot save as AVIF!" << std::endl;
        return;
    }

    // 确保像素格式为 RGB 或 RGBA
    if(surface->format->BytesPerPixel != 3 && surface->format->BytesPerPixel != 4)
    {
        std::cerr << "Unsupported pixel format! Use RGB or RGBA surfaces only." << std::endl;
        return;
    }

    // 初始化 avifImage
    avifImage* avif = avifImageCreate(surface->w, surface->h, 8, AVIF_PIXEL_FORMAT_YUV444);
    if(!avif)
    {
        std::cerr << "Failed to create avifImage!" << std::endl;
        return;
    }

    // 填充像素数据（将 SDL_Surface 转换为 YUV）
    unsigned char* pixels = static_cast<unsigned char*>(surface->pixels);
    avifRGBImage   rgb;
    avifRGBImageSetDefaults(&rgb, avif);
    rgb.format   = (surface->format->BytesPerPixel == 3) ? AVIF_RGB_FORMAT_RGB : AVIF_RGB_FORMAT_RGBA;
    rgb.depth    = 8;
    rgb.rowBytes = surface->pitch;
    rgb.pixels   = pixels;

    if(avifImageRGBToYUV(avif, &rgb) != AVIF_RESULT_OK)
    {
        std::cerr << "Failed to convert RGB to YUV!" << std::endl;
        avifImageDestroy(avif);
        return;
    }

    // 编码 AVIF
    avifEncoder* encoder  = avifEncoderCreate();
    encoder->maxThreads   = 4;
    encoder->minQuantizer = 63 - (quality * 63 / 100); // 根据质量调整量化器
    encoder->maxQuantizer = encoder->minQuantizer;

    avifRWData encodedData = AVIF_DATA_EMPTY;
    avifResult result      = avifEncoderWrite(encoder, avif, &encodedData);
    if(result != AVIF_RESULT_OK)
    {
        std::cerr << "Failed to encode AVIF: " << avifResultToString(result) << std::endl;
    }
    else
    {
        // 写入文件
        write_file(filename, encodedData.data, encodedData.size);
    }

    // 释放资源
    avifRWDataFree(&encodedData);
    avifEncoderDestroy(encoder);
    avifImageDestroy(avif);
}

void
save_image(SDL_Surface* surface, const char* output_path, uint32_t page_idx)
{
    // 生成输出文件名
    static char output_file[256];
    if(!output_path) output_path = DEFAULT_OUTPUT_PATH;
    sprintf(output_file, "%s\\%04d.avif", output_path, page_idx);
    printf("output_file: %s\n", output_file);

    // 保存为 avif 文件
    SaveSurfaceAsAVIF(surface, output_file, 100);
}

void
Combine(SDL_Renderer* renderer, cJSON* page_list, uint32_t idx, bool is_single_page, bool is_right_to_left, const char* output_path)
{
    if(!renderer) return;

    // 检查合法性
    if(!cJSON_IsArray(page_list)) return;
    if(idx >= cJSON_GetArraySize(page_list)) return;

    SDL_Surface* surface_A   = nullptr;
    SDL_Surface* surface_B   = nullptr;
    SDL_Surface* mergedImage = nullptr;

    uint32_t page_idx = 1;

    if(is_single_page)
    {
        // 单独处理一张图片
        cJSON* page = cJSON_GetArrayItem(page_list, idx);
        if(page)
        {
            printf("page: %s\n", cJSON_Print(page));
            const char* path = cJSON_GetStringValue(page);
            if(path) surface_A = IMG_Load(path);
            if(surface_A) surface_B = SDL_ConvertSurfaceFormat(surface_A, SDL_PIXELFORMAT_RGBA32, 0);
        }

        if(surface_B) save_image(surface_B, output_path, page_idx++);

        // 清理
        SDL_FreeSurface(surface_A);
        SDL_FreeSurface(surface_B);

        idx++;
    }

    // 从idx开始，每次取两个元素
    for(uint32_t i = idx; i < cJSON_GetArraySize(page_list); i += 2)
    {
        uint32_t texture_w = 0;
        uint32_t texture_h = 0;

        uint32_t A_w = 0;
        uint32_t A_h = 0;
        uint32_t B_w = 0;
        uint32_t B_h = 0;

        // 加载A
        cJSON* page_A = cJSON_GetArrayItem(page_list, i);
        if(page_A)
        {
            printf("page_A: %s\n", cJSON_Print(page_A));
            const char* path_A = cJSON_GetStringValue(page_A);
            if(path_A) surface_A = IMG_Load(path_A);
        }

        // 加载B
        cJSON* page_B = cJSON_GetArrayItem(page_list, i + 1);
        if(page_B)
        {
            printf("page_B: %s\n", cJSON_Print(page_B));
            const char* path_B = cJSON_GetStringValue(page_B);
            if(path_B) surface_B = IMG_Load(path_B);
        }

        // 如果从右到左，交换A和B
        if(is_right_to_left)
        {
            SDL_Surface* temp = surface_A;
            surface_A         = surface_B;
            surface_B         = temp;
        }

        // 获取A和B的宽高
        if(surface_A)
        {
            A_w = surface_A->w;
            A_h = surface_A->h;
        }
        if(surface_B)
        {
            B_w = surface_B->w;
            B_h = surface_B->h;
        }

        // 计算合并后的纹理大小
        texture_w = A_w + B_w;
        texture_h = A_h > B_h ? A_h : B_h;

        // 创建一个合并后的表面
        mergedImage = SDL_CreateRGBSurface(
            0,
            texture_w,
            texture_h,
            32,
            0x00FF0000,
            0x0000FF00,
            0x000000FF,
            0xFF000000);
        if(!mergedImage)
        {
            printf("Error: %s\n", SDL_GetError());
            SDL_FreeSurface(surface_A);
            SDL_FreeSurface(surface_B);
            return;
        }

        // 将两张图片合并到一个表面上
        SDL_Rect dstrect;
        if(surface_A)
        {
            dstrect.x = 0;
            dstrect.y = 0;
            dstrect.w = A_w;
            dstrect.h = A_h;
            SDL_BlitSurface(surface_A, NULL, mergedImage, &dstrect);
        }
        if(surface_B)
        {
            dstrect.x = A_w;
            dstrect.y = 0;
            dstrect.w = B_w;
            dstrect.h = B_h;
            SDL_BlitSurface(surface_B, NULL, mergedImage, &dstrect);
        }

        // 将合并后的表面转换为纹理
        convert_BGR_to_RGB(
            (unsigned char*)mergedImage->pixels,
            mergedImage->w,
            mergedImage->h,
            mergedImage->format->BytesPerPixel

        );

        save_image(mergedImage, output_path, page_idx++);

        // 清理
        SDL_FreeSurface(surface_A);
        SDL_FreeSurface(surface_B);
        SDL_FreeSurface(mergedImage);
    }
}
