#include "utimer_impl.h"
#include <math.h>

#include <string>
#include <chrono>

static const unsigned MAX_ENABLE_TIMER = 0xFFFFFFFF - 1000;

namespace UNIQS
{
	UTimerImpl::UTimerImpl() : m_oTimerPool()
	{
		m_oTimerMap.clear();
		for (unsigned i = 0; i < TIMERARRAY_COUNT; ++i)
		{
			m_aoSlotLevels[i].Init(i);
		}

#if (defined(WIN32) || defined(WIN64))
		m_dwMsPerTick = 1;
#else
		m_dwMsPerTick = 1;
#endif
		m_dwLastTickCountMS = UTimerGetTickCount();
	}

	UTimerImpl::~UTimerImpl()
	{
		m_oTimerMap.clear();
	}

	bool UTimerImpl::SetTimer(IUTimer* poTimerHandler, unsigned dwTimerId, unsigned dwElapseMS, unsigned dwLoopCount)
	{
		if (nullptr == poTimerHandler || (dwElapseMS > MAX_ENABLE_TIMER) || (0 == dwElapseMS))
		{
			return false;
		}

		if (LookupTimer(poTimerHandler, dwTimerId) != nullptr)
		{
			return false;
		}

		UTimerItem* poTimer = m_oTimerPool.Alloc();
		if (nullptr == poTimer)
		{
			return false;
		}

		poTimer->Init(this, poTimerHandler, dwElapseMS, dwTimerId, dwLoopCount);
		m_oTimerMap[poTimerHandler][dwTimerId] = poTimer;
		return AddTimerForNextTrigger(poTimer);
	}

	bool UTimerImpl::AddTimerForNextTrigger(UTimerItem * poTimer)
	{
		unsigned dwTimeFromLastMS = GetTimeFromLastMS(m_dwLastTickCountMS);
		unsigned dwLevel = poTimer->SetupForNextTrigger(dwTimeFromLastMS);
		m_aoSlotLevels[dwLevel].AddTimer(poTimer);
		return true;
	}

	bool UTimerImpl::KillTimer(IUTimer* pHandler, unsigned dwTimerId)
	{
		auto it = m_oTimerMap.find(pHandler);
		if (it == m_oTimerMap.end())
			return false;

		auto it2 = it->second.find(dwTimerId);
		if (it2 == it->second.end())
			return false;

		UTimerItem* poTimer = it2->second;
		it->second.erase(it2);
		if (it->second.empty())
		{
			m_oTimerMap.erase(it);
		}
		m_aoSlotLevels[poTimer->m_timerLevel].DelTimer(poTimer);
		m_oTimerPool.Free(poTimer);

		return true;
	}

	bool UTimerImpl::KillTimer(IUTimer* pHandler)
	{
		auto it = m_oTimerMap.find(pHandler);
		if (it == m_oTimerMap.end())
			return false;

		for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2)
		{
			UTimerItem* poTimer = it2->second;

			m_aoSlotLevels[poTimer->m_timerLevel].DelTimer(poTimer);
			m_oTimerPool.Free(poTimer);
		}

		m_oTimerMap.erase(it);

		return true;
	}

	bool UTimerImpl::Run()
	{
		for (unsigned dwTimeFromLastMS = GetTimeFromLastMS(); dwTimeFromLastMS > 0; --dwTimeFromLastMS)
		{
			TrigerTimersForCurSlot();
		}
		return true;
	}

	unsigned UTimerImpl::TrigerTimersForCurSlot()
	{
		auto& oCurSlot = m_aoSlotLevels[0].GetCurSlot();
		unsigned dwTimeOutCount = 0;

		while (!oCurSlot.empty())
		{
			UTimerItem * poTimer = oCurSlot.front();
			poTimer->SetLastExpdTime(UTimerGetTickCount());
			poTimer->OnTimer();

			if (oCurSlot.empty() || (oCurSlot.front() != poTimer))
				continue;

			oCurSlot.pop_front();
			if (poTimer->CheckLoop())
			{
				AddTimerForNextTrigger(poTimer);
			}
			++dwTimeOutCount;
		}
		if (!m_aoSlotLevels[0].NextSlot(true))
			ReloadSlotLevel();

		return dwTimeOutCount;
	}

	bool UTimerImpl::ReloadSlotLevel(unsigned dwLevel)
	{
		if (dwLevel >= TIMERARRAY_COUNT - 1)
			return false;

		UTimerSlotLevel& oNextLevel = m_aoSlotLevels[dwLevel + 1];

		if (!oNextLevel.NextSlot(true))
			ReloadSlotLevel(dwLevel + 1);

		m_aoSlotLevels[dwLevel].ReloadFromLevel(oNextLevel);
		return true;
	}

	std::string UTimerImpl::DumpTimerInfo()
	{
		std::stringstream info;
		for (unsigned level = 0; level < TIMERARRAY_COUNT; ++level)
		{
			info << m_aoSlotLevels[level].DumpTimerInfo() << std::endl;
		}
		return info.str();
	}

	unsigned UTimerImpl::GetTimeFromLastMS()
	{
		unsigned dwCurTick = UTimerGetTickCount();
		unsigned dwTickFromLast;
		if (dwCurTick < m_dwLastTickCountMS)
		{
#if (defined(WIN32) || defined(WIN64))
			dwTickFromLast = (unsigned)0xFFFFFFFF - m_dwLastTickCountMS + dwCurTick + 1;
#else
			dwTickFromLast = (unsigned)0x7FFFFFFF - m_dwLastTickCountMS + dwCurTick + 1;
#endif
		}
		else
		{
			dwTickFromLast = dwCurTick - m_dwLastTickCountMS;
		}
		m_dwLastTickCountMS = dwCurTick;
		return dwTickFromLast * m_dwMsPerTick;
	}

	unsigned UTimerImpl::GetTimeFromLastMS(unsigned dwLastTimeMS)
	{
		unsigned dwCurTickMS = UTimerGetTickCount();
		unsigned dwTickCountFromLastMS;
		if (dwCurTickMS < dwLastTimeMS)
		{
#if (defined(WIN32) || defined(WIN64))
			dwTickCountFromLastMS = (unsigned)0xFFFFFFFF - dwLastTimeMS + dwCurTickMS + 1;
#else
			dwTickCountFromLastMS = (unsigned)0x7FFFFFFF - dwLastTimeMS + dwCurTickMS + 1;
#endif
		}
		else
		{
			dwTickCountFromLastMS = dwCurTickMS - dwLastTimeMS;
		}

		return dwTickCountFromLastMS * m_dwMsPerTick;
	}

	UTimerItem* UTimerImpl::LookupTimer(IUTimer* poTimerHandler, unsigned dwTimerId)
	{
		auto it = m_oTimerMap.find(poTimerHandler);
		if (it == m_oTimerMap.end())
			return nullptr;
		auto it2 = it->second.find(dwTimerId);
		if (it2 == it->second.end())
			return nullptr;

		return it2->second;
	}
}
