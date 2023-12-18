
#include "toolsForMannga.h"

int getMiddleOfHight(int height)
{
	int y = (WINDOW_SIZE[1] - height) / 2;
	return y;
}

int getMiddleOfWidth(int width, bool isLeftOrRight)
{
	int x = 0;
	int m = WINDOW_SIZE[0] / 2;
	isLeftOrRight ? x = m - width : x = m;
	return x;
}

void drawLine()
{
	POINT from, to;						// 画线的起点和终点
	bool drawing = false;				// 是否在画线
	ExMessage msg;						// 消息变量

	setrop2(R2_XORPEN);					// 设置画笔为异或模式
	setlinecolor(GREEN);				// 设置画线颜色为绿色

	do
	{
		msg = getmessage();				// 获取消息

		switch (msg.message)
		{
		case WM_LBUTTONDOWN:		// 当左键按下时
			setcapture();
			from.x = to.x = msg.x;
			from.y = to.y = msg.y;
			drawing = true;
			line(from.x, from.y, to.x, to.y);
			break;

		case WM_LBUTTONUP:			// 当左键抬起时
			releasecapture();
			drawing = false;
			break;

		case WM_MOUSEMOVE:			// 当鼠标移动时
			if (drawing)
			{
				line(from.x, from.y, to.x, to.y);		// 擦掉上次的线
				to.x = msg.x;
				to.y = msg.y;
				line(from.x, from.y, to.x, to.y);		// 画新的线
			}
			break;

		default:
			break;
		}
	} while (msg.vkcode != VK_RETURN);				// 当按下回车键时退出
}
