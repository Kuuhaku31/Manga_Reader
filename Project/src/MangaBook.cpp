
#include "MangaReader.hpp"

void
MangaBook::Add_Volume(Volume v)
{
    volumes.push_back(v);
}

void
MangaBook::Del_Volume(int i)
{
    if(i == -1)
    {
        i = volume_index;
    }

    if(i < 0 || i >= volumes.size())
    {
        return;
    }

    volumes.erase(volumes.begin() + i);
}

void
MangaBook::Volume_Next()
{
    if(volume_index < volumes.size() - 1)
    {
        volume_index++;
    }
}

void
MangaBook::Volume_Last()
{
    if(volume_index > 0)
    {
        volume_index--;
    }
}

void
MangaBook::Volume_Set(int v)
{
    if(v < 0 || v >= volumes.size())
    {
        return;
    }

    volume_index = v;
}

bool
MangaBook::Get_Volume(Volume** v)
{
    if(volumes.empty())
    {
        return false;
    }
    else
    {
        *v = &volumes[volume_index];
        return true;
    }
}

std::string
MangaBook::Get_Volume_Page_Index()
{
    std::string v_path  = volumes[volume_index].Get_Path();                  // 获取卷路径
    std::string p_index = std::to_string(volumes[volume_index].Get_Index()); // 获取当前页书签
    std::string v_index = std::to_string(volume_index);                      // 获取当前卷书签

    return "Volume: " + v_index + " Page: " + p_index + "  Volume Path: " + v_path;
}
