#include "http_requester.h"
#include "http_requester_thread.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>

using namespace std;


http_requester g_oRequest;
http_requester* http_request;
std::atomic<bool> g_brequest_thread_working;

int main()
{
	cout<<"xxxx"<<endl;
	printf("main\n");
	http_request = &g_oRequest;
	g_brequest_thread_working = true;
	printf("11111\n");

	std::thread t(&deal_request_thread);
	t.detach();

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	printf("22222\n");
	DoTest(&g_oRequest, 111);
	printf("33333\n");
	string strTmp;
	while(true)
	{
		cin>>strTmp;
		if(strTmp == "e"||strTmp == "E") break;
		g_brequest_thread_working = false;
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	return 0;
}

