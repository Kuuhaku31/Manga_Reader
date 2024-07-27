
#pragma once

#include <filesystem>
#include <algorithm>
#include <string>
#include <vector>
#include <graphics.h>

class ManngaPages
{
public:
    ManngaPages() {}
    ~ManngaPages() {}

    bool
    Init_Pages(std::string file_path)
    {
        if (std::filesystem::exists(file_path))
        {
            std::vector<std::filesystem::path> jpg_files;

            for (const auto &entry : std::filesystem::directory_iterator(file_path))
            {
                if (entry.path().extension() == ".jpg")
                {
                    jpg_files.push_back(entry.path());
                }
                else if (entry.path().extension() == ".JPEG")
                {
                    jpg_files.push_back(entry.path());
                }
                else if (entry.path().extension() == ".png")
                {
                    jpg_files.push_back(entry.path());
                }
            }
            std::sort(jpg_files.begin(), jpg_files.end());

            for (int i = 0; i < jpg_files.size(); i++)
            {
                IMAGE img;
                loadimage(&img, jpg_files[i].string().c_str(), 0, 0, true);
                manga_pages.push_back(img);
            }
            return true;
        }
        else
        {
            return false;
        }
    }

    int
    Get_Page_Count()
    {
        return manga_pages.size();
    }

    IMAGE *
    Get_Page(int n)
    {
        if (0 > n || manga_pages.size() <= n)
        {
            return nullptr;
        }
        return &manga_pages[n];
    }

private:
    std::vector<IMAGE> manga_pages;
};
