
#include "MangaReader.hpp"

// 定义回调函数
int CALLBACK
BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
    if(uMsg == BFFM_INITIALIZED)
    {
        SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
    }
    return 0;
}

// 选择文件夹
bool
getPath(std::string* p)
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
        *p = std::string(path_ch);
        return true;
    }
    else
    {
        return false;
    }
}

// 弹出一个消息框让用户选择是或否，并返回相应的 bool 值
bool
askUserYesNo(const std::string& message, const std::string& title)
{
    int result = MessageBox(
        GetForegroundWindow(), // 获取当前活动窗口的句柄
        message.c_str(),
        title.c_str(),
        MB_YESNO | MB_ICONQUESTION | MB_TOPMOST // 确保消息框在最前端显示
    );

    return (result == IDYES);
}

// 递归地遍历目录，将所有子目录的路径存储在 folders 向量中
void
dfs(std::string path, std::vector<std::string>& folders)
{
    std::cout << "dfs at: " << path << std::endl;

    WIN32_FIND_DATAA FindFileData;                                                  // 用于存储文件查找结果的结构体
    HANDLE           hFind = FindFirstFileA((path + "\\*").c_str(), &FindFileData); // 查找第一个文件或目录

    // 如果查找失败，返回
    if(hFind == INVALID_HANDLE_VALUE) return;
    do
    {
        // 如果找到的是目录
        if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            // 排除当前目录 "." 和父目录 ".."
            if(strcmp(FindFileData.cFileName, ".") != 0 && strcmp(FindFileData.cFileName, "..") != 0)
            {
                // 将子目录路径添加到 folders 向量中
                std::string subpath = path + "\\" + FindFileData.cFileName;
                std::cout << "dfs add: " << subpath << std::endl;
                folders.push_back(subpath);

                // 递归调用 dfs 以遍历子目录
                dfs(subpath, folders);
            }
        }
    } while(FindNextFileA(hFind, &FindFileData)); // 查找下一个文件或目录

    FindClose(hFind); // 关闭查找句柄
}

void
Reader::initBook()
{
    std::cout << "init book" << std::endl;

    // 储存root_path下的所有文件夹路径
    std::vector<std::string> folders;

    // 递归遍历root_path下的所有文件夹
    dfs(root_path, folders);

    // 依次加载每个卷，并添加到book中
    for(const auto& folder : folders)
    {
        Volume volume;
        if(volume.Load(folder, is_read_right_to_left))
        {
            book.Add_Volume(volume);
        }
    }

    std::cout << "init book done" << std::endl;
    std::cout << "book has " << book.Get_Volumes()->size() << " volumes" << std::endl;
}

void
Reader::initGraph()
{
    std::cout << "init graph" << std::endl;

    // 初始化图形窗口
    graph_HWND = initgraph(WINDOW_WIDE, WINDOW_HIGH);
    BeginBatchDraw(); // 开始批量绘图
    setbkcolor(KH_BK_COLOR);
    clearcliprgn();

    graph_HDC = GetImageHDC(NULL);                  // 获取主窗口的 DC
    SetStretchBltMode(graph_HDC, STRETCH_HALFTONE); // 设置拉伸贴图模式为抗锯齿

    // 全屏
    SetWindowLong(graph_HWND, GWL_STYLE, GetWindowLong(graph_HWND, GWL_STYLE) - WS_CAPTION);
    SetWindowPos(graph_HWND, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);

    std::cout << "init graph done" << std::endl;
}

void
Reader::updateGraph()
{
    is_update_graph = false;

    // 清屏
    cleardevice();

    // 画背景
    setfillcolor(KH_BK_COLOR);
    solidrectangle(0, 0, WINDOW_WIDE, WINDOW_HIGH);

    // 画图片
    Volume* v = nullptr;
    if(book.Get_Volume(&v)) v->Get_Image(&output_img);
    imgPrinter.Print_img(&output_img, graph_HDC);

    if(is_print_index)
    {
        // 画书签
        settextcolor(WHITE);
        setbkmode(TRANSPARENT);
        settextstyle(20, 0, _T("Consolas"));
        SetWorkingImage();
        outtextxy(10, 10, book.Get_Volume_Page_Index().c_str());
    }
    std::cout << book.Get_Volume_Page_Index() << std::endl;

    // 刷新
    FlushBatchDraw();
}

