
#include "MangaReader.hpp"

bool Volume::Load(std::string p)
{
    namespace fs = std::filesystem;

    volume_path = p;

    // 清空 volume_pages 容器
    volume_pages.clear();

    // 遍历目录中的所有文件
    for (const auto &entry : fs::directory_iterator(volume_path))
    {
        // 如果是文件
        if (entry.is_regular_file())
        {
            // 获取文件路径和扩展名
            std::string path = entry.path().string();
            std::string extension = entry.path().extension().string();

            // 检查文件扩展名是否为图片格式
            if (extension == ".jpg" || extension == ".png" || extension == ".jpeg" || extension == ".bmp" || extension == ".gif")
            {
                // 将图片路径存储到 volume_pages 容器中
                volume_pages.push_back(path);
            }
        }
    }

    // 按字典序排序
    std::sort(volume_pages.begin(), volume_pages.end());

    // 如果 volume_pages 为空，表示加载失败
    return !volume_pages.empty();
}

bool Volume::Page_Get(std::string *str, int i)
{
    if (i == -1)
    {
        i = page_index;
    }

    if (i < 0 || i >= volume_pages.size())
    {
        return false;
    }
    else
    {
        *str = volume_pages[i];
        return true;
    }
}

bool Volume::operator==(const Volume &v) { return volume_path == v.volume_path; }
bool Volume::operator!=(const Volume &v) { return volume_path != v.volume_path; }
bool Volume::operator<(const Volume &v) { return volume_path < v.volume_path; }
bool Volume::operator>(const Volume &v) { return volume_path > v.volume_path; }
