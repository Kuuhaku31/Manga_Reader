
#pragma once

#include <iostream>
#include <filesystem>
#include <ShlObj.h>
#include <string>

#include <graphics.h>

class ManngaBook
{
public:

	void 
	READ()
	{
		if (!init()) { return; }

		do
		{
			print_page(manga_pages[now_page], false);
			print_page(manga_pages[now_page + 1], true);
			Sleep(500);
			flushmessage();

			getmessage(&msg);
			switch (msg.vkcode)
			{
			case VK_LEFT:
				now_page += 2;
				break;

			case VK_UP:
				now_page += 1;
				break;

			case VK_RIGHT:
				now_page -= 2;
				break;

			case VK_DOWN:
				now_page -= 1;
				break;

			default:
				break;
			}

			if (now_page < 0)
			{
				now_page = 0;
			}
			if (now_page >= page_num - 1)
			{
				now_page = page_num - 2;
			}

		} while (msg.vkcode != VK_RETURN);

		close_book();
	}

private:

	IMAGE** manga_pages;

	ExMessage msg;

	int page_num = 0;
	int now_page = 0;


	std::string file_path;

	const int WINDOW_WIDE = GetSystemMetrics(SM_CXSCREEN);
	const int WINDOW_HIGH = GetSystemMetrics(SM_CYSCREEN);

	HWND graph_HWND;

	inline bool
	init()
	{
		graph_HWND = initgraph(WINDOW_WIDE, WINDOW_HIGH, SHOWCONSOLE);
		setbkcolor(0x333333);
		clearcliprgn();

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

		return load_manga_pages();
	}

	inline bool
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
			//std::reverse(jpg_files.begin(), jpg_files.end());

			page_num = jpg_files.size();
			manga_pages = new IMAGE* [page_num];

			for (int i = 0; i < page_num; i++)
			{
				manga_pages[i] = new IMAGE();
				loadimage(manga_pages[i], jpg_files[i].string().c_str(), 0, 0, true);
			}
			return true;
		}
		else
		{
			std::cout << "目录不存在: " << file_path << std::endl;
			return false;
		}
	}

	void 
	print_page(IMAGE* img, bool is_in_right)
	{
		int a = WINDOW_WIDE / 2;
		putimage(is_in_right ? a - img->getwidth() : a, (WINDOW_HIGH - img->getheight()) / 2, img);
	}

	void
	close_book()
	{
		delete[] manga_pages;
		closegraph();
	}
};

