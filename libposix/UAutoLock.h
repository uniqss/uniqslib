#pragma once

#include "umutex.h"

namespace  UNIQS
{
	// һ���򵥵��Զ��ͷŵ���
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

