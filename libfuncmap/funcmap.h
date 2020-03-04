#pragma once

#include<stdint.h>
#include<map>
#include<vector>
#include<string>
#include<stdio.h>
#include<stdlib.h>


enum edyparam_type
{
	edyparam_type_int,
	edyparam_type_uint,
	edyparam_type_int64,
	edyparam_type_uint64,
	edyparam_type_string,
	edyparam_type_cstring,
};

class dyParam
{
	public:
		edyparam_type type;

		int valint;
		unsigned valuint;
		int64_t valint64;
		uint64_t valuint64;
		std::string valstr;

		operator int() const
		{
			if(type == edyparam_type_int) return valint;
			if(type == edyparam_type_uint) return (int)valuint;
			if(type == edyparam_type_string) return atoi(valstr.c_str());
			if(type == edyparam_type_cstring) return atoi(valstr.c_str());
			return 0;
		}
		operator unsigned int() const
		{
			if(type == edyparam_type_int) return (unsigned)valint;
			if(type == edyparam_type_uint) return valuint;
			if(type == edyparam_type_string) return (unsigned int)atoi(valstr.c_str());
			if(type == edyparam_type_cstring) return (unsigned int)atoi(valstr.c_str());
			return 0;
		}
		operator int64_t() const
		{
			if(type == edyparam_type_int) return (int64_t)valint;
			if(type == edyparam_type_uint) return (int64_t)valuint;
			if(type == edyparam_type_int64) return (int64_t)valint64;
			if(type == edyparam_type_uint64) return (int64_t)valuint64;
			if(type == edyparam_type_string) return (int64_t)atoll(valstr.c_str());
			if(type == edyparam_type_cstring) return (int64_t)atoll(valstr.c_str());
			return 0;
		}
		operator uint64_t() const
		{
			if(type == edyparam_type_int) return (uint64_t)valint;
			if(type == edyparam_type_uint) return (uint64_t)valuint;
			if(type == edyparam_type_int64) return (uint64_t)valint64;
			if(type == edyparam_type_uint64) return (uint64_t)valuint64;
			if(type == edyparam_type_string) return (uint64_t)atoll(valstr.c_str());
			if(type == edyparam_type_cstring) return (uint64_t)atoll(valstr.c_str());
			return 0;
		}
};

typedef std::vector<dyParam> FUNCMAP_PARAMS;

static void printParam(const dyParam& param)
{
	if(param.type == edyparam_type_int) printf("type:%d val:[%d]", param.type, param.valint);
	if(param.type == edyparam_type_uint) printf("type:%d val:[%u]", param.type, param.valuint);
	if(param.type == edyparam_type_int64) printf("type:%d val:[%lld]", param.type, (long long int)param.valint64);
	if(param.type == edyparam_type_uint64) printf("type:%d val:[%llu]", param.type, (long long unsigned int)param.valuint64);
	if(param.type == edyparam_type_string) printf("type:%d val:[%s]", param.type, param.valstr.c_str());
	if(param.type == edyparam_type_cstring) printf("type:%d val:[%s]", param.type, param.valstr.c_str());
}

static void printParams(const FUNCMAP_PARAMS& params)
{
	printf("params:");
	for(FUNCMAP_PARAMS::const_iterator it = params.begin();it != params.end();++it)
		printParam(*it);
	printf("\n");
}

template<class K, class T>
class funcMap
{
	typedef bool(T::*FUNC)(const FUNCMAP_PARAMS&, std::string& strResult);
	typedef std::map<K, std::pair<T*, FUNC> > MAPFUNCS;
	MAPFUNCS m_mapFuncs;
	public:
	funcMap(){}
	~funcMap(){m_mapFuncs.clear();}

	bool addFunc(const K& key, T* pClass, FUNC f)
	{
		typename funcMap<K, T>::MAPFUNCS::iterator it = m_mapFuncs.find(key);
		if(it != m_mapFuncs.end()){return false;}

		m_mapFuncs[key] = std::make_pair(pClass, f);

		return true;
	}

#define DOFUNC_BEGIN \
	typename funcMap<K, T>::MAPFUNCS::iterator it = m_mapFuncs.find(key);\
	if(it == m_mapFuncs.end()){strResult = "function not registered.";return false;}

#define DOFUNC_END return (it->second.first->*it->second.second)(params, strResult);

