#include <iostream>

// �ڴ�ؿ��Ա���Ƶ���������ϵͳ�����ڴ���ͷ��ڴ�
// ͬʱ���Ա���̫����ڴ���Ƭ��ʵ���ϲ���ϵͳ�������ڴ���Ƭ��
// �ڴ�ؿ��԰����ϵͳ���õĹ�����Ƭ
int main(int argc, char **argv)
{
	std::cout << "Hello Memory Pool..." << std::endl;

	char *c = new char;
	delete c;

	char* arr = new char[3];
	delete[] arr;

	return 0;
}