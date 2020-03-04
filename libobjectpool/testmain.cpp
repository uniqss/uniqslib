#include "uobjectpool.h"
using namespace UNIQS;

#include <memory>
#include <thread>
#include <string>
#include <iostream>
#include <atomic>
#include <list>
#include <mutex>

class TestPoolObject
{
public:
	TestPoolObject(int i, int ntest2){ nIdx = i; nTest2 = ntest2; }
	int nIdx;
	int nTest2;
};

std::unique_ptr<UObjectPool<TestPoolObject, UMutex>> g_pTestObjectPool;

bool g_bWorking;
int g_nIdx = 0;
int g_nMaxTestCount = 100000000;
int g_nMinFreeCount = 10;
std::atomic<int> g_nObjCount;

std::list<TestPoolObject*> g_listObjs;
std::mutex g_listObjsMutex;

#define PRINT_SLEEP_SECONDS 10
std::atomic<int> g_nAllocInOnePrint;
std::atomic<int> g_nFreeInOnePrint;

void AllocThread()
{
	while (g_bWorking)
	{
		int nIdx = ++g_nIdx;
		int j = 0;
		if (g_nObjCount < g_nMaxTestCount)
		{
			auto * pTmp = g_pTestObjectPool->Alloc(nIdx, j);

			{
				std::lock_guard<std::mutex> lk(g_listObjsMutex);
				g_listObjs.push_back(pTmp);
			}
			++g_nObjCount;
			++g_nAllocInOnePrint;
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
}

void FreeThread()
{
	while (g_bWorking)
	{
		int nIdx = ++g_nIdx;
		int j = 0;
		if (g_nObjCount > g_nMinFreeCount)
		{
			TestPoolObject* pTmp = nullptr;
			{
				std::lock_guard<std::mutex> lk(g_listObjsMutex);
				pTmp = g_listObjs.front();
				g_listObjs.pop_front();
			}
			g_pTestObjectPool->Free(pTmp);
			--g_nObjCount;
			++g_nFreeInOnePrint;
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	for (auto it = g_listObjs.begin(); it != g_listObjs.end(); ++it)
	{
		g_pTestObjectPool->Free(*it);
	}
	g_listObjs.clear();
}

void PrintThread()
{
	while (g_bWorking)
	{
		std::cout
			<< "g_nIdx:" << g_nIdx
			<< " g_nObjCount:" << g_nObjCount
			<< " g_nAllocInOnePrint:" << g_nAllocInOnePrint
			<< " g_nFreeInOnePrint:" << g_nFreeInOnePrint
			<< std::endl;
		g_nAllocInOnePrint = 0;
		g_nFreeInOnePrint = 0;

		std::this_thread::sleep_for(std::chrono::seconds(PRINT_SLEEP_SECONDS));
	}
}

int main()
{
	// 这样写貌似GCC不支持，，，
	//g_pTestObjectPool = std::make_unique<UObjectPool<TestPoolObject, UMutex>>(1024, 1024);
	std::unique_ptr<UObjectPool<TestPoolObject, UMutex>> pTmp(new UObjectPool<TestPoolObject, UMutex>(1024, 1024));
	g_pTestObjectPool = std::move(pTmp);

	g_bWorking = true;
	std::thread a(&AllocThread);
	a.detach();
	std::thread f(&FreeThread);
	f.detach();

	std::thread p(&PrintThread);
	p.detach();

	while (true)
	{
		std::string strInput;
		std::cin >> strInput;
		if (strInput == "e"||strInput == "E")
		{
			break;
		}
		else
		{
			std::cout << "input e to exit." << std::endl;
		}
	}

	g_bWorking = false;

	return 0;
}