	bool doFunc(const K& key, std::string& strResult)
	{
		DOFUNC_BEGIN;

		FUNCMAP_PARAMS params;

		DOFUNC_END;
	}

	template<typename T1>
		bool doFunc(const K& key, std::string& strResult, T1 t1)
		{
			DOFUNC_BEGIN;

			FUNCMAP_PARAMS params;
			if(!_trimParam(params, t1)){strResult = "param1 error."; return false;}

			DOFUNC_END;
		}

	template<typename T1, typename T2>
		bool doFunc(const K& key, std::string& strResult, T1 t1, T2 t2)
		{
			DOFUNC_BEGIN;

			FUNCMAP_PARAMS params;
			if(!_trimParam(params, t1)){strResult = "param1 error."; return false;}
			if(!_trimParam(params, t2)){strResult = "param2 error."; return false;}

			DOFUNC_END;
		}

	template<typename T1, typename T2, typename T3>
		bool doFunc(const K& key, std::string& strResult, T1 t1, T2 t2, T3 t3)
		{
			DOFUNC_BEGIN;

			FUNCMAP_PARAMS params;
			if(!_trimParam(params, t1)){strResult = "param1 error."; return false;}
			if(!_trimParam(params, t2)){strResult = "param2 error."; return false;}
			if(!_trimParam(params, t3)){strResult = "param3 error."; return false;}

			DOFUNC_END;
		}

	template<typename T1, typename T2, typename T3, typename T4>
		bool doFunc(const K& key, std::string& strResult, T1 t1, T2 t2, T3 t3, T4 t4)
		{
			DOFUNC_BEGIN;

			FUNCMAP_PARAMS params;
			if(!_trimParam(params, t1)){strResult = "param1 error."; return false;}
			if(!_trimParam(params, t2)){strResult = "param2 error."; return false;}
			if(!_trimParam(params, t3)){strResult = "param3 error."; return false;}
			if(!_trimParam(params, t4)){strResult = "param4 error."; return false;}

			DOFUNC_END;
		}

