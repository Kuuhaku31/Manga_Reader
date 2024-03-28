
#include <windows.h>

#include "ManngaBook.h"

#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) // 设置入口地址

int 
main()
{
	ManngaBook book;
	book.READ();

	return 0;
}
