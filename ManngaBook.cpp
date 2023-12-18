
#include "ManngaBook.h"

ManngaBook::ManngaBook() : hwnd(nullptr), imgLeft(), imgRight(), n(0.475)//n(0.775)
{
}

ManngaBook::~ManngaBook()
{
}

void ManngaBook::readBook()
{
	initBook();

	int c = 0;
	int p = 2;
	do
	{
		imgRight = getImg(p);
		imgLeft = getImg(p + 1);
		
		showImg(imgRight, 0);
		showImg(imgLeft, 1);
		
		if (_getch() == 13) { break; }
		c = _getch();

		switch (c)
		{
		case 75:
			p += 2;
			break;

		case 80:
			p += 1;
			break;

		case 77:
			p -= 2;
			break;

		case 72:
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
	hwnd = initgraph(WINDOW_SIZE[0], WINDOW_SIZE[1], SHOWCONSOLE);
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
	loadimage(&img1, (PATH_2 + std::to_string(num) + ".jpg").c_str(), width * n, hight * n);

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