	template<typename T1, typename T2, typename T3, typename T4, typename T5>
		bool doFunc(const K& key, std::string& strResult, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5)
		{
			DOFUNC_BEGIN;

			FUNCMAP_PARAMS params;
			if(!_trimParam(params, t1)){strResult = "param1 error."; return false;}
			if(!_trimParam(params, t2)){strResult = "param2 error."; return false;}
			if(!_trimParam(params, t3)){strResult = "param3 error."; return false;}
			if(!_trimParam(params, t4)){strResult = "param4 error."; return false;}
			if(!_trimParam(params, t5)){strResult = "param5 error."; return false;}

			DOFUNC_END;
		}

	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
		bool doFunc(const K& key, std::string& strResult, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6)
		{
			DOFUNC_BEGIN;

			FUNCMAP_PARAMS params;
			if(!_trimParam(params, t1)){strResult = "param1 error."; return false;}
			if(!_trimParam(params, t2)){strResult = "param2 error."; return false;}
			if(!_trimParam(params, t3)){strResult = "param3 error."; return false;}
			if(!_trimParam(params, t4)){strResult = "param4 error."; return false;}
			if(!_trimParam(params, t5)){strResult = "param5 error."; return false;}
			if(!_trimParam(params, t6)){strResult = "param6 error."; return false;}

			DOFUNC_END;
		}

	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
		bool doFunc(const K& key, std::string& strResult, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7)
		{
			DOFUNC_BEGIN;

			FUNCMAP_PARAMS params;
			if(!_trimParam(params, t1)){strResult = "param1 error."; return false;}
			if(!_trimParam(params, t2)){strResult = "param2 error."; return false;}
			if(!_trimParam(params, t3)){strResult = "param3 error."; return false;}
			if(!_trimParam(params, t4)){strResult = "param4 error."; return false;}
			if(!_trimParam(params, t5)){strResult = "param5 error."; return false;}
			if(!_trimParam(params, t6)){strResult = "param6 error."; return false;}
			if(!_trimParam(params, t7)){strResult = "param7 error."; return false;}

			DOFUNC_END;
		}

	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
		bool doFunc(const K& key, std::string& strResult, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8)
		{
			DOFUNC_BEGIN;

			FUNCMAP_PARAMS params;
			if(!_trimParam(params, t1)){strResult = "param1 error."; return false;}
			if(!_trimParam(params, t2)){strResult = "param2 error."; return false;}
			if(!_trimParam(params, t3)){strResult = "param3 error."; return false;}
			if(!_trimParam(params, t4)){strResult = "param4 error."; return false;}
			if(!_trimParam(params, t5)){strResult = "param5 error."; return false;}
			if(!_trimParam(params, t6)){strResult = "param6 error."; return false;}
			if(!_trimParam(params, t7)){strResult = "param7 error."; return false;}
			if(!_trimParam(params, t8)){strResult = "param8 error."; return false;}

			DOFUNC_END;
		}

	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
		bool doFunc(const K& key, std::string& strResult, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9)
		{
			DOFUNC_BEGIN;

			FUNCMAP_PARAMS params;
			if(!_trimParam(params, t1)){strResult = "param1 error."; return false;}
			if(!_trimParam(params, t2)){strResult = "param2 error."; return false;}
			if(!_trimParam(params, t3)){strResult = "param3 error."; return false;}
			if(!_trimParam(params, t4)){strResult = "param4 error."; return false;}
			if(!_trimParam(params, t5)){strResult = "param5 error."; return false;}
			if(!_trimParam(params, t6)){strResult = "param6 error."; return false;}
			if(!_trimParam(params, t7)){strResult = "param7 error."; return false;}
			if(!_trimParam(params, t8)){strResult = "param8 error."; return false;}
			if(!_trimParam(params, t9)){strResult = "param9 error."; return false;}

			DOFUNC_END;
		}

	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
		bool doFunc(const K& key, std::string& strResult, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10)
		{
			DOFUNC_BEGIN;

			FUNCMAP_PARAMS params;
			if(!_trimParam(params, t1)){strResult = "param1 error."; return false;}
			if(!_trimParam(params, t2)){strResult = "param2 error."; return false;}
			if(!_trimParam(params, t3)){strResult = "param3 error."; return false;}
			if(!_trimParam(params, t4)){strResult = "param4 error."; return false;}
			if(!_trimParam(params, t5)){strResult = "param5 error."; return false;}
			if(!_trimParam(params, t6)){strResult = "param6 error."; return false;}
			if(!_trimParam(params, t7)){strResult = "param7 error."; return false;}
			if(!_trimParam(params, t8)){strResult = "param8 error."; return false;}
			if(!_trimParam(params, t9)){strResult = "param9 error."; return false;}
			if(!_trimParam(params, t10)){strResult = "param10 error."; return false;}

			DOFUNC_END;
		}

	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
		bool doFunc(const K& key, std::string& strResult,T1 t1,T2 t2,T3 t3,T4 t4,T5 t5,T6 t6,T7 t7,T8 t8,T9 t9,T10 t10,T11 t11)
		{
			DOFUNC_BEGIN;

			FUNCMAP_PARAMS params;
			if(!_trimParam(params, t1)){strResult = "param1 error."; return false;}
			if(!_trimParam(params, t2)){strResult = "param2 error."; return false;}
			if(!_trimParam(params, t3)){strResult = "param3 error."; return false;}
			if(!_trimParam(params, t4)){strResult = "param4 error."; return false;}
			if(!_trimParam(params, t5)){strResult = "param5 error."; return false;}
			if(!_trimParam(params, t6)){strResult = "param6 error."; return false;}
			if(!_trimParam(params, t7)){strResult = "param7 error."; return false;}
			if(!_trimParam(params, t8)){strResult = "param8 error."; return false;}
			if(!_trimParam(params, t9)){strResult = "param9 error."; return false;}
			if(!_trimParam(params, t10)){strResult = "param10 error."; return false;}
			if(!_trimParam(params, t11)){strResult = "param11 error."; return false;}

			DOFUNC_END;
		}

	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
		bool doFunc(const K& key, std::string& strResult,T1 t1,T2 t2,T3 t3,T4 t4,T5 t5,T6 t6,T7 t7,T8 t8,T9 t9,T10 t10,T11 t11,T12 t12)
		{
			DOFUNC_BEGIN;

			FUNCMAP_PARAMS params;
			if(!_trimParam(params, t1)){strResult = "param1 error."; return false;}
			if(!_trimParam(params, t2)){strResult = "param2 error."; return false;}
			if(!_trimParam(params, t3)){strResult = "param3 error."; return false;}
			if(!_trimParam(params, t4)){strResult = "param4 error."; return false;}
			if(!_trimParam(params, t5)){strResult = "param5 error."; return false;}
			if(!_trimParam(params, t6)){strResult = "param6 error."; return false;}
			if(!_trimParam(params, t7)){strResult = "param7 error."; return false;}
			if(!_trimParam(params, t8)){strResult = "param8 error."; return false;}
			if(!_trimParam(params, t9)){strResult = "param9 error."; return false;}
			if(!_trimParam(params, t10)){strResult = "param10 error."; return false;}
			if(!_trimParam(params, t11)){strResult = "param11 error."; return false;}
			if(!_trimParam(params, t12)){strResult = "param12 error."; return false;}

			DOFUNC_END;
		}

