#pragma once

#include "common_def.hpp"
#include <list>
#include <curl/curl.h>
#include <mutex>

#define URL_LEN         1024
#define CONTENT_LEN     8192
#define RECVBUFF_LEN    8192

enum request_type_t
{
	em_get_request = 1,
	em_post_request,
};

struct request_t
{
	uint32_t userid;
	char url[URL_LEN + 1];
	char content[CONTENT_LEN + 1];
	request_type_t type;
};

struct recv_buffer_t
{
	char buffer[RECVBUFF_LEN];
	size_t size;
};

class RequestInfo
{
public:
	RequestInfo();
	~RequestInfo();

	CURL *get_handle()
	{
		return curl_handle;
	}
	void set_handle(CURL *handle)
	{
		curl_handle = handle;
	}

	recv_buffer_t *get_buff()
	{
		return &recv_buff;
	}

	uint32_t get_userid(){ return m_userid; }
	void set_userid(uint32_t uid){ m_userid = uid; }
private:
	CURL *curl_handle;
	recv_buffer_t recv_buff;
	uint32_t m_userid;
};

class HttpRequest
{
public:
	HttpRequest();
	~HttpRequest();

public:
	// bAsync表示 add_request 和 request_deal_loop 不在一个线程里。 不在一个线程里的时候，内部会加锁，性能会略低于在同一个线程里处理
	int add_request(uint32_t uid, request_type_t type, const char *url, const char *content, bool bAsync = true);

	// bAsync表示 add_request 和 request_deal_loop 不在一个线程里。 不在一个线程里的时候，内部会加锁，性能会略低于在同一个线程里处理
	void request_deal_loop(bool bAsync = true);

	virtual void on_request_done(RequestInfo* p_info);

protected:
	int pop_request_list(bool bAsync, int nMaxProcessPerFrame);
	int request_select();
	int request_done();

	int add_request_handle(RequestInfo *p_info);
	int add_get_request(const char* url, uint32_t uid);
	int add_post_request(const char *url, const char *post_content, uint32_t uid);

protected:
	CURLM* curl_m;
	int request_nums;
	std::list<request_t> request_list;
	std::mutex request_list_mutex;
};
