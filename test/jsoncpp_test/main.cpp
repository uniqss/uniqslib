#include "../../common_header/jsoncpp_helper.h"

#include <string.h>
#include <iostream>

int main()
{
	{
		Json::Value Root;
		Json::Reader Parser;
		const char* strJson = "{\"aa\":123, \"bb\":\"strtest\"}";
		std::cout<<"json read test. strJson:"<<strJson<<std::endl;
		if(Parser.parse(strJson, strJson + strlen(strJson), Root, false));
		{
			int nAA = 0;
			_TrimJsonValueInt(Root, "aa", nAA);
			std::string strBB;
			_TrimJsonValueString(Root, "bb", strBB);
			std::cout<<"nAA: "<<nAA<<" strBB:"<<strBB<<std::endl;
		}
	}

	{
		std::cout<<"json write test."<<std::endl;
		Json::Value Root;
		std::cout<<Root.toStyledString()<<std::endl;
		Root["aaa"] = 1234;
		std::cout<<Root.toStyledString()<<std::endl;
		Root["bbb"] = "5678";
		std::cout<<Root.toStyledString()<<std::endl;
		Json::Value RetObj;
		RetObj["time"] = 123.4567;
		RetObj["online_num"] = 8899;
		Root["ret_obj"].append(RetObj);
		Root["ret_obj"].append(RetObj);
		std::cout<<Root.toStyledString()<<std::endl;
		Root["ret_obj2"] = (RetObj);
		Json::Value intArray;
		for(int nIdx = 0;nIdx < 3;++nIdx)
		{
			intArray.append(nIdx);
		}
		Root["ttt3"] = intArray;
		std::cout<<Root.toStyledString()<<std::endl;
	}

	return 0;
}

