
#include "combine.h"
#include "decodeavif.h"

#include <filesystem>
#include <iostream>

std::string root_path   = "D:\\Project\\FishTank\\Manga_Reader\\images\\chi";
std::string output_path = "D:\\Project\\FishTank\\Manga_Reader\\images\\output\\";

int ignore_pages[]    = { 1 };
int ignore_pages_size = 1;

std::vector<couple> couples;

int
main()
{
    std::cout << "main start." << std::endl;

    root_path   = getPath();
    output_path = root_path + "\\combine\\";

    try
    {
        // 如果路径不存在，创建输出文件夹
        if(!std::filesystem::exists(output_path))
        {
            std::filesystem::create_directory(output_path);
        }

        // 分组
        SetGroup(&couples, root_path, ignore_pages, ignore_pages_size);

        for(int i = 0; i < couples.size(); i++)
        {
            std::string outputname;

            // 生成文件名，用0补齐
            if(couples[i].id < 10)
            {
                outputname = "00" + std::to_string(couples[i].id);
            }
            else if(couples[i].id < 100)
            {
                outputname = "0" + std::to_string(couples[i].id);
            }
            else
            {
                outputname = std::to_string(couples[i].id);
            }

            CombineAvifToPng(couples[i].imgA_path, couples[i].imgB_path, output_path, outputname);
        }
    }
    catch(const std::exception& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
        std::cout << e.what() << std::endl;
    }

    std::cout << "main end." << std::endl;

    return 0;
}
