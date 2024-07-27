
#pragma once

#include <iostream>
#include <filesystem>
#include <ShlObj.h>
#include <string>
#include <vector>

#include <graphics.h>

#include "DATA.hpp"
#include "ManngaPages.hpp"
#include "Camera.hpp"

class ManngaBook
{   public:

	void 
	READ()
	{
		if (!init()) { return; }

		bool r = false;
		do
		{
			cleardevice();
			if (is_one_page)
			{
                camera.Print_page_M(mannga_pages.Get_Page(page_A));
			}
			else
			{
                camera.Print_page_R(mannga_pages.Get_Page(page_B));
                camera.Print_page_L(mannga_pages.Get_Page(page_A));
			}

			if(r)
			{
				Sleep(100);
			}

			if (msg.message == WM_KEYUP); { r = true; }
			if (msg.message == WM_KEYDOWN); { r = false; }

			getmessage(&msg, EX_KEY);
			
			if (msg.message == WM_KEYDOWN)
			{
				switch (msg.vkcode)
				{
				case VK_LEFT:
					page_A += 2;
					break;

				case VK_UP:
                    page_A += 1;
					break;

				case VK_RIGHT:
                    page_A -= 2;
					break;

				case VK_DOWN:
                    page_A -= 1;
					break;

                case 'N':
                    camera.Revolve_pp();
                    break;

                case 'M':
                    camera.Revolve_mm();
                    break;

				case 188:
					camera.Size_pp();
					break;

				case 190:
					camera.Size_mm();
					break;

				case 191:
					is_one_page = !is_one_page;
					break;

				default:
					break;
				}

                page_B = page_A + 1;

				if (page_B < 0)
				{
                    page_A = -1;
                    page_B = 0;
				}
                if(page_A >= page_count)
                {
                    page_A = page_count;
                    page_B = page_count + 1;
                }
			}

		} while (msg.vkcode != VK_RETURN);

        closegraph();
	}

private:

    std::string file_path;

    ManngaPages mannga_pages;
    Camera camera;
    
    int page_count = 0;

	ExMessage msg;

	bool is_one_page = false;
	int page_A = 0;
    int page_B = 1;

	inline bool
	init()
	{
        HWND graph_HWND = initgraph(WINDOW_WIDE, WINDOW_HIGH);
		setbkcolor(KH_BK_COLOR);
		clearcliprgn();

		HDC mainDC = GetImageHDC(NULL);					// 获取主窗口的 DC
		SetStretchBltMode(mainDC, STRETCH_HALFTONE);	// 设置拉伸贴图模式为抗锯齿

		// 全屏
		SetWindowLong(graph_HWND, GWL_STYLE, GetWindowLong(graph_HWND, GWL_STYLE) - WS_CAPTION);
		SetWindowPos(graph_HWND, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);

		char path_ch[4096];

		BROWSEINFO bInfo = { 0 };
		bInfo.hwndOwner = GetForegroundWindow(); //父窗口
		bInfo.lpszTitle = TEXT("Browse File Folder");
		bInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI /*包含一个编辑框 用户可以手动填写路径 对话框可以调整大小之类的..*/ |
			BIF_UAHINT /*带TIPS提示*/;
		LPITEMIDLIST lpDlist;
		lpDlist = SHBrowseForFolder(&bInfo);
		if (lpDlist != NULL)
		{
			SHGetPathFromIDList(lpDlist, path_ch);
			file_path = std::string(path_ch);
		}
		else
		{
			return false;
		}

        bool f = mannga_pages.Init_Pages(file_path);
        page_count = mannga_pages.Get_Page_Count();

		return f;
	}
};

