#pragma once

#include "utimer.h"
#include <list>
#include <sstream>

#if (defined(WIN32) || defined(WIN64))
#include <windows.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif

namespace UNIQS
{
	static unsigned UTimerGetTickCount()
	{
#if (defined(WIN32) || defined(WIN64))
		unsigned dwRet = GetTickCount();
#else
#if 1
		struct timeval cateq_v;
		struct timezone cateq_z;

		gettimeofday(&cateq_v, &cateq_z);

		unsigned dwRet = (unsigned)(1000.0 * (double)(cateq_v.tv_sec) + (double)(cateq_v.tv_usec) / 1000.0);
#else
		struct timespec ts;

		clock_gettime(CLOCK_MONOTONIC, &ts);

		unsigned dwRet = (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
#endif
#endif
		return dwRet;
	}

	static const unsigned TIMER_ARRAY_LEN = 1000;
	static const unsigned TIMERARRAY_COUNT = 5;

	class UTimerImpl;
	class UTimerItem
	{
	public:
		unsigned m_timerLevel;

		void Init(UTimerImpl* poTimerCtrl,
			IUTimer* poTimerHandler,
			unsigned dwElapseMS,
			unsigned dwTimerID,
			unsigned loopCount);

		unsigned SetupForNextTrigger(unsigned dwDelayMS);

		inline void SetLastExpdTime(unsigned dwExpTime)
		{
			m_dwLastExpTime = dwExpTime;
		}

		inline unsigned GetLastExpdTime()
		{
			return 	m_dwLastExpTime;
		}

		unsigned GetSlotIndexForNextTrigger(unsigned level);

		void OnTimer();

		inline bool CheckLoop()
		{
			if (m_dwLoopCount == 0xFFFFFFFF)
			{
				return true;
			}
			return m_dwLoopCount-- > 0;
		}

	private:
		unsigned	m_dwTimerID;
		unsigned	m_dwElapseMS;
		IUTimer*	m_poTimerHandler;
		unsigned	m_adwSlotIndexes[TIMERARRAY_COUNT];
		unsigned	m_dwLastExpTime;
		unsigned	m_dwLoopCount;
	};

	class UTimerSlotLevel
	{
	public:
		UTimerSlotLevel();

		bool NextSlot(bool bRewindOnEnd);

		void ReloadFromLevel(UTimerSlotLevel& oSlotLevel);

		void Init(unsigned level)
		{
			m_timerLevel = level;
		}

		inline unsigned GetCurSlotIndex()
		{
			return m_dwCurSlot;
		}

		std::list<UTimerItem*>& GetCurSlot()
		{
			return m_aoTimerSlots[m_dwCurSlot];
		}

		inline void AddTimer(UTimerItem* poTimer)
		{
			poTimer->m_timerLevel = m_timerLevel;
			m_aoTimerSlots[poTimer->GetSlotIndexForNextTrigger(m_timerLevel)]
				.push_back(poTimer);
		}

		inline void DelTimer(UTimerItem* poTimer)
		{
			if (poTimer->m_timerLevel != m_timerLevel)
			{
				return;
			}
			m_aoTimerSlots[poTimer->GetSlotIndexForNextTrigger(m_timerLevel)].remove(poTimer);
		}

		std::string DumpTimerInfo()
		{
			std::stringstream info;
			for (unsigned slot = 0; slot < TIMER_ARRAY_LEN; ++slot)
			{
				if (!m_aoTimerSlots[slot].empty())
				{
					info << "Timer in ArrayLevel " << m_timerLevel << ", Slot " << slot << std::endl;
				}
			}
			return info.str();
		}

	private:
		unsigned				m_timerLevel;
		unsigned				m_dwCurSlot;
		std::list<UTimerItem*>	m_aoTimerSlots[TIMER_ARRAY_LEN];
	};
}
