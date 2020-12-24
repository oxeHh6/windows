#pragma once
#include <vector>
#include <string>
#include <map>
#include <windows.h>
//#include "CCASQueue.h"

using namespace std;



class CGlobalConfig
{
public:

	static vector<string> GlobalScanIpsA;//a类
	static vector<string> GlobalScanIpsB;
	static vector<string> GlobalScanIpsC;

	//static vector<vector<string>> GlobalIps;


	static vector<string> gLocalIps;

	static map<string, vector<vector<string>>> gMapIps;
	static map<string, vector<string>> gMapScans;
	static map<string, vector<string>> gMapVulns;

	static vector<string> GlobalLocalMasks;

	static map<string, vector<string>> GlobalVulns;
	static map<string, vector<string>> GlobalTotalNets;
	static map<string, vector<string>> GLobalSelfNets;



	//static CCASQueue<int> ipsQueue;
};


/*
10.0.0.0~10.255.255.255（A类）
172.16.0.0~172.31.255.255（B类）
192.168.0.0~192.168.255.255（C类）
*/




