#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "http_request.hpp"

using namespace std;

size_t recv_data(void *content, size_t size, size_t nmemb, void *userp)
{
	FUNC_ENTER;
	size_t realsize = size * nmemb;

	recv_buffer_t *buff = (recv_buffer_t*)userp;
	if (!buff)
	{
		printf("recv_data buff is null\n");
		return 0;
	}

	//printf("buff:%llu buff->size:%u realsize:%u, size:%u, nmemb:%u\n", (uint64_t)buff, buff->size, realsize, size, nmemb);

	if (buff->size + realsize >= RECVBUFF_LEN) {
		ERROR_LOG("recv buff size max[%d]", buff->size + realsize);
		return 0;
	}

#if defined(WIN32)||defined(WINDOWS)||defined(WIN64)
	strncat_s(buff->buffer, (char*)content, realsize);
#else
	strncat(buff->buffer, (char*)content, realsize);
#endif

	buff->size += realsize;
	buff->buffer[buff->size] = '\0';
	FUNC_LEAVE;
	return realsize;
}

RequestInfo::RequestInfo()
{
	FUNC_ENTER;
	curl_handle = NULL;

	recv_buff.size = 1L;
	recv_buff.buffer[0] = '\0';
	m_userid = 0;
	FUNC_LEAVE;
}

RequestInfo::~RequestInfo()
{
	FUNC_ENTER;
	if (curl_handle)
		curl_easy_cleanup(curl_handle);
	curl_handle = NULL;

	recv_buff.size = 0;
	m_userid = 0;
	FUNC_LEAVE;
}

HttpRequest::HttpRequest()
{
	FUNC_ENTER;
	curl_m = curl_multi_init();
	request_nums = 0;
	FUNC_LEAVE;
}

HttpRequest::~HttpRequest()
{
	FUNC_ENTER;
	if (curl_m) {
		curl_multi_cleanup(curl_m);
	}
	FUNC_LEAVE;
}

int HttpRequest::add_request(uint32_t uid, request_type_t type, const char *url, const char *content, bool bAsync)
{
	FUNC_ENTER;
	request_t info;
	info.userid = uid;
	info.type = type;

	if (!url || strlen(url) >= URL_LEN)
	{
		if (url)
		{
			ERROR_LOG("%d url len error[%d]", uid, strlen(url));
		}
		else
		{
			ERROR_LOG("%d url is NULL ", uid);
		}

		return -1;
	}
	if (content && strlen(content) >= CONTENT_LEN)
	{
		ERROR_LOG("%d request content len error[%d]", uid, strlen(content));
		return -1;
	}

#if defined(WIN32)||defined(WINDOWS)||defined(WIN64)
	strcpy_s(info.url, url);
	if (content) {
		strcpy_s(info.content, content);
	}
#else
	strcpy(info.url, url);
	if (content) {
		strcpy(info.content, content);
	}
#endif
	if (bAsync)
	{
		std::lock_guard<std::mutex> lock(request_list_mutex);
		request_list.push_back(info);
	}
	else
	{
		request_list.push_back(info);
	}
	DEBUG_LOG("%u ADD REQUEST[type=%d url=%s]", uid, type, url);
	FUNC_LEAVE;
	return 0;
}

void HttpRequest::request_deal_loop(bool bAsync)
{
	FUNC_ENTER;
	pop_request_list(bAsync, 50);
	//while (get_request_nums)
	request_select();
	request_done();
	FUNC_LEAVE;
	return;
}


void HttpRequest::on_request_done(RequestInfo* p_info)
{

}