	bool doFunc_direct(const K& key, std::string& strResult,FUNCMAP_PARAMS params)
	{
		DOFUNC_BEGIN;
		DOFUNC_END;
	}

	void process_cmd(const std::string& strGMCommandFromChat, std::string& strResult)
	{
		std::string strTmp = strGMCommandFromChat;
		std::string::size_type pos = strGMCommandFromChat.find(" ");
		std::string strCmd;
		if (pos == std::string::npos)
		{
			strCmd = strGMCommandFromChat;
		}
		else
		{
			strCmd = strGMCommandFromChat.substr(0, pos);
			strTmp = strGMCommandFromChat.substr(pos + 1);
		}

		FUNCMAP_PARAMS params;

		while (true)
		{
			if (strTmp.empty())
			{
				break;
			}

			std::string::size_type pos = strTmp.find(" ");
			if (pos == std::string::npos)
			{
				dyParam p;
				p.type = edyparam_type_string;
				p.valstr = strTmp;
				params.push_back(p);
				break;
			}
			else
			{
				std::string strParam = strTmp.substr(0, pos);
				strTmp = strTmp.substr(pos + 1);
				{
					dyParam p;
					p.type = edyparam_type_string;
					p.valstr = strParam;
					params.push_back(p);
				}
			}
		}

		doFunc_direct(strCmd, strResult, params);
	}

	private:
	bool _trimParam(FUNCMAP_PARAMS& params, int t){dyParam param;param.type = edyparam_type_int; param.valint = t; params.push_back(param); return true;}
	bool _trimParam(FUNCMAP_PARAMS& params, unsigned t){dyParam param;param.type = edyparam_type_uint; param.valuint = t; params.push_back(param); return true;}
	bool _trimParam(FUNCMAP_PARAMS& params, int64_t t){dyParam param;param.type = edyparam_type_int64; param.valint64 = t; params.push_back(param); return true;}
	bool _trimParam(FUNCMAP_PARAMS& params, uint64_t t){dyParam param;param.type = edyparam_type_uint64; param.valuint64 = t; params.push_back(param); return true;}
	bool _trimParam(FUNCMAP_PARAMS& params, std::string t){dyParam param;param.type = edyparam_type_string; param.valstr = t; params.push_back(param); return true;}
	bool _trimParam(FUNCMAP_PARAMS& params, const char* t){dyParam param;param.type = edyparam_type_cstring; param.valstr = t; params.push_back(param); return true;}
};

