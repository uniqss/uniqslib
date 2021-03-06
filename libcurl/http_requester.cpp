#include "http_requester.h"
#include "log.h"

// stl
#include <string.h>
#include <thread>
#include <chrono>

#include "../common_header/debugfunc.h"


http_requester::http_requester(void)
{
	FUNC_ENTER;
	m_nextRequestID = 1;
	m_CURLMHandle = NULL;
	m_HaveRequestRunning = 0;
}

http_requester::~http_requester(void)
{
	FUNC_ENTER;
	for (auto it = m_RequestPool.begin(); it != m_RequestPool.end(); ++it)
	{
		if (it->second){ delete it->second; it->second = NULL; }
	}
	m_RequestPool.clear();
	if (m_CURLMHandle)
	{
		curl_multi_cleanup(m_CURLMHandle);
		m_CURLMHandle = NULL;
	}
	curl_global_cleanup();
}

bool http_requester::Initialize()
{
	FUNC_ENTER;
	if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK)
	{
		Log("http_requester::Init curl_global_init failed.");
		FUNC_LEAVE;
		return false;
	}

	m_CURLMHandle = curl_multi_init();
	if (m_CURLMHandle == NULL)
	{
		Log("http_requester::Init curl_multi_init failed.");
		FUNC_LEAVE;
		return false;
	}

	long TimeOut = -1;
	int Result = curl_multi_timeout(m_CURLMHandle, &TimeOut);
	if (TimeOut >= 0)
	{
		m_SelectTimeOut.tv_sec = TimeOut / 1000;
		if (m_SelectTimeOut.tv_sec > 1)
			m_SelectTimeOut.tv_sec = 1;
		else
			m_SelectTimeOut.tv_usec = (TimeOut % 1000) * 1000;
	}
	else
	{
		m_SelectTimeOut.tv_sec = 0;
		m_SelectTimeOut.tv_usec = 0;
	}
	m_HaveRequestRunning = 0;

	Log("http_requester init ok.\n");
	FUNC_LEAVE;
	return true;
}

int http_requester::Update()
{
	int ProcessCount = 0;

	if (m_HaveRequestRunning)
	{
		ProcessCount += RunCURL();
	}
	if (m_RequestPool.size() > 0)
	{
		ProcessCount += FinishCURL();
	}

	return ProcessCount;
}

void http_requester::GetPostAck(int result, REQUEST_TYPE tpye, const char* URL, const char* Content, const QUERY_PARAM_ON_HTTP_REQUEST& Param, const REQUEST_INFO* pRequestInfo)
{
}

int http_requester::PostRequest(const char* URL, const char* Content, const char* UserAgent, const QUERY_PARAM_ON_HTTP_REQUEST& Param)
{
	FUNC_ENTER;
	HTTP_HEADER_LIST Headers;
	if (UserAgent != NULL && UserAgent[0] != '\0') {
		HTTP_HEADER_ITEM oTmp;
		Headers.Headers.push_back(oTmp);
		HTTP_HEADER_ITEM *pItem = &Headers.Headers[Headers.Headers.size() - 1];
		pItem->Name = "User-Agent";
		pItem->Content = UserAgent;
	}
	if (!AddPostRequest(URL, Content, 0, Headers, Param))
	{
		GetPostAck(ERequestResult_Error, REQUEST_TYPE_POST, URL, Content, Param, NULL);
		printf("http_requester::PostRequest AddPostRequest failed. url:%s content:%s\n", URL, Content);
		FUNC_LEAVE;
		return -1;
	}
	FUNC_LEAVE;
	return 0;
}

