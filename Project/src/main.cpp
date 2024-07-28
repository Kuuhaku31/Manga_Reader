
#include <windows.h>

#include "MangaReader.hpp"

#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) // 设置入口地址

int 
main()
{
	Reader r;
	r.READ();

	return 0;
}
