#pragma once

/*
˵������ VS2013 WIN7 X64 ������������CRITICAL_SECTION��C11��std::mutex����Ҫ�߳�����
������ʱ����ʹ��CRITICAL_SECTION

�����汾ǿ�ҽ���ʹ��std::mutex���п�ƽ̨
*/

#if !(defined(WIN32) || defined(WIN64))
#include <pthread.h>
#else
#include <windows.h>
#endif

namespace UNIQS
{
#if (defined(WIN32) || defined(WIN64))
	typedef CRITICAL_SECTION UMutexId;
#else
	typedef pthread_mutex_t UMutexId;
#endif

	bool UFun_MutexInit(UMutexId& id);

	void UFun_MutexLock(UMutexId& id);

	void UFun_MutexUnlock(UMutexId& id);

	bool UFun_MutexUninit(UMutexId&id);

	class UNonMutex
	{
	public:
		inline void  Lock() {}

		inline void  Unlock() {}
	};

	class UMutex
	{
	public:
		UMutex();
		~UMutex();

		void Lock();

		void Unlock();

	private:
		UMutex(const UMutex&);
		UMutex& operator= (const UMutex&);

	private:
#if (defined(WIN32) || defined(WIN64))
		UMutexId m_mutexId;
#else
		mutable UMutexId m_mutexId;
#endif
	};
}
