
#include "ManngaBook.h"

int main()
{
	ManngaBook* book = new ManngaBook();
	book->readBook();
	delete book;
	return 0;
}