
#include "combine.h"

std::string root_path = "D:\\Project\\FishTank\\Manga_Reader\\combine\\img\\01";

int ignore_pages[]    = {0};
int ignore_pages_size = 1;

int
main()
{
    std::cout << "Test Combine" << std::endl;

    Combine(root_path, ignore_pages, ignore_pages_size);

    std::cout << "Test Combine End" << std::endl;

    return 0;
}
