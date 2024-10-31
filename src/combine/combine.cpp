
#include "combine.h"

namespace fs = std::filesystem;
using std::string;
using std::vector;
using std::cout;
using std::endl;

// 储存文件路径
vector<string> files;

// 检测string是否属于Extension
inline bool
isExtension(string str)
{
    // 可支持的拓展名的集合
    const string Extension[] = { ".jpg", ".jpeg", ".png", ".bmp", ".gif", "" };

    int i = 0;
    while(Extension[i] != "")
    {
        if(str == Extension[i])
        {
            return true;
        }
        i++;
    }

    return false;
}

// 传入文件夹路径，加载文件夹下的所有文件路径（不包括子文件夹）
int
LoadPath(string path)
{
    // 如果路径不存在
    if(!fs::exists(path))
    {
        cout << "路径无法打开: " << path << ::endl;
        return 1;
    }
    // 遍历目录中的所有文件
    for(const auto& entry : fs::directory_iterator(path))
    {
        // 如果是文件
        if(entry.is_regular_file())
        {
            // 获取文件路径和扩展名
            string file_path = entry.path().string();
            string ext       = entry.path().extension().string();

            // 检查文件扩展名是否合法
            if(isExtension(ext))
            {
                // 将图片路径存储到 files 容器中
                files.push_back(file_path);
            }
        }
    }

    return 0;
}

// 处理图像合并的函数，返回是否合并成功
bool
CombineImage(string imageA_path, string imageB_path, string output_path, bool is_swap = false)
{
    // 如果需要交换两张图片的位置
    if(is_swap)
    {
        string temp = imageA_path;
        imageA_path = imageB_path;
        imageB_path = temp;
    }

    // 读取两张图片
    IMAGE imageA;
    IMAGE imageB;
    loadimage(&imageA, imageA_path.c_str());
    loadimage(&imageB, imageB_path.c_str());

    // 获取两张图片的宽高
    int wA = imageA.getwidth();
    int hA = imageA.getheight();
    int wB = imageB.getwidth();
    int hB = imageB.getheight();

    // 创建新的图片
    IMAGE* pr_imageC = new IMAGE;

    int wC = wA + wB;
    int hC = hA > hB ? hA : hB;

    pr_imageC->Resize(wC, hC);

    // 将两张图片合并到新的图片中
    SetWorkingImage(pr_imageC);
    putimage(0, (hC - hA) / 2, &imageA);
    putimage(wA, (hC - hB) / 2, &imageB);

    saveimage((LPCTSTR)output_path.c_str(), pr_imageC);

    // 释放内存
    delete pr_imageC;

    cout << "成功合并图片: " << imageA_path << " 和 " << imageB_path << " 到 " << output_path << endl;

    return true;
}

// 处理图像合并的函数，返回是否合并成功
bool
CombineImage(couple cop, string output_path)
{
    return CombineImage(cop.imgA_path, cop.imgB_path, output_path);
}

// 选择文件夹
// 回调函数，用于设置文件夹浏览对话框的初始路径
int CALLBACK
BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
    if(uMsg == BFFM_INITIALIZED)
    {
        std::string tmp = (const char*)lpData;
        SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)tmp.c_str());
    }
    return 0;
}

// 获取文件夹路径
string
getPath()
{
    std::string defaultPath = "D:\\manga";

    char path_ch[4096];

    BROWSEINFO bInfo = { 0 };
    bInfo.hwndOwner  = GetForegroundWindow();      // 父窗口
    bInfo.lpszTitle  = TEXT("Choose a folder..."); // 标题
    bInfo.ulFlags    = BIF_RETURNONLYFSDIRS | BIF_USENEWUI /*包含一个编辑框 用户可以手动填写路径 对话框可以调整大小之类的..*/ | BIF_UAHINT /*带TIPS提示*/;

    bInfo.lpfn   = BrowseCallbackProc;
    bInfo.lParam = reinterpret_cast<LPARAM>(defaultPath.c_str());

    LPITEMIDLIST lpDlist;
    lpDlist = SHBrowseForFolder(&bInfo); // 显示文件夹浏览对话框

    if(lpDlist != NULL)
    {
        SHGetPathFromIDList(lpDlist, path_ch);
        return std::string(path_ch);
    }
    else
    {
        return "";
    }
}

// 将所有的图片路径各自分组
// 判断一张图片的情况
bool
isSingle(int count, string pathA, string pathB, int* ignore_pages, int ignore_pages_size)
{
    IMAGE imageA;
    loadimage(&imageA, (LPCTSTR)pathA.c_str());
    IMAGE imageB;
    loadimage(&imageB, (LPCTSTR)pathB.c_str());

    int wA = imageA.getwidth();
    int hA = imageA.getheight();
    int wB = imageB.getwidth();
    int hB = imageB.getheight();

    // 如果图片A的宽度大于高度，则判断为单张图片
    if(wA > hA)
    {
        cout << pathA << "是单张图片" << endl;
        return true;
    }

    // 判断是否属于single
    for(int i = 0; i < ignore_pages_size; i++)
    {
        if(count == ignore_pages[i])
        {
            cout << pathA << "是单张图片" << endl;
            return true;
        }
    }

    return false;
}

void
Set_couple(std::vector<couple>* couples, int* ignore_pages, int ignore_pages_size)
{
    // 清空容器
    couples->clear();

    for(int i = 0, count = 0; i < files.size(); count++)
    {
        couple cop;
        cop.id = count;

        string imageA_path = files[i];
        string imageB_path;
        if(i + 1 < files.size())
        {
            imageB_path = files[i + 1];
        }

        // 如果判断为一组
        if(isSingle(count, imageA_path, imageB_path, ignore_pages, ignore_pages_size))
        {
            cout << "将" << imageA_path << "判断为一组" << endl;
            cop.imgA_path = imageA_path;
            couples->push_back(cop);
            i++;
        }
        else
        {
            cout << "将" << imageA_path << "和" << imageB_path << "判断为一组" << endl;
            cop.imgA_path = imageA_path;
            cop.imgB_path = imageB_path;
            cop.swap();

            couples->push_back(cop);
            i += 2;
        }
    }
}

int
Combine(string root_path, int* ignore_pages, int ignore_pages_size)
{
    cout << "Combine start." << endl;

    // 获取文件夹路径
    // root_path   = getPath();
    string output_path = root_path + "-combine";

    LoadPath(root_path);

    // 输出文件数量、所有文件路径
    cout << "文件数量: " << files.size() << endl;
    for(auto& file : files)
    {
        cout << file << " ";
    }
    cout << endl;

    std::vector<couple> couples;

    // 每两张图片合并为一组
    Set_couple(&couples, ignore_pages, ignore_pages_size);

    // 输出所有图片组
    // 创建输出文件夹
    fs::create_directory(output_path);
    for(auto& cop : couples)
    {
        // 输出合并的图片路径
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "%s\\%03d.jpg", output_path.c_str(), cop.id);
        std::string output_path = buffer;

        CombineImage(cop, output_path);
    }

    cout << "Combine end." << endl;

    return 0;
}