int http_requester::GetRequest(const char* URL, const char* UserAgent, const QUERY_PARAM_ON_HTTP_REQUEST& Param)
{
	FUNC_ENTER;
	//http_requesterAckMsgCaller MsgCaller(m_pOperator,m_pOperator->GetObjectID(),GetCurMsgSenderID());
	HTTP_HEADER_LIST Headers;
	if (UserAgent != NULL && UserAgent[0] != '\0') {
		HTTP_HEADER_ITEM oTmp;
		Headers.Headers.push_back(oTmp);
		HTTP_HEADER_ITEM *pItem = &Headers.Headers[Headers.Headers.size() - 1];
		pItem->Name = "User-Agent";
		pItem->Content = UserAgent;
	}
	if (!AddGetRequest(URL, Headers, Param))
	{
		GetPostAck(ERequestResult_Error, REQUEST_TYPE_GET, URL, NULL, Param, NULL);
		printf("http_requester::GetRequest AddGetRequest failed. url:%s \n", URL);
		FUNC_LEAVE;
		return -1;
	}
	FUNC_LEAVE;
	return 0;
}

int http_requester::PostRequestWithHeaders(const char* URL, const char* Content, const HTTP_HEADER_LIST& Headers, const QUERY_PARAM_ON_HTTP_REQUEST& Param)
{
	FUNC_ENTER;
	//http_requesterAckMsgCaller MsgCaller(m_pOperator,m_pOperator->GetObjectID(),GetCurMsgSenderID());
	if (!AddPostRequest(URL, Content, 0, Headers, Param))
	{
		GetPostAck(ERequestResult_Error, REQUEST_TYPE_POST, URL, Content, Param, NULL);
		FUNC_LEAVE;
		return -1;
	}
	FUNC_LEAVE;
	return 0;
}

int http_requester::GetRequestWithHeaders(const char* URL, const HTTP_HEADER_LIST& Headers, const QUERY_PARAM_ON_HTTP_REQUEST& Param)
{
	FUNC_ENTER;
	//http_requesterAckMsgCaller MsgCaller(m_pOperator,m_pOperator->GetObjectID(),GetCurMsgSenderID());
	if (!AddGetRequest(URL, Headers, Param))
	{
		GetPostAck(ERequestResult_Error, REQUEST_TYPE_GET, URL, NULL, Param, NULL);
		FUNC_LEAVE;
		return -1;
	}
	FUNC_LEAVE;
	return 0;
}

int http_requester::PostBinRequest(const char* URL, const std::string& Content, const char* UserAgent, const QUERY_PARAM_ON_HTTP_REQUEST& Param)
{
	FUNC_ENTER;
	//http_requesterAckMsgCaller MsgCaller(m_pOperator,m_pOperator->GetObjectID(),GetCurMsgSenderID());
	HTTP_HEADER_LIST Headers;
	if (UserAgent[0] != '\0') {
		HTTP_HEADER_ITEM oTmp;
		Headers.Headers.push_back(oTmp);
		HTTP_HEADER_ITEM *pItem = &Headers.Headers[Headers.Headers.size() - 1];
		pItem->Name = "User-Agent";
		pItem->Content = UserAgent;
	}
	if (!AddPostRequest(URL, Content.c_str(), (unsigned)Content.size(), Headers, Param))
	{
		GetPostAck(ERequestResult_Error, REQUEST_TYPE_POST_BIN, URL, Content.c_str(), Param, NULL);
		FUNC_LEAVE;
		return -1;
	}
	FUNC_LEAVE;
	return 0;
}

struct curl_slist * http_requester::GetHeaderList(const HTTP_HEADER_LIST& Headers)
{
	FUNC_ENTER;
	struct curl_slist *headers = NULL;
	const std::vector<HTTP_HEADER_ITEM>& List = Headers.Headers;
	for (unsigned i = 0; i < List.size(); ++i) {
		const HTTP_HEADER_ITEM& Item = List[i];
		std::string Header = Item.Name + ":" + Item.Content;
		struct curl_slist *newheaders = curl_slist_append(headers, (const char*)Header.c_str());
		if (newheaders == NULL) {
			curl_slist_free_all(headers);
			Log("AddPostRequest::SendRequest: curl_slist_append failed. Header:%s", (const char*)Header.c_str());
			FUNC_LEAVE;
			return NULL;
		}
		headers = newheaders;
	}
	FUNC_LEAVE;
	return headers;
}

