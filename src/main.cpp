
#include "combine.h"
#include "decodeavif.h"

#include <filesystem>
#include <iostream>

std::string root_path   = "D:\\Project\\FishTank\\Manga_Reader\\decodeavif\\images\\";
std::string output_path = "D:\\Project\\FishTank\\Manga_Reader\\decodeavif\\images-output\\";

int ignore_pages[]    = { 1, 2 };
int ignore_pages_size = 2;

std::vector<couple> couples;

int
main()
{
    std::cout << "main start." << std::endl;

    // 创建输出文件夹
    try
    {
        std::filesystem::create_directory(output_path);

        SetGroup(&couples, root_path, ignore_pages, ignore_pages_size);

        for(int i = 0; i < couples.size(); i++)
        {
            CombineAvifToPng(couples[i].imgA_path, couples[i].imgB_path, root_path, "output" + std::to_string(i));
        }
    }
    catch(const std::exception& e)
    {
        std::cout<< "Error: " << e.what() << std::endl;
        std::cout << e.what() << std::endl;
    }

    std::cout << "main end." << std::endl;
    std::cout << "main end." << std::endl;
    std::cout << "main end." << std::endl;
    std::cout << "main end." << std::endl;
    std::cout << "main end." << std::endl;
    std::cout << "main end." << std::endl;

    return 0;
}
