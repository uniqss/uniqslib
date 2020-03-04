#include "http_requester_thread.h"
#include "http_requester.h"

#include "log.h"

#include <string.h>
#include <thread>
#include <chrono>

#include "../common_header/debugfunc.h"

extern http_requester* http_request;


void DoTest(http_requester* prequester, uint32_t uid)
{
	FUNC_ENTER;
	const char *get_url = "http://192.168.5.164/index.php?userid=1001&&nick=%mark%";
	int nRet = 0;
	QUERY_PARAM_ON_HTTP_REQUEST oParam;
	for (int i = 0; i < 0; i++)
	{
		printf("HttpRequest::DoTest get %d\n", i);
		nRet = prequester->GetRequest(get_url, NULL, oParam);
		printf("HttpRequest::DoTest get %d, nRet:%d\n", i, nRet);
	}

	const char *post_url[10] = {
		"http://192.168.2.164/index0.php",
		"http://192.168.2.164/index1.php",
		"http://192.168.2.164/index2.php",
		"http://192.168.2.164/index3.php",
		"http://192.168.2.164/index4.php",
		"http://192.168.2.164/index5.php",
		"http://192.168.2.164/index6.php",
		"http://192.168.2.164/index7.php",
		"http://192.168.2.164/index8.php",
		"http://192.168.2.164/index9.php" };
#if 1
	const char* a = "http://192.168.2.135/versionserver/version.php";
	for (int nIdx = 0; nIdx < 500; ++nIdx)
	{
		nRet = prequester->GetRequest(a, NULL, oParam);
		//nRet = prequester->PostRequest(a, "", NULL, oParam);
	}
#else
	const char *json_str = "{\"userid\":100001, \"nick\":\"uniqs\"}";
	for (int i = 0; i < 500; i++)
	{
		nRet = prequester->PostRequest(post_url[i % 10], json_str, NULL, oParam);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
#endif
	FUNC_LEAVE;
	return;
}

void deal_request_thread()
{
	FUNC_ENTER;
	http_request->Initialize();
	printf("deal_request_thread begin\n");

	static int uniqs_ntest = 0;

	while (g_brequest_thread_working) {
		++uniqs_ntest;
		if (uniqs_ntest <= 2) printf("deal_request_thread loop\n");
		http_request->Update();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	printf("deal_request_thread end\n");
	FUNC_LEAVE;
	return;
}


