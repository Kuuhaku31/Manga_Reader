
#include "MangaReader.hpp"

bool
Volume::operator==(const Volume& v)
{
    return volume_path == v.volume_path;
}

bool
Volume::operator!=(const Volume& v)
{
    return volume_path != v.volume_path;
}

bool
Volume::operator<(const Volume& v)
{
    return volume_path < v.volume_path;
}

bool
Volume::operator>(const Volume& v)
{
    return volume_path > v.volume_path;
}

bool
Volume::Load(std::string p, bool reading_directions)
{
    namespace fs = std::filesystem;

    // 如果路径不存在
    if(!fs::exists(p))
    {
        std::cout << "Volume path not exists: " << p << std::endl;
        return false;
    }

    volume_path = p;

    // 清空 volume_pages 容器
    volume_pages.clear();

    // 遍历目录中的所有文件
    for(const auto& entry : fs::directory_iterator(volume_path))
    {
        // 如果是文件
        if(entry.is_regular_file())
        {
            // 获取文件路径和扩展名
            std::string file_path = entry.path().string();
            std::string extension = entry.path().extension().string();

            // 检查文件扩展名是否为图片格式
            if(extension == ".jpg" || extension == ".png" || extension == ".jpeg" || extension == ".bmp" || extension == ".gif")
            {
                // 将图片路径存储到 volume_pages 容器中
                volume_pages.push_back(file_path);
            }
        }
    }

    // 如果 volume_pages 为空，表示加载失败
    if(volume_pages.empty())
    {
        std::cout << "Volume pages empty: " << volume_path << std::endl;
        return false;
    }
    else
    {
        // 根据阅读方向排序
        is_read_right_to_left = reading_directions;
        if(is_read_right_to_left)
        {
            // 逆序
            std::reverse(volume_pages.begin(), volume_pages.end());
            page_index = volume_pages.size() - 1;
        }
        else
        {
            // 顺序
            std::sort(volume_pages.begin(), volume_pages.end());
            page_index = 0;
        }

        // 输出成功加载该路径
        std::cout << "Volume loaded: " << volume_path << std::endl;
        return true;
    }
}

int
Volume::Get_Index()
{
    return page_index;
}

std::string
Volume::Get_Page()
{
    return volume_pages[page_index];
}

std::string
Volume::Get_Path()
{
    return volume_path;
}

void
Volume::Page_Turn_Left()
{
    // 如果合并两页
    if(is_combine)
    {
        page_index -= 2;
    }
    else
    {
        page_index -= 1;
    }

    if(page_index < 0)
    {
        page_index = 0;
    }
}

void
Volume::Page_Turn_Right()
{
    // 如果合并两页
    if(is_combine)
    {
        page_index += 2;
    }
    else
    {
        page_index += 1;
    }

    if(page_index >= volume_pages.size())
    {
        page_index = volume_pages.size() - 1;
    }
}

void
Volume::Page_Index_Move_Left()
{
    page_index -= 1;

    if(page_index < 0)
    {
        page_index = 0;
    }
}

void
Volume::Page_Index_Move_Right()
{
    page_index += 1;

    if(page_index >= volume_pages.size())
    {
        page_index = volume_pages.size() - 1;
    }
}

inline int
max(int a, int b)
{
    return a > b ? a : b;
}

// 合并两img
inline void
combine_img(IMAGE* img1, IMAGE* img2, IMAGE* output)
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

bool
Volume::Get_Image(IMAGE* img)
{
    if(volume_pages.empty())
    {
        return false;
    }
    else
    {
        // 如果需要跨页合并，并且当前页不是最后一页
        // 合并当前书签和下一页
        if(is_combine && page_index < volume_pages.size() - 1)
        {
            // 加载两页图像
            IMAGE img1, img2;
            loadimage(&img1, volume_pages[page_index].c_str());
            loadimage(&img2, volume_pages[page_index + 1].c_str());

            // 合并两页图像
            combine_img(&img1, &img2, img);
        }
        else
        {
            // 加载当前页图像
            loadimage(img, volume_pages[page_index].c_str(), 0, 0, true);
        }

        return true;
    }
}
