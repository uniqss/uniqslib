#include <cassert>
#include <cerrno>
#include "umutex.h"
#include "UAutoLock.h"

#if (defined(WIN32) || defined(WIN64))
#  include <windows.h>
#  include <winbase.h>
#else
#  include <pthread.h>
#endif

namespace UNIQS
{
	bool UFun_MutexInit(UMutexId& mid)
	{
#if (defined(WIN32) || defined(WIN64))
		InitializeCriticalSection(&mid);
		return TRUE;
#else
		int  rc = pthread_mutex_init(&mid, 0);
		assert(rc == 0);
		return rc == 0;
#endif
	}

	void UFun_MutexLock(UMutexId& mid)
	{
#if (defined(WIN32) || defined(WIN64))
		EnterCriticalSection(&mid);
#else
		int  rc = pthread_mutex_lock(&mid);
		assert(rc != EINVAL);
		assert(rc != EDEADLK);
		assert(rc == 0);
#endif
	}

	void UFun_MutexUnlock(UMutexId& mid)
	{
#if (defined(WIN32) || defined(WIN64))
		LeaveCriticalSection(&mid);
#else
		int  rc = pthread_mutex_unlock(&mid);
		assert(rc != EINVAL);
		assert(rc != EPERM);
		assert(rc == 0);
#endif
	}

	bool UFun_MutexUninit(UMutexId& mid)
	{
#if (defined(WIN32) || defined(WIN64))
		DeleteCriticalSection(&mid);
		return 0;
#else
		int  rc = pthread_mutex_destroy(&mid);
		assert(rc != EBUSY);
		assert(rc == 0);
		return rc == 0;
#endif
	}

	UMutex::UMutex()
	{
		UFun_MutexInit(m_mutexId);
	}

	UMutex::~UMutex()
	{
		UFun_MutexUninit(m_mutexId);
	}

	void  UMutex::Lock()
	{
		UFun_MutexLock(m_mutexId);
	}

	void  UMutex::Unlock()
	{
		UFun_MutexUnlock(m_mutexId);
	}
}

