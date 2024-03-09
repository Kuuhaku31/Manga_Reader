
#pragma once

#include <iostream>
#include <array>
#include <string>

#include <graphics.h>
#include <conio.h>

#include <Windows.h>
#include <commdlg.h>
#include <ShlObj.h>
#include <conio.h>


class ManngaBook
{
public:

	std::string path;
	const std::array<int, 2> WINDOW_SIZE_USER = { GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };

	//std::array<int, 2> WINDOW_SIZE_USER;
	//std::array<int, 2> PAGE_SIZE;

	std::string PATH_1 = "./Mannga/少女Null v01s DL-Raw.Se/少女Null v01s/P - ";
	std::string PATH_2 = "./Mannga/Test/P - ";

	HWND hwnd;
	IMAGE imgLeft;
	IMAGE imgRight;

	double n;

	ManngaBook() : hwnd(nullptr), imgLeft(), imgRight(), n(0.475)//n(0.775)
	{}
	~ManngaBook()
	{

	}

	void readBook()
	{
		initBook();

		int p = 2;

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
			path = std::string(path_ch);
		}
		else {
			path = PATH_2;
		}
		do
		{
			imgRight = getImg(p);
			imgLeft = getImg(p + 1);

			showImg(imgRight, 0);
			showImg(imgLeft, 1);

			ExMessage msg;
			getmessage(&msg);
			if (msg.vkcode == VK_RETURN)
			{
				break;
			}

			switch (msg.vkcode)
			{
			case 'a':
				p += 2;
				break;

			case 's':
				p += 1;
				break;

			case 'f':
				p -= 2;
				break;

			case 'd':
				p -= 1;
				break;

			default:
				break;
			}
		} while (1);

		closegraph();
	}

	void initBook()
	{
		hwnd = initgraph(WINDOW_SIZE_USER[0], WINDOW_SIZE_USER[1], SHOWCONSOLE);
		setbkcolor(0x333333);
		clearcliprgn();
		fullScreen();
	}

	void fullScreen()
	{
		SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) - WS_CAPTION);
		SetWindowPos(hwnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);
	}

	IMAGE getImg(int num)
	{
		IMAGE img0;
		loadimage(&img0, (PATH_2 + std::to_string(num) + ".jpg").c_str());

		int width = img0.getwidth();
		std::cout << width << std::endl;
		int hight = img0.getheight();
		std::cout << hight << std::endl;

		IMAGE img1;
		loadimage(&img1, (path + "/" + std::to_string(num) + ".jpg").c_str(), width * n, hight * n);
		//loadimage(&img1, "D:/KH_qBittorrent/static/IKA/1.jpg", width * n, hight * n);

		width = img1.getwidth();
		std::cout << width << std::endl;
		hight = img1.getheight();
		std::cout << hight << std::endl;

		return img1;
	}

	void showImg(IMAGE img, bool i)
	{
		int x = 0;
		int y = 0;

		int m = WINDOW_SIZE_USER[0] / 2;
		i ? x = m - img.getwidth() : x = m;

		y = (WINDOW_SIZE_USER[1] - img.getheight()) / 2;
		
		putimage(x, y, &img);
	}
};

