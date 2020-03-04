#pragma once

#include<vector>
#include<string>
#include<list>
#include<map>
#include<stdint.h>

#include "curl/curl.h"
#include "log.h"

struct QUERY_PARAM_ON_HTTP_REQUEST
{
	uint64_t 		Param1;
	unsigned 		Param2;
	unsigned 		Param3;
	uint64_t 		Param4;
	unsigned 		Param5;
};

struct HTTP_HEADER_ITEM
{
	std::string 	Name;
	std::string	Content;
};

struct HTTP_HEADER_LIST
{
	std::vector<HTTP_HEADER_ITEM>Headers;
};

enum ERequestResult
{
	ERequestResult_Error = -1,
	ERequestResult_OK = 0,
	ERequestResult_Param = 1,
};

enum REQUEST_TYPE
{
	REQUEST_TYPE_POST,
	REQUEST_TYPE_POST_BIN,
	REQUEST_TYPE_GET,
};


class http_requester
{
protected:
	struct REQUEST_INFO
	{
		unsigned					RequestID;
		int							RequestType;
		std::string					RequestURL;
		std::string 				RequestContent;
		CURL *						CURLHandle;
		struct curl_slist *         Headers;
		std::string 				RecvBuffer;
		QUERY_PARAM_ON_HTTP_REQUEST	Param;
		REQUEST_INFO(unsigned id)
		{
			RequestID = id;
			RequestType = 0;
			CURLHandle = NULL;
			Headers = NULL;
			LogDebug("RequestInfo ctor this:%llu\n", (uint64_t)this);
		}
		~REQUEST_INFO()
		{
			if (Headers)
			{
				curl_slist_free_all(Headers);
			}
			if (CURLHandle)
			{
				curl_easy_cleanup(CURLHandle);
			}
			LogDebug("RequestInfo dtor this:%llu\n", (uint64_t)this);
		}
	};

	unsigned m_nextRequestID;
	std::map<unsigned, REQUEST_INFO*>	m_RequestPool;

	CURLM *						m_CURLMHandle;
	int							m_HaveRequestRunning;

	timeval						m_SelectTimeOut;
public:
	http_requester(void);
	~http_requester(void);
	bool Initialize();

	virtual int Update();

	virtual void GetPostAck(int result, REQUEST_TYPE type, const char* URL, const char* Content, const QUERY_PARAM_ON_HTTP_REQUEST& Param, const REQUEST_INFO* pRequestInfo);
public:
	virtual int PostRequest(const char* URL, const char* Content, const char* UserAgent, const QUERY_PARAM_ON_HTTP_REQUEST& Param);
	virtual int GetRequest(const char* URL, const char* UserAgent, const QUERY_PARAM_ON_HTTP_REQUEST& Param);
	virtual int PostRequestWithHeaders(const char* URL, const char* Content, const HTTP_HEADER_LIST& Headers, const QUERY_PARAM_ON_HTTP_REQUEST& Param);
	virtual int GetRequestWithHeaders(const char* URL, const HTTP_HEADER_LIST& Headers, const QUERY_PARAM_ON_HTTP_REQUEST& Param);
	virtual int PostBinRequest(const char* URL, const std::string& Content, const char* UserAgent, const QUERY_PARAM_ON_HTTP_REQUEST& Param);
protected:
	struct curl_slist *GetHeaderList(const HTTP_HEADER_LIST& Headers);
	bool AddPostRequest(const char* szURL, const void* pData, unsigned DataLen, const HTTP_HEADER_LIST& List, const QUERY_PARAM_ON_HTTP_REQUEST& Param);
	bool AddGetRequest(const char* szURL, const HTTP_HEADER_LIST& List, const QUERY_PARAM_ON_HTTP_REQUEST& Param);
	bool SendRequest(REQUEST_INFO * pRequestInfo);
	void ProcessRequestEnd(REQUEST_INFO * pRequestInfo, CURLcode Result, int ResponseCode);
	static size_t ReadCallback(void *ptr, size_t size, size_t nmemb, void *userp);
	int RunCURL();
	int FinishCURL();
};

