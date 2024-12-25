
// combine_page.cpp

#include "combin_page.h"

#include "imgui_setup.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

static ImGui_setup& imgui = ImGui_setup::Instance();

#define DEFAULT_OUTPUT_PATH "output"

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
Combine(cJSON* page_list, uint32_t idx, bool is_single_page, bool is_right_to_left, const char* output_path)
{
    // 检查合法性
    if(!cJSON_IsArray(page_list)) return;
    if(idx >= cJSON_GetArraySize(page_list)) return;

    SDL_Renderer* renderer = imgui.renderer;

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

        if(surface_B)
        {
            // 保存
            char output_file[256];
            if(!output_path) output_path = DEFAULT_OUTPUT_PATH;
            sprintf(output_file, "%s/%04d.png", output_path, page_idx++);
            stbi_write_png(
                output_file,
                surface_B->w,
                surface_B->h,
                4,
                surface_B->pixels,
                surface_B->pitch

            );
            printf("output_file: %s\n", output_file);
        }

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

        // 保存纹理为png
        char output_file[256];
        if(!output_path) output_path = DEFAULT_OUTPUT_PATH;
        // 用0填充，保证文件名长度相同
        sprintf(output_file, "%s/%04d.png", output_path, page_idx++);
        stbi_write_png(
            output_file,
            mergedImage->w,
            mergedImage->h,
            4,
            mergedImage->pixels,
            texture_w * 4);
        printf("output_file: %s\n", output_file);

        // 清理
        SDL_FreeSurface(surface_A);
        SDL_FreeSurface(surface_B);
        SDL_FreeSurface(mergedImage);
    }
}
