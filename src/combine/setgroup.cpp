
#include "combine.h"

using namespace std;
namespace fs = std::filesystem;

extern vector<string> files;

int
SetGroup(std::vector<couple>* couples, string root_path, int* ignore_pages, int ignore_pages_size)
{
    cout << "SetGroup start." << endl;

    LoadPath(root_path);

    // 输出文件数量、所有文件路径
    cout << "文件数量: " << files.size() << endl;
    for(auto& file : files)
    {
        cout << file << " ";
    }
    cout << endl;

    // 每两张图片合并为一组
    Set_couple(couples, ignore_pages, ignore_pages_size);

    cout << "SetGroup end." << endl;

    return 0;
}
