
#pragma once

#include <iostream>
#include <filesystem>
#include <ShlObj.h>
#include <string>
#include <vector>

#include <graphics.h>

#include "ManngaPages.h"

#define PI 3.14159265358979323846264338

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
                print_page_M(page_A);
			}
			else
			{
				print_page_R(page_B);
				print_page_L(page_A);
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
                    revolve ++;
                    break;

                case 'M':
                    revolve --;
                    break;

				case 188:
					size += 0.1;
					break;

				case 190:
					size -= 0.1;
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
                    page_A = page_count - 1;
                    page_B = page_count;
                }

                if(-2 > revolve)
                {
                    revolve = +1;
                }
                if(+2 < revolve)
                {
                    revolve = -1;
                }
			}

		} while (msg.vkcode != VK_RETURN);

		close_book();
	}

private:

    ManngaPages mannga_pages;

    std::string file_path;
    //std::vector<IMAGE> manga_pages;
    int page_count = 0;

	ExMessage msg;

    int revolve = 0;
	bool is_one_page = false;
	int page_A = 0;
    int page_B = 1;

	float size = 1.0f;

	const int WINDOW_WIDE = GetSystemMetrics(SM_CXSCREEN);
	const int WINDOW_HIGH = GetSystemMetrics(SM_CYSCREEN);

	HWND graph_HWND;

	inline bool
	init()
	{
		graph_HWND = initgraph(WINDOW_WIDE, WINDOW_HIGH);
		setbkcolor(0x333333);
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

	/*inline bool
	load_manga_pages()
	{
		if (std::filesystem::exists(file_path))
		{
			std::vector<std::filesystem::path> jpg_files;

			for (const auto& entry : std::filesystem::directory_iterator(file_path))
			{
				if (entry.path().extension() == ".jpg")
				{ 
                    jpg_files.push_back(entry.path());
                }
			}
			std::sort(jpg_files.begin(), jpg_files.end());

            for(int i = 0; i < jpg_files.size(); i++)
			{
				IMAGE img;
                loadimage(&img, jpg_files[i].string().c_str(), 0, 0, true);
                manga_pages.push_back(img);
			}
            page_count = manga_pages.size();

			return true;
		}
		else
		{
			std::cout << "目录不存在: " << file_path << std::endl;
			return false;
		}
	}*/

	void 
	print_page_R(int n)
	{
        if(0 > n || page_count <= n) { return; }

        IMAGE* img = mannga_pages.Get_Page(n);
        int wide = img->getwidth() * size;
		int high = img->getheight() * size;

		StretchBlt
		(
			  GetImageHDC()
			, WINDOW_WIDE / 2 - wide
			, (WINDOW_HIGH - high) / 2
			, wide
			, high
			, GetImageHDC(img)
			, 0
			, 0	
			, img->getwidth()
			, img->getheight()
			, SRCCOPY
		);		
	}

    void
    print_page_L(int n)
    {
        if(0 > n || page_count <= n) { return; }

        IMAGE* img = mannga_pages.Get_Page(n);
        int wide = img->getwidth() * size;
        int high = img->getheight() * size;

        StretchBlt
        (
            GetImageHDC()
            , WINDOW_WIDE / 2
            , (WINDOW_HIGH - high) / 2
            , wide
            , high
            , GetImageHDC(img)
            , 0
            , 0
            , img->getwidth()
            , img->getheight()
            , SRCCOPY
        );
    }

    void
    print_page_M(int n)
	{
        if(0 > n || page_count <= n) { return; }

        IMAGE img;
        switch(revolve)
        {
        case -2:
        case +2:
            rotateimage(&img, mannga_pages.Get_Page(n), PI, 0x333333, true, false);
            break;

        case +1:
            rotateimage(&img, mannga_pages.Get_Page(n), PI/2, 0x333333, true, false);
            break;

        case -1:
            rotateimage(&img, mannga_pages.Get_Page(n), -PI/2, 0x333333, true, false);
            break;

        default:
            img = *mannga_pages.Get_Page(n);
            break;
        }
       
		int wide = img.getwidth() * size;
		int high = img.getheight() * size;

		StretchBlt
		(
			GetImageHDC()
			, (WINDOW_WIDE - wide) / 2
			, (WINDOW_HIGH - high) / 2
			, wide
			, high
			, GetImageHDC(&img)
			, 0
			, 0
			, img.getwidth()
			, img.getheight()
			, SRCCOPY
		);
	}

	void
	close_book()
	{
		closegraph();
	}
};

