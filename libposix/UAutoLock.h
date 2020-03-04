#pragma once

#include "umutex.h"

namespace  UNIQS
{
	// 一个简单的自动释放的锁
	template <class MT = UMutex>
	class UAutoLock
	{
	public:
		UAutoLock(MT& rMutex) :m_mutex(rMutex)
		{
			m_mutex.Lock();
		}
		~UAutoLock()
		{
			m_mutex.Unlock();
		}
	private:
		MT& m_mutex;
	};

	typedef UAutoLock<UMutex>  UMutexLock;
}

