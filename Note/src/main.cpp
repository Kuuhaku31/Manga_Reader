
#include <iostream>
#include <conio.h>

#include <graphics.h>

#include <windows.h>

// 获取键盘按键的虚拟键码
void getvccode()
{
	std::cout << "Hello World!" << std::endl;

	// 获取键盘按键的虚拟键码
	int vccode = 0;
	do
	{
		vccode = _getch();								// 获取按键的虚拟键码
		std::cout << "vccode: " << vccode << std::endl; // 输出虚拟键码
	} while (vccode != 27);

	std::cout << "Bye!" << std::endl;
}

inline void coutmessage(ExMessage &msg)
{
	std::cout << "message: " << (int)msg.message << std::endl;
	std::cout << "vccode: " << (int)msg.vkcode << std::endl;
	std::cout << "scancode: " << (int)msg.scancode << std::endl
			  << std::endl;
}

void exmessage()
{
	std::cout << "Hello World!" << std::endl;

	// 创建一个窗口
	initgraph(640, 480);

	// 创建一个消息
	ExMessage msg;

	// 拉取消息
	do
	{
		while (peekmessage(&msg))
		{
			if (msg.message == WM_KEYDOWN)
			{
				coutmessage(msg);
			}
		}
	} while (msg.vkcode != 27); // 按下 ESC 键退出

	// 关闭窗口
	closegraph();

	std::cout << "Bye!" << std::endl;
}

int main()
{
	// getvccode();
	exmessage();
	return 0;
}