void
Reader::exitGraph()
{
    EndBatchDraw(); // 结束批量绘图
    closegraph();
}

void
Reader::processInputs()
{
    // 处理键盘消息
    switch(msg.vkcode)
    {
    case VK_RETURN: exit(); break;          // 回车键       退出
    case VK_ESCAPE: exit(); break;          // ESC 键       退出
    case VK_NEXT: turnRight(); break;       // Page Down    向右翻页
    case VK_PRIOR: turnLeft(); break;       // Page Up      向左翻页
    case VK_RIGHT: turnRight(); break;      // 右键         向右翻页
    case VK_LEFT: turnLeft(); break;        // 左键         向左翻页
    case VK_UP: movRight(); break;          // 上键         向右调整跨页
    case VK_DOWN: movLeft(); break;         // 下键         向左调整跨页
    case VK_SPACE: break;                   // 空格键       未定义
    case VK_OEM_COMMA: rotate_ccw(); break; // , 键         逆时针旋转
    case VK_OEM_PERIOD: rotate_cw(); break; // . 键         顺时针旋转
    case VK_OEM_1: break;                   // ; 键         未定义
    case VK_OEM_7: break;                   // ' 键         未定义
    case VK_OEM_4: nextVolume(); break;     // [ 键         下一卷
    case VK_OEM_6: lastVolume(); break;     // ] 键         上一卷
    case VK_OEM_5: showIndex(); break;      // \ 键         显示页码
    case VK_OEM_2: combine(); break;        // / 键         合并两页
    case VK_OEM_PLUS: magnify(); break;     // = 键         放大
    case VK_OEM_MINUS: shrink(); break;     // - 键         缩小
    case VK_ADD: magnify(); break;          // + 键         放大
    case VK_SUBTRACT: shrink(); break;      // - 键         缩小
    default: break;
    }
}

void
Reader::update()
{
    if(peekmessage(&msg) && msg.message == WM_KEYDOWN)
    {
        // 清空消息队列
        flushmessage();

        Volume* v = nullptr;
        if(book.Get_Volume(&v)) processInputs(); // 处理输入消息
        if(is_update_graph) updateGraph();       // 如果需要更新图像
    }
}

void
Reader::READ()
{
    // 如果成功选择文件夹
    if(getPath(&root_path))
    {
        is_read_right_to_left = askUserYesNo("Read from right to left?", "Read Right to Left");
        initBook();                  // 初始化书本
        initGraph();                 // 初始化图形界面
        updateGraph();               // 更新一次图像
        while(is_continue) update(); // 主循环
        exitGraph();                 // 关闭图形界面
    }
}

void
Reader::magnify()
{
    imgPrinter.Size_Set_d(0.1f);
    is_update_graph = true;
}

void
Reader::shrink()
{
    imgPrinter.Size_Set_d(-0.1f);
    is_update_graph = true;
}

void
Reader::turnLeft()
{
    Volume* v = nullptr;
    if(book.Get_Volume(&v))
    {
        v->Page_Turn_Left();
        is_update_graph = true;
    }
}

void
Reader::turnRight()
{
    Volume* v = nullptr;
    if(book.Get_Volume(&v))
    {
        v->Page_Turn_Right();
        is_update_graph = true;
    }
}

void
Reader::movLeft()
{
    Volume* v = nullptr;
    if(book.Get_Volume(&v))
    {
        v->Page_Index_Move_Left();
        is_update_graph = true;
    }
}

void
Reader::movRight()
{
    Volume* v = nullptr;
    if(book.Get_Volume(&v))
    {
        v->Page_Index_Move_Right();
        is_update_graph = true;
    }
}

void
Reader::nextVolume()
{
    book.Volume_Next();
    is_update_graph = true;
}

void
Reader::lastVolume()
{
    book.Volume_Last();
    is_update_graph = true;
}

void
Reader::rotate_cw()
{
    imgPrinter.Revolve_clockwise();
    is_update_graph = true;
}

void
Reader::rotate_ccw()
{
    imgPrinter.Revolve_counterclockwise();
    is_update_graph = true;
}

void
Reader::combine()
{
    Volume* v = nullptr;
    if(book.Get_Volume(&v))
    {
        v->is_combine   = !v->is_combine;
        is_update_graph = true;
    }
}

void
Reader::showIndex()
{
    is_print_index  = !is_print_index;
    is_update_graph = true;
}

void
Reader::exit()
{
    is_continue = false;
}
