
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
inline bool
getPath(std::string& path)
{
    std::string defaultPath = "D:\\manga";

    char path_ch[ 4096 ];

    BROWSEINFO bInfo = {0};
    bInfo.hwndOwner  = GetForegroundWindow(); // 父窗口
    bInfo.lpszTitle  = TEXT("Browse File Folder");
    bInfo.ulFlags    = BIF_RETURNONLYFSDIRS | BIF_USENEWUI /*包含一个编辑框 用户可以手动填写路径 对话框可以调整大小之类的..*/ | BIF_UAHINT /*带TIPS提示*/;

    bInfo.lpfn   = BrowseCallbackProc;
    bInfo.lParam = reinterpret_cast< LPARAM >(defaultPath.c_str());

    LPITEMIDLIST lpDlist;
    lpDlist = SHBrowseForFolder(&bInfo); // 显示文件夹浏览对话框

    if(lpDlist != NULL)
    {
        SHGetPathFromIDList(lpDlist, path_ch);
        path = std::string(path_ch);
        return true;
    }
    else
    {
        return false;
    }
}

Reader::Reader() {}
Reader::~Reader() {}

void
Reader::READ()
{
    std::string path;
    if(getPath(path) && book.Load_Volume(path))
    {
        // 初始化图形界面
        io.Init_Graph();

        do
        {
        } while(update());

        // 关闭图形界面
        io.Close_Graph();
    }
}

bool
Reader::update()
{
    if(io.Input_Peekmessage() && io.msg.message == WM_KEYDOWN)
    {
        // 清空消息队列
        flushmessage();

        bool is_update_graph = false;
        switch(io.msg.vkcode)
        {

        case VK_RETURN:
            // 如果输入消息为按下return键
            // 立刻返回false
            return false;

            //====================================================================================================
        case VK_RIGHT:
            // 向右翻页
            book.Page_Turn_Right();
            is_update_graph = true;
            break;
        case VK_LEFT:
            // 向左翻页
            book.Page_Turn_Left();
            is_update_graph = true;
            break;
        case VK_UP:
            // 向右翻一页
            book.Page_Index_Move(1);
            is_update_graph = true;
            break;
        case VK_DOWN:
            // 向左翻一页
            book.Page_Index_Move(-1);
            is_update_graph = true;
            break;
            //====================================================================================================

        case VK_SPACE:
            //...
            break;

        case VK_ESCAPE:
            //...
            break;

            //====================================================================================================
        case VK_OEM_COMMA: // ,键
            // 放大
            io.Size_Set_d(0.1f);
            is_update_graph = true;
            break;
        case VK_OEM_PERIOD: // .键
            // 缩小
            io.Size_Set_d(-0.1f);
            is_update_graph = true;
            break;

        case VK_OEM_1: // ;键
            //...
            break;
        case VK_OEM_7: // '键
            //...
            break;

        case VK_OEM_4: // [键
            // 下一卷
            book.Volume_Next();
            is_update_graph = true;
            break;
        case VK_OEM_6: // ]键
            // 上一卷
            book.Volume_Last();
            is_update_graph = true;
            break;

        case VK_OEM_5: // \键
            //...
            break;
        case VK_OEM_2: // /键
            // 切换跨页显示
            book.Combine_Page();
            is_update_graph = true;
            break;
            //====================================================================================================

        default:
            break;
        }

        if(is_update_graph)
        {
            updateGraph();
        }

        return true;
    }
    else
    {
        return true;
    }
}

void
Reader::updateGraph()
{
    // 清屏
    cleardevice();

    // 画背景
    setfillcolor(KH_BK_COLOR);
    solidrectangle(0, 0, WINDOW_WIDE, WINDOW_HIGH);

    // 画图片
    book.Get_Image(&output_img);
    io.Print_img(&output_img);

    // 画书签
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    settextstyle(20, 0, _T("Consolas"));
    outtextxy(10, 10, book.Index_Get().c_str());
}