int HttpRequest::pop_request_list(bool bAsync, int nMaxProcessPerFrame)
{
	FUNC_ENTER;
	while (true)
	{
		request_t req;
		request_t& rInfo = req;
		if (bAsync)
		{
			std::lock_guard<std::mutex> lock(request_list_mutex);
			if (request_list.size() <= 0)
			{
				break;
			}
			req = request_list.front();
			rInfo = req;
		}
		else
		{
			if (request_list.size() <= 0)
			{
				break;
			}
			rInfo = request_list.front();
		}

		if (rInfo.type == em_get_request) {
			add_get_request(rInfo.url, rInfo.userid);
		}
		else if (rInfo.type == em_post_request) {
			add_post_request(rInfo.url, rInfo.content, rInfo.userid);
		}
		else {
			printf("HttpRequest::pop_request_list info.type[%d] unrecogonized!", rInfo.type);
		}

		if (!bAsync)
		{
			request_list.pop_front();
		}

		if (nMaxProcessPerFrame-- <= 0)
		{
			break;
		}
	}

	FUNC_LEAVE;
	return 0;
}

int HttpRequest::request_select()
{
	FUNC_ENTER;
	CURLMcode ret;
	int max_fd = -1;
	struct timeval timeout_tv;

	fd_set fd_read;
	fd_set fd_write;
	fd_set fd_except;
	FD_ZERO(&fd_read);
	FD_ZERO(&fd_write);
	FD_ZERO(&fd_except);

	timeout_tv.tv_sec = 1;
	timeout_tv.tv_usec = 0;

	ret = curl_multi_fdset(curl_m, &fd_read, &fd_write, &fd_except, &max_fd);
	if (ret != CURLM_OK) {
		printf("HttpRequest::request_select curl_multi_fdset error[%d]\n", ret);
		ERROR_LOG("curl multi fdset error[%d]", ret);
		return -1;
	}

	if (max_fd >= 0) {
		int rc = select(max_fd + 1, &fd_read, &fd_write, &fd_except, &timeout_tv);
		switch (rc) {
		case -1:
		{
			printf("HttpRequest::request_select select error[%d]\n", ret);
			return -1;
		}
		case 0:
		default:
			curl_multi_perform(curl_m, &request_nums);
			break;
		}
	}
	else {
		//important
		curl_multi_perform(curl_m, &request_nums);
	}

	FUNC_LEAVE;
	return 0;
}

int HttpRequest::request_done()
{
	FUNC_ENTER;
	struct CURLMsg *m;
	int msg_left;

	m = curl_multi_info_read(curl_m, &msg_left);
	if (m && m->msg == CURLMSG_DONE) {
		CURL *curl = m->easy_handle;
		RequestInfo *p_request = NULL;
		curl_easy_getinfo(curl, CURLINFO_PRIVATE, &p_request);
		curl_multi_remove_handle(curl_m, curl);
		if (p_request) {
			on_request_done(p_request);
			printf("REQUEST FINISH[%d][%s]", p_request->get_userid(), p_request->get_buff()->buffer);
			delete p_request;
		}
	}

	FUNC_LEAVE;
	return msg_left;
}

int HttpRequest::add_request_handle(RequestInfo *p_info)
{
	FUNC_ENTER;
	if (!p_info || !curl_m) return -1;

	CURLMcode ret;
	curl_multi_add_handle(curl_m, p_info->get_handle());
	ret = curl_multi_perform(curl_m, &request_nums);
	if (ret != CURLM_OK && ret != CURLM_CALL_MULTI_PERFORM) {
		ERROR_LOG("add request failed ret=%d!\n", ret);
	}

	FUNC_LEAVE;
	return 0;
}

int HttpRequest::add_get_request(const char* url, uint32_t uid)
{
	FUNC_ENTER;
	CURLcode ret;
	RequestInfo *p_info = new RequestInfo();
	p_info->set_userid(uid);

	p_info->set_handle(curl_easy_init());
	ret = curl_easy_setopt(p_info->get_handle(), CURLOPT_URL, url);
	if (ret != CURLE_OK) {
		ERROR_LOG("get request setopt error[%d]", ret);
		return ret;
	}
	ret = curl_easy_setopt(p_info->get_handle(), CURLOPT_HEADER, 0L);
	if (ret != CURLE_OK) {
		ERROR_LOG("get request setopt error[%d]", ret);
		return ret;
	}
	ret = curl_easy_setopt(p_info->get_handle(), CURLOPT_WRITEFUNCTION, recv_data);
	if (ret != CURLE_OK) {
		ERROR_LOG("get request setopt error[%d]", ret);
		return ret;
	}
	ret = curl_easy_setopt(p_info->get_handle(), CURLOPT_WRITEDATA, (void *)(p_info->get_buff()));
	if (ret != CURLE_OK) {
		ERROR_LOG("get request setopt error[%d]", ret);
		return ret;
	}
	ret = curl_easy_setopt(p_info->get_handle(), CURLOPT_PRIVATE, p_info);
	if (ret != CURLE_OK) {
		ERROR_LOG("get request setopt error[%d]", ret);
		return ret;
	}

	add_request_handle(p_info);
	FUNC_LEAVE;
	return ret;
}

