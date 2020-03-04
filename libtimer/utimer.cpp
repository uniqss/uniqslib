#include "utimer.h"
#include "utimer_impl.h"

namespace UNIQS
{
	UTimer::UTimer()
	{
		m_pTimerImpl = new UTimerImpl();
	}
	UTimer::~UTimer()
	{
		delete m_pTimerImpl;
	}

	bool UTimer::SetTimer(IUTimer *pTimer, unsigned dwTimerID, unsigned dwElapseMS, unsigned dwLoop /*= 0xFFFFFFFF*/)
	{
		return m_pTimerImpl->SetTimer(pTimer, dwTimerID, dwElapseMS, dwLoop);
	}

	bool UTimer::KillTimer(IUTimer *pTimer, unsigned dwTimerId)
	{
		return m_pTimerImpl->KillTimer(pTimer, dwTimerId);
	}

	bool UTimer::KillTimer(IUTimer *pTimer)
	{
		return m_pTimerImpl->KillTimer(pTimer);
	}

	// ע�������������ѭ���е���һ�£����򲻻ᴥ��
	bool UTimer::Run()
	{
		return  m_pTimerImpl->Run();
	}

	std::string UTimer::DumpTimerInfo()
	{
		return m_pTimerImpl->DumpTimerInfo();
	}
};
