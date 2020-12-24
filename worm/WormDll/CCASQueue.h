#pragma once


#include <Windows.h>
 
template<typename T>
class CCASQueue
{
public:
	struct Node
	{
		Node* pNext;
		T tValue;
	};
public:
	CCASQueue() {}
	/*
	CCASQueue(int nSize) :m_nSizeMax(nSize), m_nSizeCur(0) {
		pHead = pTail = new Node;
		pHead->tValue = -1;
		pHead->pNext = NULL;
	}*/
	~CCASQueue() {
		Node* pTemp = pHead;
		while (pTemp != pTail)
		{
			if (pHead->pNext)
			{
				pHead = pHead->pNext;
				delete pTemp, pTemp = NULL;
				pTemp = pHead;
			}
		}
		if (pTemp)
		{
			delete pTemp;
			pTemp = NULL;
		}
	}
	void init()
	{
		m_nSizeMax = 655350000;
		m_nSizeCur = 0;
		pHead = pTail = new Node;
		pHead->tValue = -1;
		pHead->pNext = NULL;
	}
	void enqueue(const T& val)
	{
		if (InterlockedIncrement(&m_nSizeCur) > m_nSizeMax)
		{
			InterlockedDecrement(&m_nSizeCur);
			return;
		}

		Node* pNodeTemp = new Node;
		pNodeTemp->pNext = NULL;
		pNodeTemp->tValue = val;
		Node* pTailTemp;

		do
		{
			pTailTemp = pTail;
		} while (InterlockedCompareExchange((long*)&(pTailTemp->pNext), (long)pNodeTemp, NULL) != NULL);

		InterlockedCompareExchange((long*)&pTail, (long)pNodeTemp, (long)pTailTemp);
	}

	bool dequeue(T& val)
	{
		Node* pHeadTemp;
		do
		{
			pHeadTemp = pHead;
			if (pHeadTemp->pNext == NULL)
			{
				return false;
			}
		} while (InterlockedCompareExchange((long*)(&pHead), (long)pHead->pNext, (long)pHeadTemp) != (long)pHeadTemp);
		val = pHeadTemp->pNext->tValue;
		InterlockedDecrement(&m_nSizeCur);
		if (pHeadTemp)
		{
			delete pHeadTemp, pHeadTemp = NULL;
		}
		return true;
	}

	long GetCurSize()
	{
		return m_nSizeCur;
	}

private:
	Node* pTail;
	Node* pHead;
	long m_nSizeMax;
	long m_nSizeCur;
};