bool http_requester::AddPostRequest(const char* szURL, const void* pData, unsigned DataLen, const HTTP_HEADER_LIST& Headers, const QUERY_PARAM_ON_HTTP_REQUEST& Param)
{
	FUNC_ENTER;
	++m_nextRequestID;// not thread safe.

	REQUEST_INFO* pRequestInfo = new REQUEST_INFO(m_nextRequestID);
	if (pRequestInfo)
	{
		m_RequestPool[m_nextRequestID] = pRequestInfo;
		pRequestInfo->RequestURL = szURL;
		if (DataLen == 0)
		{
			DataLen = (unsigned)strlen((char *)pData);
			pRequestInfo->RequestType = REQUEST_TYPE_POST;
		}
		else
		{
			pRequestInfo->RequestType = REQUEST_TYPE_POST_BIN;
		}
		pRequestInfo->RequestContent.append((const char*)pData, DataLen);
		pRequestInfo->Headers = GetHeaderList(Headers);
		pRequestInfo->Param = Param;
		if (pRequestInfo->CURLHandle)
		{
			curl_easy_cleanup(pRequestInfo->CURLHandle);
			pRequestInfo->CURLHandle = NULL;
		}
		pRequestInfo->CURLHandle = curl_easy_init();
		if (pRequestInfo->CURLHandle == NULL)
		{
			auto it = m_RequestPool.find(pRequestInfo->RequestID);
			if (it != m_RequestPool.end())
			{
				if (it->second){ delete it->second; it->second = NULL; }
				m_RequestPool.erase(it);
			}
			Log("AddPostRequest::Init:curl_easy_init failed.");
			FUNC_LEAVE;
			return false;
		}

		if (!SendRequest(pRequestInfo))
		{
			printf("http_requester::AddPostRequest SendRequest failed.");
			auto it = m_RequestPool.find(pRequestInfo->RequestID);
			if (it != m_RequestPool.end())
			{
				if (it->second){ delete it->second; it->second = NULL; }
				m_RequestPool.erase(it);
			}
			FUNC_LEAVE;
			return false;
		}

		FUNC_LEAVE;
		return true;
	}
	else
	{
		Log("AddPostRequest::Init: memory alloc failed() currsize:%d\n", m_RequestPool.size());
	}

	FUNC_LEAVE;
	return false;
}

bool http_requester::AddGetRequest(const char* szURL, const HTTP_HEADER_LIST& Headers, const QUERY_PARAM_ON_HTTP_REQUEST& Param)
{
	FUNC_ENTER;
	++m_nextRequestID;// not thread safe.

	REQUEST_INFO* pRequestInfo = new REQUEST_INFO(m_nextRequestID);
	if (pRequestInfo)
	{
		m_RequestPool[m_nextRequestID] = pRequestInfo;
		pRequestInfo->RequestURL = szURL;
		pRequestInfo->RequestType = REQUEST_TYPE_GET;
		pRequestInfo->Headers = GetHeaderList(Headers);
		pRequestInfo->Param = Param;
		if (pRequestInfo->CURLHandle)
		{
			curl_easy_cleanup(pRequestInfo->CURLHandle);
			pRequestInfo->CURLHandle = NULL;
		}
		pRequestInfo->CURLHandle = curl_easy_init();
		if (pRequestInfo->CURLHandle == NULL)
		{
			auto it = m_RequestPool.find(pRequestInfo->RequestID);
			if (it != m_RequestPool.end())
			{
				if (it->second){ delete it->second; it->second = NULL; }
				m_RequestPool.erase(it);
			}
			Log("AddGetRequest::Init:curl_easy_init failed.");
			FUNC_LEAVE;
			return false;
		}

		if (!SendRequest(pRequestInfo))
		{
			Log("http_requester::AddGetRequest SendRequest failed.");
			auto it = m_RequestPool.find(pRequestInfo->RequestID);
			if (it != m_RequestPool.end())
			{
				if (it->second){ delete it->second; it->second = NULL; }
				m_RequestPool.erase(it);
			}
			FUNC_LEAVE;
			return false;
		}

		FUNC_LEAVE;
		return true;
	}
	else
	{
		Log("AddPostRequest::Init: memory alloc failed() currsize:%d\n", m_RequestPool.size());
	}

	FUNC_LEAVE;
	return false;
}

