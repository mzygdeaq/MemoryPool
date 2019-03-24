#include <iostream>

// 内存池可以避免频繁的向操作系统申请内存和释放内存
// 同时可以避免太多的内存碎片（实际上操作系统会整理内存碎片）
// 内存池可以帮操作系统更好的管理碎片
int main(int argc, char **argv)
{
	std::cout << "Hello Memory Pool..." << std::endl;

	char *c = new char;
	delete c;

	char* arr = new char[3];
	delete[] arr;

	return 0;
}