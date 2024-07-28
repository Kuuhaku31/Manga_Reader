
#include "MangaReader.hpp"

MangaBook::MangaBook() : volume_index(0), is_combine(false) {}

MangaBook::~MangaBook() {}

bool MangaBook::Load_Volume(std::string path)
{
    // 清空 volumes 容器
    volumes.clear();

    // 遍历目录中的所有文件
    for (const auto &entry : std::filesystem::directory_iterator(path))
    {
        // 如果是文件夹
        if (entry.is_directory())
        {
            // 创建卷对象
            Volume volume;
            std::string volume_path = entry.path().string();
            if (volume.Load(volume_path))
            {
                // 加载成功，将卷对象存储到 volumes 容器中
                volumes.push_back(volume);
            }
        }
    }

    // 按字典序排序
    std::sort(volumes.begin(), volumes.end());

    // 如果 volumes 容器为空，表示加载失败
    return !volumes.empty();
}

void MangaBook::Volume_Next()
{
    if (volume_index < volumes.size() - 1)
    {
        volume_index++;
    }
}

void MangaBook::Volume_Last()
{
    if (volume_index > 0)
    {
        volume_index--;
    }
}

void MangaBook::Volume_Set(int v)
{
    if (v < 0 || v >= volumes.size())
    {
        return;
    }

    volume_index = v;
}

void MangaBook::Page_Turn_Left()
{
    if (is_combine)
    {
        volumes[volume_index].page_index += 2;
    }
    else
    {
        volumes[volume_index].page_index++;
    }

    if (volumes[volume_index].page_index >= volumes[volume_index].volume_pages.size())
    {
        volumes[volume_index].page_index = volumes[volume_index].volume_pages.size() - 1;
    }
}

void MangaBook::Page_Turn_Right()
{
    if (is_combine)
    {
        volumes[volume_index].page_index -= 2;
    }
    else
    {
        volumes[volume_index].page_index--;
    }

    if (volumes[volume_index].page_index < 0)
    {
        volumes[volume_index].page_index = 0;
    }
}

void MangaBook::Page_Index_Move(int i)
{
    volumes[volume_index].page_index += i;

    if (volumes[volume_index].page_index < 0)
    {
        volumes[volume_index].page_index = 0;
    }
    else if (volumes[volume_index].page_index >= volumes[volume_index].volume_pages.size())
    {
        volumes[volume_index].page_index = volumes[volume_index].volume_pages.size() - 1;
    }
}

void MangaBook::Combine_Page()
{
    is_combine = !is_combine;
}

// 从卷中获取图像
inline bool getImgFromVolume(IMAGE *output, Volume *volume, int i = -1)
{
    std::string img_path;
    if (volume->Page_Get(&img_path, i))
    {
        loadimage(output, img_path.c_str(), 0, 0, true);
        return true;
    }
    else
    {
        return false;
    }
}

inline int max(int a, int b) { return a > b ? a : b; }

// 合并两页
inline void combine_img(IMAGE *img1, IMAGE *img2, IMAGE *output)
{
    // 获取两页图像的宽高
    int img1_width = img1->getwidth();
    int img1_heigh = img1->getheight();
    int img2_width = img2->getwidth();
    int img2_heigh = img2->getheight();

    // 输出图像的宽高
    int output_width = img1_width + img2_width;
    int output_heigh = max(img1_heigh, img2_heigh);

    // 初始化输出图像
    SetWorkingImage(output);

    cleardevice();
    output->Resize(output_width, output_heigh);

    putimage(0, 0, img1);
    putimage(img1_width, 0, img2);

    SetWorkingImage();
}

bool MangaBook::Get_Image(IMAGE *output)
{
    // 如果 volumes 容器为空，表示没有漫画
    if (volumes.empty())
    {
        return false;
    }
    else
    {
        Volume *v = &volumes[volume_index];
        if (is_combine)
        {
            IMAGE img1, img2;
            bool f1 = getImgFromVolume(&img1, v, v->page_index);
            bool f2 = getImgFromVolume(&img2, v, v->page_index + 1);
            if (f1 || f2)
            {
                // 合并两页
                combine_img(&img2, &img1, output);
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return getImgFromVolume(output, v);
        }
    }
}

std::string MangaBook::Index_Get()
{
    // 当前卷的书签位置
    int volum_i = volume_index;
    // 当前卷的页位置
    int page_i = volumes[volume_index].page_index;

    return "Volume: " + std::to_string(volum_i) + " Page: " + std::to_string(page_i);
}
