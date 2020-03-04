#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <thread>
#include <chrono>
#include <atomic>

#include "http_request.hpp"
#include "http_request_thread.hpp"


using namespace std;

void do_test(HttpRequest* prequest, uint32_t uid_begin)
{
	FUNC_ENTER;
	const char *get_url = "http://192.168.2.135/versionserver/version.php";
	int nRet = 0;
	uint32_t uid = uid_begin;
	for (int i = 0; i < 0; i++)
	{
		printf("HttpRequest::do_test get %d\n", i);
		nRet = prequest->add_request(++uid, em_get_request, get_url, NULL);
		printf("HttpRequest::DoTest get %d, nRet:%d\n", i, nRet);
	}

	const char *post_url = "http://192.168.2.135/versionserver/version.php";
	//const char *json_str = "{\"userid\":100001, \"nick\":\"markmao\"}";
	const char *json_str = "{}";
	// 每秒50个
	const int nTestPerSecond = 500;
	// 测试时长，秒数
	const int nTestSeconds = 60;

	uint64_t timems_start = get_ms();
	uint64_t timems_loop_begin = get_ms();

	uid = uid_begin;
	for (int i = 0; i < nTestSeconds * nTestPerSecond; i++)
	{
		nRet = prequest->add_request(++uid, em_post_request, post_url, json_str);

		// 计算需要sleep的毫秒数
		uint64_t timems_after_add_request = get_ms();
		uint64_t timems_sleep = 1000 / nTestPerSecond - (timems_after_add_request - timems_loop_begin);

		std::this_thread::sleep_for(std::chrono::milliseconds(timems_sleep));
		//printf("%llu ", timems_sleep);

		timems_loop_begin = timems_after_add_request + timems_sleep;
	}
	FUNC_LEAVE;
	return;
}

extern std::atomic<bool> g_bdeal_thread_working;
extern std::atomic<bool> g_bdeal_thread_exited;

void deal_request_thread2()
{
	FUNC_ENTER;
	printf("deal_request_thread begin\n");

	while (g_bdeal_thread_working) {
		http_request->request_deal_loop();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	g_bdeal_thread_exited = true;

	printf("deal_request_thread end\n");
	FUNC_LEAVE;
}

uint64_t get_ms()
{
	// 取毫秒
	auto time_now = chrono::system_clock::now();
	auto duration_in_ms = chrono::duration_cast<chrono::milliseconds>(time_now.time_since_epoch());
	return duration_in_ms.count();
}