bool http_requester::SendRequest(REQUEST_INFO * pRequestInfo)
{
	// 高并发时如果不sleep一下，会segment fault(core dump)
	std::this_thread::sleep_for(std::chrono::milliseconds(20));
	FUNC_ENTER;
	int Result;
	Result = curl_easy_setopt(pRequestInfo->CURLHandle, CURLOPT_URL, pRequestInfo->RequestURL.c_str());
	if (Result != CURLE_OK)
	{
		Log("AddPostRequest::SendRequest:设置URL失败%d", Result);
		FUNC_LEAVE;
		return false;
	}
	if (pRequestInfo->RequestType == REQUEST_TYPE_POST || pRequestInfo->RequestType == REQUEST_TYPE_POST_BIN)
	{
		Result = curl_easy_setopt(pRequestInfo->CURLHandle, CURLOPT_POSTFIELDS, pRequestInfo->RequestContent.c_str());
		if (Result != CURLE_OK)
		{
			Log("AddPostRequest::SendRequest:设置Post内容失败%d", Result);
			FUNC_LEAVE;
			return false;
		}
		Result = curl_easy_setopt(pRequestInfo->CURLHandle, CURLOPT_POSTFIELDSIZE, pRequestInfo->RequestContent.size());
		if (Result != CURLE_OK)
		{
			Log("AddPostRequest::SendRequest:设置Post内容长度失败%d", Result);
			FUNC_LEAVE;
			return false;
		}
		Result = curl_easy_setopt(pRequestInfo->CURLHandle, CURLOPT_POST, 1L);
		if (Result != CURLE_OK)
		{
			Log("AddPostRequest::SendRequest:设置Post开关失败%d", Result);
			FUNC_LEAVE;
			return false;
		}
	}
	Result = curl_easy_setopt(pRequestInfo->CURLHandle, CURLOPT_WRITEFUNCTION, http_requester::ReadCallback);
	if (Result != CURLE_OK)
	{
		Log("AddPostRequest::SendRequest:设置回调函数失败%d", Result);
		FUNC_LEAVE;
		return false;
	}
	Result = curl_easy_setopt(pRequestInfo->CURLHandle, CURLOPT_WRITEDATA, pRequestInfo);
	if (Result != CURLE_OK)
	{
		Log("AddPostRequest::SendRequest:设置回调参数失败%d", Result);
		FUNC_LEAVE;
		return false;
	}
	LogDebug("AddPostRequest::SendRequest curl_easy_setopt CURLOPT_PRIVATE pRequestInfo:%llu, CURLHandle:%llu\n", (uint64_t)pRequestInfo, (uint64_t)pRequestInfo->CURLHandle);
	Result = curl_easy_setopt(pRequestInfo->CURLHandle, CURLOPT_PRIVATE, pRequestInfo);
	if (Result != CURLE_OK)
	{
		Log("AddPostRequest::SendRequest:设置参数失败%d", Result);
		FUNC_LEAVE;
		return false;
	}
	Result = curl_easy_setopt(pRequestInfo->CURLHandle, CURLOPT_SSL_VERIFYPEER, 0);
	if (Result != CURLE_OK)
	{
		Log("AddPostRequest::SendRequest:设置SSL参数失败%d", Result);
		FUNC_LEAVE;
		return false;
	}
	if (pRequestInfo->Headers != NULL) {
		Result = curl_easy_setopt(pRequestInfo->CURLHandle, CURLOPT_HTTPHEADER, pRequestInfo->Headers);
		if (Result != CURLE_OK)
		{
			Log("AddPostRequest::SendRequest:设置Headers失败%d", Result);
			FUNC_LEAVE;
			return false;
		}
	}
	int MResult;
	MResult = curl_multi_add_handle(m_CURLMHandle, pRequestInfo->CURLHandle);
	if (MResult != CURLM_OK)
	{
		Log("AddPostRequest::SendRequest:添加请求失败%d", MResult);
		FUNC_LEAVE;
		return false;
	}
	else
	{
		//Log("AddPostRequest::SendRequest:添加请求成功%d",MResult);
	}
	MResult = curl_multi_perform(m_CURLMHandle, &m_HaveRequestRunning);
	if (MResult != CURLM_OK&&MResult != CURLM_CALL_MULTI_PERFORM)
	{
		Log("AddPostRequest::SendRequest:处理请求失败%d", MResult);
		FUNC_LEAVE;
		return false;
	}
	switch (pRequestInfo->RequestType)
	{
	case REQUEST_TYPE_POST:
	case REQUEST_TYPE_POST_BIN:
		LogDebug("http_requester::SendRequest:(%u)RequestURL=%s  RequestContent=%s",
			pRequestInfo->RequestID, pRequestInfo->RequestURL.c_str(), pRequestInfo->RequestContent.c_str());
		break;
	default:
		LogDebug("http_requester::SendRequest:(%u)RequestURL=%s",
			pRequestInfo->RequestID, pRequestInfo->RequestURL.c_str());
		break;
	}

	FUNC_LEAVE;
	return true;
}

