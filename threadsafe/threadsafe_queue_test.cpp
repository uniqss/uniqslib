#include "threadsafe_queue.h"

#include <thread>
#include <atomic>
#include <chrono>
#include <vector>
#include <string>
#include <iostream>

class TestElement
{
public:
	int nThreadIdx;
	int iii;
};

threadsafe_queue<TestElement> g_TestQueue;

std::atomic<int> push_count;
std::atomic<int> pop_count;
std::atomic<bool> bProducing;
std::atomic<bool> bConsuming;

#define TEST_NUM 4

#define TEST_SECONDS 10

void produce()
{
	while (!bProducing)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	while (bProducing)
	{
		TestElement oTmp;
		oTmp.iii = push_count;
		g_TestQueue.push(oTmp);
		//std::cout << oTmp.iii << "P ";
		++push_count;
	}
}

void consume()
{
	while (bConsuming)
	{
		TestElement oTmp;
		if (g_TestQueue.try_pop(oTmp))
		{
			//std::cout << oTmp.iii << "C ";
			++pop_count;
		}
	}

	// 最后再把队列中的清掉
	TestElement oTmp;
	while (g_TestQueue.try_pop(oTmp))
	{
		//std::cout << oTmp.iii << " ";
		++pop_count;
	}
}

void control()
{
	bProducing = false;
	std::this_thread::sleep_for(std::chrono::seconds(1));

	bProducing = true;
	std::this_thread::sleep_for(std::chrono::seconds(TEST_SECONDS));
	bProducing = false;

	bConsuming = false;
}

int main()
{
	bProducing = false;
	bConsuming = true;

	std::thread t(control);

	std::vector<std::thread> vp;
	std::vector<std::thread> vc;
	for (unsigned nIdx = 0; nIdx < TEST_NUM;++nIdx)
	{
		std::thread p(produce);
		vp.push_back(std::move(p));

		std::thread c(consume);
		vc.push_back(std::move(c));
	}

	t.join();
	for (unsigned nIdx = 0; nIdx < TEST_NUM;++nIdx)
	{
		vp[nIdx].join();
		vc[nIdx].join();
	}

	std::string strInput;
	while (true)
	{
		std::cin >> strInput;
		if (strInput == "E" || strInput == "e")
		{
			break;
		}

		if (strInput == "P"||strInput == "p")
		{
			std::cout << push_count << "\t" << pop_count << std::endl;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	return 0;
}