int HttpRequest::add_post_request(const char *url, const char *post_content, uint32_t uid)
{
	FUNC_ENTER;
	CURLcode ret;
	RequestInfo *p_info = new RequestInfo();
	p_info->set_userid(uid);

	p_info->set_handle(curl_easy_init());
	ret = curl_easy_setopt(p_info->get_handle(), CURLOPT_URL, url);
	if (ret != CURLE_OK)
	{
		ERROR_LOG("post request setopt error[%d]", ret);
		return ret;
	}
	ret = curl_easy_setopt(p_info->get_handle(), CURLOPT_HEADER, 0L);
	if (ret != CURLE_OK)
	{
		ERROR_LOG("post request setopt error[%d]", ret);
		return ret;
	}
	ret = curl_easy_setopt(p_info->get_handle(), CURLOPT_CUSTOMREQUEST, "post");
	if (ret != CURLE_OK)
	{
		ERROR_LOG("post request setopt error[%d]", ret);
		return ret;
	}
	ret = curl_easy_setopt(p_info->get_handle(), CURLOPT_POST, 1L);
	if (ret != CURLE_OK)
	{
		ERROR_LOG("post request setopt error[%d]", ret);
		return ret;
	}
	ret = curl_easy_setopt(p_info->get_handle(), CURLOPT_POSTFIELDS, post_content);
	if (ret != CURLE_OK)
	{
		ERROR_LOG("post request setopt error[%d]", ret);
		return ret;
	}
	ret = curl_easy_setopt(p_info->get_handle(), CURLOPT_POSTFIELDSIZE, strlen(post_content));
	if (ret != CURLE_OK)
	{
		ERROR_LOG("post request setopt error[%d]", ret);
		return ret;
	}
	ret = curl_easy_setopt(p_info->get_handle(), CURLOPT_PRIVATE, p_info);
	if (ret != CURLE_OK)
	{
		ERROR_LOG("post request setopt error[%d]", ret);
		return ret;
}

	struct curl_slist *list = NULL;
	char cl_str[128] = { 0x00 };
#if defined(WIN32)||defined(WINDOWS)||defined(WIN64)
	sprintf_s(cl_str, "Content-Length:%ld", strlen(post_content));
#else
	sprintf(cl_str, "Content-Length:%ld", strlen(post_content));
#endif
	list = curl_slist_append(list, "Content-Type: application/json");
	list = curl_slist_append(list, cl_str);

	//printf("HttpRequest::add_post_request cl_str:%s\n", cl_str);
	//printf("HttpRequest::add_post_request post_content:%s\n", post_content);

	ret = curl_easy_setopt(p_info->get_handle(), CURLOPT_HTTPHEADER, list);
	if (ret != CURLE_OK)
	{
		ERROR_LOG("post request setopt error[%d]", ret);
		return ret;
	}

	ret = curl_easy_setopt(p_info->get_handle(), CURLOPT_WRITEFUNCTION, recv_data);
	if (ret != CURLE_OK)
	{
		ERROR_LOG("post request setopt error[%d]", ret);
		return ret;
	}
	ret = curl_easy_setopt(p_info->get_handle(), CURLOPT_WRITEDATA, (void *)p_info->get_buff());
	if (ret != CURLE_OK)
	{
		ERROR_LOG("post request setopt error[%d]", ret);
		return ret;
	}

	add_request_handle(p_info);
	FUNC_LEAVE;
	return ret;
}