void http_requester::ProcessRequestEnd(REQUEST_INFO * pRequestInfo, CURLcode Result, int ResponseCode)
{
	FUNC_ENTER;
	Log("%d:%d ", pRequestInfo->RequestID, ResponseCode);
	LogDebug("http_requester::ProcessRequestEnd:Result=%s,ResponseCode=%d",
		curl_easy_strerror(Result), ResponseCode);

	if (Result == CURLE_OK&&ResponseCode == 200)
	{
		if (pRequestInfo->RecvBuffer.size())
		{
			pRequestInfo->RecvBuffer = "";

			LogDebug("http_requester::ProcessRequestEnd:(%u)ReturnContent=%s",
				pRequestInfo->RequestID, pRequestInfo->RecvBuffer.c_str());
		}
		switch (pRequestInfo->RequestType)
		{
		case REQUEST_TYPE_POST:
			//MsgCaller.PostRequestAck(0,(const char*)pRequestInfo->RecvBuffer.GetBuffer(),pRequestInfo->Param);		
			break;
		case REQUEST_TYPE_POST_BIN:
			//MsgCaller.PostBinRequestAck(0,pRequestInfo->RecvBuffer,pRequestInfo->Param);		
			break;
		case REQUEST_TYPE_GET:
			//MsgCaller.GetRequestAck(0,(const char*)pRequestInfo->RecvBuffer.GetBuffer(),pRequestInfo->Param);
			break;
		}
	}
	else
	{
		switch (pRequestInfo->RequestType)
		{
		case REQUEST_TYPE_POST:
			//MsgCaller.PostRequestAck(COMMON_RESULT_FAILED,"",pRequestInfo->Param);		
			break;
		case REQUEST_TYPE_POST_BIN:
			//MsgCaller.PostBinRequestAck(COMMON_RESULT_FAILED,CEasyBuffer(),pRequestInfo->Param);		
			break;
		case REQUEST_TYPE_GET:
			//MsgCaller.GetRequestAck(COMMON_RESULT_FAILED,"",pRequestInfo->Param);
			break;
		}
	}
	// done in dtor.
	//curl_easy_cleanup(pRequestInfo->CURLHandle);
	//pRequestInfo->CURLHandle=NULL;
	auto it = m_RequestPool.find(pRequestInfo->RequestID);
	if (it != m_RequestPool.end())
	{
		if (it->second){ delete it->second; it->second = NULL; }
		m_RequestPool.erase(it);
	}
}
size_t http_requester::ReadCallback(void *ptr, size_t size, size_t nmemb, void *userp)
{
	FUNC_ENTER;
	REQUEST_INFO * pRequestInfo = (REQUEST_INFO *)userp;

	size = size*nmemb;

	if (pRequestInfo)
	{
		pRequestInfo->RecvBuffer.append((const char*)ptr, (unsigned)size);
		FUNC_LEAVE;
		return size;
	}
	return size;
}

