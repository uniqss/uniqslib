#pragma once

#if( defined(WIN32) || defined(WIN64) )
#include "jsoncpp/include/json/json.h"
#else
#include "jsoncpp/json/json.h"
#endif
#include <stdlib.h>
#include <stdint.h>

#include <string>

static bool _TrimJsonValueString(Json::Value &Root, const char* pszNodeName, std::string& strOut)
{
	if (Root.isMember(pszNodeName) && !Root[pszNodeName].isNull() && Root[pszNodeName].isString())
	{
		strOut = Root[pszNodeName].asString();
		return true;
	}

	return false;
}

static bool _TrimJsonValueInt(Json::Value &Root, const char* pszNodeName, int& nOut, bool bFromString = false)
{
	if (bFromString)
	{
		std::string strTmp;
		if (!_TrimJsonValueString(Root, pszNodeName, strTmp)) return false;
		nOut = atoi(strTmp.c_str());
		return true;
	}

	if (Root.isMember(pszNodeName) && !Root[pszNodeName].isNull() && Root[pszNodeName].isInt())
	{
		nOut = Root[pszNodeName].asInt();
		return true;
	}

	return false;
}

static bool _TrimJsonValueUint(Json::Value &Root, const char* pszNodeName, unsigned int& nOut, bool bFromString = false)
{
	if (bFromString)
	{
		std::string strTmp;
		if (!_TrimJsonValueString(Root, pszNodeName, strTmp)) return false;
		nOut = atoi(strTmp.c_str());
		return true;
	}

	if (Root.isMember(pszNodeName) && !Root[pszNodeName].isNull() && Root[pszNodeName].isInt())
	{
		nOut = Root[pszNodeName].asInt();
		return true;
	}

	return false;
}

static bool _TrimJsonValueInt64(Json::Value &Root, const char* pszNodeName, int64_t& nOut)
{
	std::string strTmp;
	if (!_TrimJsonValueString(Root, pszNodeName, strTmp)) return false;
	nOut = atoll(strTmp.c_str());
	return true;
}

static bool _TrimJsonValueUint64(Json::Value &Root, const char* pszNodeName, uint64_t& nOut)
{
	std::string strTmp;
	if (!_TrimJsonValueString(Root, pszNodeName, strTmp)) return false;
	nOut = atoll(strTmp.c_str());
	return true;
}

static bool _TrimJsonValueFloat(Json::Value &Root, const char* pszNodeName, float& fOut)
{
	if (Root.isMember(pszNodeName) && !Root[pszNodeName].isNull() && Root[pszNodeName].isInt())
	{
		fOut = Root[pszNodeName].asFloat();
		return true;
	}

	return false;
}

static bool _TrimJsonValueDouble(Json::Value &Root, const char* pszNodeName, double& dOut)
{
	if (Root.isMember(pszNodeName) && !Root[pszNodeName].isNull() && Root[pszNodeName].isInt())
	{
		dOut = Root[pszNodeName].asDouble();
		return true;
	}

	return false;
}
