#pragma once

#include "common_def.hpp"
#include <list>
#include <curl/curl.h>

extern HttpRequest* http_request;

void do_test(HttpRequest* prequest, uint32_t uid_begin);
void deal_request_thread2();

uint64_t get_ms();
