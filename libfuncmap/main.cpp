#include "funcmap.h"
#include <stdio.h>

// for test
#include <iostream>
using namespace std;

class Player
{
	funcMap<std::string, Player> m_funcmap_gmcommands;
	public:
	Player()
	{
		m_funcmap_gmcommands.addFunc("additem", this, &Player::gm_additem);
	}
	void Test()
	{
		std::string strResult;
		m_funcmap_gmcommands.addFunc("additem", this, &Player::gm_additem);
		m_funcmap_gmcommands.addFunc("BBB", this, &Player::BBB);

		// on msg
		std::string strTest = "additem 100001 1";
		// trim
		std::string str1 = "additem";
		std::string str2 = "100001";
		std::string str3 = "1";
		m_funcmap_gmcommands.doFunc(str1, strResult, str2, str3);
	}
	void Test2(const std::string& strGMCommandFromChat)
	{
		string strTmp = strGMCommandFromChat;
		string::size_type pos = strGMCommandFromChat.find(" ");
		string strCmd;
		if (pos == string::npos)
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

			string::size_type pos = strTmp.find(" ");
			if (pos == string::npos)
			{
				dyParam p;
				p.type = edyparam_type_string;
				p.valstr = strTmp;
				params.push_back(p);
				break;
			}
			else
			{
				string strParam = strTmp.substr(0, pos);
				strTmp = strTmp.substr(pos + 1);
			}
		}

		std::string strResult;
		m_funcmap_gmcommands.doFunc_direct(strCmd, strResult, params);
	}
	void Test3(const std::string& strGMCommandFromChat)
	{
		std::string strResult;
		m_funcmap_gmcommands.process_cmd(strGMCommandFromChat, strResult);
	}
	bool gm_additem(const FUNCMAP_PARAMS& params, std::string& strResult)
	{
		printParams(params);
		strResult = "AAA is OK.";
		additem(params[0], params[1]);
	}

	bool BBB(const FUNCMAP_PARAMS& params, std::string& strResult)
	{
		printParams(params);
		strResult = "BBB is OK.";
	}

	void additem(int itemid, int ncount)
	{
		cout<<"additem itemid:"<<itemid<<" ncount:"<<ncount<<endl;
	}
};

int main()
{
	Player oPlayer;
	//oPlayer.Test();
	//oPlayer.Test2("additem 200002 111 222 333 444");
	oPlayer.Test3("additem 300003 333");
	return 0;
}

