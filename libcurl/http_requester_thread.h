#pragma once

#include <atomic>

class http_requester;
extern http_requester* http_request;

extern std::atomic<bool> g_brequest_thread_working;


void DoTest(http_requester* prequester, uint32_t uid);
void deal_request_thread();

