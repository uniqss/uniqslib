#include "utimer.h"
using namespace UNIQS;

#include<stdio.h>
#include<time.h>

#include <iostream>
#include <thread>
#include <string>
#include <memory>
#include <chrono>
using namespace std;

std::unique_ptr<UTimer> g_pTimer1;
std::unique_ptr<UTimer> g_pTimer2;

#define TIMERID_1 1
#define TIMERID_2 2


#if (defined(WIN32) || defined(WIN64))
#include <windows.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif

void pt(const std::string& strPrefix)
{
	time_t nowtime;    
	struct tm *timeinfo;    
	time( &nowtime );    
	timeinfo = localtime( &nowtime );    

	int xtn = timeinfo->tm_year + 1900;    
	int xty = timeinfo->tm_mon + 1;    
	int xtr = timeinfo->tm_mday;    
	int xts = timeinfo->tm_hour;  
	int xtf = timeinfo->tm_min;  
	int xtm = timeinfo->tm_sec;  

	cout << strPrefix << " " << xtn << "-" << xty << "-" << xtr << " " << xts << ":" << xtf << ":" << xtm << endl;
}

#define pf cout << __FILE__ << __LINE__ << endl;

class TimerTest1 : public IUTimer
{
	public:
		~TimerTest1()
		{
			pf;
			g_pTimer1->KillTimer(this);
		}
		virtual void  OnTimer(unsigned dwTimerID)
		{
			std::string strTmp = "TimerTest1 OnTimer dwTimerID:";
			strTmp += std::to_string(dwTimerID);
			pt(strTmp);
		}
};

class TimerTest2 : public IUTimer
{
	public:
		~TimerTest2()
		{
			pf;
			g_pTimer2->KillTimer(this);
		}
		virtual void  OnTimer(unsigned dwTimerID)
		{
			std::string strTmp = "TimerTest2 OnTimer";
			strTmp += std::to_string(dwTimerID);
			pt(strTmp);
		}
};


bool g_bWorking;
void TimerThread1()
{
	while (g_bWorking)
	{
		g_pTimer1->Run();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}
void TimerThread2()
{
	while (g_bWorking)
	{
		g_pTimer2->Run();
		std::this_thread::sleep_for(std::chrono::microseconds(1));
	}
}

int main()
{
	{
		std::unique_ptr<UTimer> oTmp(new UTimer());
		g_pTimer1 = std::move(oTmp);
	}
	{
		std::unique_ptr<UTimer> oTmp(new UTimer());
		g_pTimer2 = std::move(oTmp);
	}

	TimerTest1 ttt;
	pt("main setTimers");

	unsigned dwTimerID = 0;
	unsigned t = 0;
	unsigned loop = 1000;

	dwTimerID = TIMERID_1 * 10;
	t = dwTimerID;
	/*t = dwTimerID + 1000; g_pTimer1->SetTimer(&ttt, t, t, loop);*/
	t = dwTimerID + 60000; g_pTimer1->SetTimer(&ttt, t, t, loop);
	t = dwTimerID + 300000; g_pTimer1->SetTimer(&ttt, t, t, loop);
	t = dwTimerID + 3600000; g_pTimer1->SetTimer(&ttt, t, t, loop);
	t = dwTimerID + 86400000; g_pTimer1->SetTimer(&ttt, t, t, loop);
	t = dwTimerID + 864000000; g_pTimer1->SetTimer(&ttt, t, t, loop);

	std::this_thread::sleep_for(std::chrono::seconds(1));

	dwTimerID = TIMERID_2 * 10;
	t = dwTimerID;
	/*t = dwTimerID + 1000; g_pTimer2->SetTimer(&ttt, t, t, loop);*/
	t = dwTimerID + 60000; g_pTimer2->SetTimer(&ttt, t, t, loop);
	t = dwTimerID + 300000; g_pTimer2->SetTimer(&ttt, t, t, loop);
	t = dwTimerID + 3600000; g_pTimer2->SetTimer(&ttt, t, t, loop);
	t = dwTimerID + 86400000; g_pTimer2->SetTimer(&ttt, t, t, loop);
	t = dwTimerID + 864000000; g_pTimer2->SetTimer(&ttt, t, t, loop);

	g_bWorking = true;

	std::thread trd1(&TimerThread1);
	trd1.detach();
	
	std::thread trd2(&TimerThread2);
	trd2.detach();

	while (true)
	{
		std::string strInput;
		std::cin >> strInput;
		if (strInput == "e" || strInput == "E")
		{
			break;
		}
		else
		{
			std::cout << "input e to exit." << std::endl;
		}
	}

	g_bWorking = false;
	return 0;
}


