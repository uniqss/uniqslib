#include "http_request.hpp"
#include "http_request_thread.hpp"

#include <thread>
#include <chrono>
#include <atomic>

#include <string>
#include <iostream>
using std::cout;
using std::cin;
using std::endl;
using std::string;

HttpRequest g_oRequest;
HttpRequest* http_request;

std::atomic<bool> g_bdeal_thread_working = false;
std::atomic<bool> g_bdeal_thread_exited = false;

int main()
{
	cout<<"the begin."<<endl;
	http_request = &g_oRequest;

	g_bdeal_thread_working = true;
	printf("11111\n");

	std::thread thread_deal_request(&deal_request_thread2);
	thread_deal_request.detach();
	std::this_thread::sleep_for(std::chrono::milliseconds(1));

	printf("calling do_test\n");
	do_test(http_request, 100);
	printf("the end.\n");
	string strTmp;
	while(true)
	{
		cin>>strTmp;
		if (strTmp == "e" || strTmp == "E")
		{
			g_bdeal_thread_working = false;
			break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	// 等处理线程跑完
	while (!g_bdeal_thread_exited)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	return 0;
}

