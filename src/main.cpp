
#include "combine.h" // combine/include/combine.h
#include "header.h"  // decodeavif/include/header.h

std::string root_path   = "D:\\Project\\FishTank\\Manga_Reader\\decodeavif\\images\\";
std::string output_path = "D:\\Project\\FishTank\\Manga_Reader\\decodeavif\\images-output\\";

int ignore_pages[]    = { 1, 2 };
int ignore_pages_size = 2;

std::vector<couple> couples;

int
main()
{
    // 创建输出文件夹
    std::filesystem::create_directory(output_path);

    SetGroup(&couples, root_path, ignore_pages, ignore_pages_size);

    for(int i = 0; i < couples.size(); i++)
    {
        CombineAvifToPng(couples[i].imgA_path, couples[i].imgB_path, root_path, "output" + std::to_string(i));
    }

    return 0;
}
