#include "stdafx.h"
#include "CGlobalConfig.h"
#include "CCASQueue.h"

vector<string> CGlobalConfig::GlobalScanIpsA;
vector<string> CGlobalConfig::GlobalScanIpsB;
vector<string> CGlobalConfig::GlobalScanIpsC;

vector<string> CGlobalConfig::gLocalIps;
vector<string> CGlobalConfig::GlobalLocalMasks;


//vector<vector<string>> GlobalIps;

map<string, vector<vector<string>>> CGlobalConfig::gMapIps;
map<string, vector<string>> CGlobalConfig::gMapScans;
map<string, vector<string>> CGlobalConfig::gMapVulns;

map<string, vector<string>> CGlobalConfig::GlobalVulns;
map<string, vector<string>> CGlobalConfig::GlobalTotalNets;
map<string, vector<string>> CGlobalConfig::GLobalSelfNets;

//CCASQueue<int> ipsQueue(1000);
