#pragma once

#include "uobjectpool.h"
#include "utimer_detail.h"

#include <unordered_map>

namespace UNIQS
{
	class UTimerImpl
	{
	public:
		UTimerImpl();
		~UTimerImpl();

		bool  SetTimer(IUTimer* poTimerHandler, unsigned dwTimerId, unsigned dwElapseMS, unsigned dwLoop = 0xFFFFFFFF);

		bool  KillTimer(IUTimer* pHandler, unsigned dwTimerId);

		bool KillTimer(IUTimer* pHandler);

		bool  Run();

		std::string  DumpTimerInfo();
	private:
		friend class UTimerItem;

		inline UTimerSlotLevel& GetSlotLevel(unsigned nLevel)
		{
			return m_aoSlotLevels[nLevel];
		}

		bool ReloadSlotLevel(unsigned dwLevel = 0);

		unsigned TrigerTimersForCurSlot();

		bool AddTimerForNextTrigger(UTimerItem *pTimer);

		unsigned GetTimeFromLastMS();
		unsigned GetTimeFromLastMS(unsigned dwLastTimeMS);

		UTimerItem* LookupTimer(IUTimer* poTimerHandler, unsigned dwTimerId);

		UObjectPool<UTimerItem, UMutex >			m_oTimerPool;
		UTimerSlotLevel								m_aoSlotLevels[TIMERARRAY_COUNT];
		std::unordered_map<IUTimer*, std::unordered_map<unsigned, UTimerItem*>>	m_oTimerMap;
		unsigned									m_dwLastTickCountMS;
		unsigned									m_dwMsPerTick;
	};
}
