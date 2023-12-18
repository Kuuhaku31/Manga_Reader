
#pragma once

#include "toolsForMannga.h"

class ManngaBook
{
public:
	HWND hwnd;
	IMAGE imgLeft;
	IMAGE imgRight;

	double n;

	ManngaBook();
	~ManngaBook();

	void readBook();
	
	void initBook();
	void closeBook();

	void fullScreen();

	IMAGE getImg(int num);
	void showImg(IMAGE img, bool i);
};

