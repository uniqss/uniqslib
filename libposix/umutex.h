#pragma once

/*
说明：在 VS2013 WIN7 X64 环境测试下来CRITICAL_SECTION比C11的std::mutex性能要高出不少
所以暂时还是使用CRITICAL_SECTION

后续版本强烈建议使用std::mutex进行跨平台
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
