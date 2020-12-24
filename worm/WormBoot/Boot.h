#pragma once
#include <Windows.h>


class CBoot
{
public:
	CBoot();
	~CBoot();
	void Hide(void);
protected:
	int CreatRemoteThreadInjectDll();
	DWORD GetProcessID(const char *ProcessName);
};

