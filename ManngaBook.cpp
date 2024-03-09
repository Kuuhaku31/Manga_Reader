﻿
#include "ManngaBook.h"
#include <Windows.h>
#include <commdlg.h>
#include <ShlObj.h>
#include <conio.h>


ManngaBook::ManngaBook() : hwnd(nullptr), imgLeft(), imgRight(), n(0.475)//n(0.775)
{
}

ManngaBook::~ManngaBook()
{
}

void ManngaBook::readBook()
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
	} 
	while (1);

	closeBook();
}

void ManngaBook::initBook()
{
	hwnd = initgraph(WINDOW_SIZE_USER[0], WINDOW_SIZE_USER[1], SHOWCONSOLE);
	setbkcolor(0x333333);
	clearcliprgn();
	fullScreen();
}

void ManngaBook::closeBook()
{
	closegraph();
}

void ManngaBook::fullScreen()
{
	SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) - WS_CAPTION);
	SetWindowPos(hwnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);
}

IMAGE ManngaBook::getImg(int num)
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

void ManngaBook::showImg(IMAGE img, bool i)
{
	int x = 0;
	int y = 0;

	x = getMiddleOfWidth(img.getwidth(), i);
	y = getMiddleOfHight(img.getheight());

	putimage(x, y, &img);
}

