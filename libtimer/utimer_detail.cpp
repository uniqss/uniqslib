#include "utimer_impl.h"
#include "utimer_detail.h"

namespace UNIQS
{
	void UTimerItem::Init(UTimerImpl* pTimerCtrl, IUTimer* poTimerHandler, unsigned dwElapseMS, unsigned dwTimerId, unsigned dwLoopCount)
	{
		m_poTimerHandler = poTimerHandler;
		m_dwElapseMS = dwElapseMS;
		m_dwTimerID = dwTimerId;
		m_timerLevel = TIMERARRAY_COUNT;
		m_dwLastExpTime = UTimerGetTickCount();
		m_dwLoopCount = dwLoopCount;
		for (unsigned i = 0; i < TIMERARRAY_COUNT; ++i)
		{
			m_adwSlotIndexes[i] = pTimerCtrl->GetSlotLevel(i).GetCurSlotIndex();
		}
	}

	unsigned UTimerItem::SetupForNextTrigger(unsigned dwDelayMS)
	{
		unsigned level = 0;

		m_adwSlotIndexes[0] += m_dwElapseMS + dwDelayMS;
		for (level = 0; level < TIMERARRAY_COUNT - 1; ++level)
		{
			if (m_adwSlotIndexes[level] < TIMER_ARRAY_LEN)
				break;
			m_adwSlotIndexes[level + 1] += m_adwSlotIndexes[level] / TIMER_ARRAY_LEN;
			m_adwSlotIndexes[level] = m_adwSlotIndexes[level] % TIMER_ARRAY_LEN;
		}

		// 针对最高一层进行特殊处理
		if (m_adwSlotIndexes[TIMERARRAY_COUNT - 1] >= TIMER_ARRAY_LEN)
		{
			m_adwSlotIndexes[TIMERARRAY_COUNT - 1] %= TIMER_ARRAY_LEN;
		}

		return level;
	}

	void UTimerItem::OnTimer()
	{
		return m_poTimerHandler->OnTimer(m_dwTimerID);
	}

	unsigned UTimerItem::GetSlotIndexForNextTrigger(unsigned level)
	{
		return m_adwSlotIndexes[level];
	}


	UTimerSlotLevel::UTimerSlotLevel() :
		m_timerLevel(0), m_dwCurSlot(0)
	{
	}

	bool UTimerSlotLevel::NextSlot(bool bRewindOnEnd)
	{
		if (TIMER_ARRAY_LEN - 1 == m_dwCurSlot)
		{
			if (bRewindOnEnd)
				m_dwCurSlot = 0;
			return false;
		}
		else
		{
			++m_dwCurSlot;
		}
		return true;
	}

	void UTimerSlotLevel::ReloadFromLevel(UTimerSlotLevel& oSlotLevel)
	{
		auto& slot = oSlotLevel.m_aoTimerSlots[oSlotLevel.m_dwCurSlot];
		UTimerItem* poTimer = nullptr;
		while (!slot.empty())
		{
			poTimer = slot.front();
			slot.pop_front();
			AddTimer(poTimer);
		}
	}
}
