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
		����timer pTimer�ǻص�ָ�� ע�⣺dwLoop�ǳ�ʱ�󻹻�ִ�еĴ�������0��ʾһ����timer����1��ʾ��ʱ������ִ��һ��
		*/
		bool  SetTimer(IUTimer *pTimer, unsigned dwTimerID, unsigned dwElapseMS, unsigned dwLoop = 0xFFFFFFFF);

		bool  KillTimer(IUTimer *pTimer, unsigned dwTimerID);
		// �ɵ���������ϵ�����timer,һ����dtor�е���
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