int http_requester::RunCURL()
{
	int ProcessCount = 0;

	fd_set fdread;
	fd_set fdwrite;
	fd_set fdexcep;
	int maxfd = -1;
	int rc;

	FD_ZERO(&fdread);
	FD_ZERO(&fdwrite);
	FD_ZERO(&fdexcep);

	/* get file descriptors from the transfers */
	int Result = curl_multi_fdset(m_CURLMHandle, &fdread, &fdwrite, &fdexcep, &maxfd);

	/* In a real-world program you OF COURSE check the return code of the
	   function calls.  On success, the value of maxfd is guaranteed to be
	   greater or equal than -1.  We call select(maxfd + 1, ...), specially in
	   case of (maxfd == -1), we call select(0, ...), which is basically equal
	   to sleep. */
	if (maxfd >= 0)
	{
		rc = select(maxfd + 1, &fdread, &fdwrite, &fdexcep, &m_SelectTimeOut);

		switch (rc)
		{
		case -1:
			/* select error */
		{
#if (defined(WIN32)||defined(WINDOWS)||defined(WIN64))
			int ErrCode = WSAGetLastError();
#else
			int ErrCode = errno;
#endif
			Log("http_requester::RunCURL:SelectError=%d", ErrCode);
		}
		break;
		case 0:
		default:
			/* timeout or readable/writable sockets */
			curl_multi_perform(m_CURLMHandle, &m_HaveRequestRunning);
			ProcessCount++;
			break;
		}
	}
	else
	{
		curl_multi_perform(m_CURLMHandle, &m_HaveRequestRunning);
		ProcessCount++;
	}
	return ProcessCount;
}
int http_requester::FinishCURL()
{
	int ProcessCount = 0;
	int MsgLeft = 0;
	CURLMsg * pMsg = curl_multi_info_read(m_CURLMHandle, &MsgLeft);
	if (pMsg)
	{
		if (pMsg->msg == CURLMSG_DONE)
		{
			REQUEST_INFO * pRequestInfo = NULL;
			int Result = curl_easy_getinfo(pMsg->easy_handle, CURLINFO_PRIVATE, &pRequestInfo);
			LogDebug("http_requester::FinishCURL curl_easy_getinfo CURLOPT_PRIVATE pRequestInfo:%llu, CURLHandle:%llu\n", (uint64_t)pRequestInfo, (uint64_t)pMsg->easy_handle);
			if (Result == CURLE_OK)
			{
				long ResponseCode = 0;
				LogDebug("http_requester::FinishCURL 111 pRequestInfo:%llu, CURLHandle:%llu\n", (uint64_t)pRequestInfo, (uint64_t)pMsg->easy_handle);
				Result = curl_easy_getinfo(pMsg->easy_handle, CURLINFO_RESPONSE_CODE, &ResponseCode);
				LogDebug("http_requester::FinishCURL 222 pRequestInfo:%llu, CURLHandle:%llu\n", (uint64_t)pRequestInfo, (uint64_t)pMsg->easy_handle);
				if (Result == CURLE_OK)
				{
					if (pRequestInfo)
					{
						ProcessRequestEnd(pRequestInfo, pMsg->data.result, ResponseCode);
						ProcessCount++;
					}
					else
					{
						Log("http_requester::FinishCURL pRequestInfo is NULL ResponseCode:%d\n", ResponseCode);
					}
				}
				else
				{
					Log("http_requester::FinishCURL curl_easy_getinfo failed(param 2). result:%d\n", Result);
				}
			}
			else
			{
				Log("http_requester::FinishCURL curl_easy_getinfo failed(param 1). result:%d\n", Result);
			}
		}
	}
	return ProcessCount;
}
