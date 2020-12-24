// Mytest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <iostream>

int _tmain(int argc, _TCHAR* argv[])
{
	HMODULE hModule = ::LoadLibrary("wormdll.dll");
	while (1) {
		Sleep(100000);
		std::cout << "Main" << std::endl;
	}
	//Sleep(100000);
	return 0;
}

