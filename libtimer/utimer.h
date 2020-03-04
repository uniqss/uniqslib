#pragma once

#include <string>
using namespace std;
namespace UNIQS
{
	class UTimerImpl;

	class IUTimer
	{
	public:
		virtual void  OnTimer(unsigned dwTimerID) = 0;
	};

	class UTimer
	{
	public:
		UTimer();

		~UTimer();

		/**
		创建timer pTimer是回调指针 注意：dwLoop是超时后还会执行的次数，填0表示一次性timer，填1表示到时还会再执行一次
		*/
		bool  SetTimer(IUTimer *pTimer, unsigned dwTimerID, unsigned dwElapseMS, unsigned dwLoop = 0xFFFFFFFF);

		bool  KillTimer(IUTimer *pTimer, unsigned dwTimerID);
		// 干掉这个对象上的所有timer,一般在dtor中调用
		bool  KillTimer(IUTimer *pTimer);

		bool  Run();

		std::string  DumpTimerInfo();

	protected:
		UTimer(const UTimer &){};
		UTimer& operator = (const UTimer &);

	private:
		UTimerImpl*  m_pTimerImpl;
	};
}
