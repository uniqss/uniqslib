#pragma once

#include "../libposix/umutex.h"
#include "../libposix/UAutoLock.h"
#include <deque>
#include <vector>

namespace UNIQS
{
	/*
	ע�����ObjectAllocator�Ĵ�С��ֻ�������ģ��Ե��ڴ��Խ��Խ�࣬�����ͷ�
	*/
	template<typename T, typename MT = UNonMutex>
	class UObjectAllocator
	{
		enum
		{
			POOL_INFLATE_SIZE = 100,
		};
	public:
		typedef size_t		size_type;
		typedef T*			pointer;
		typedef T&			reference;
		typedef const T*	const_pointer;
		typedef const T&	const_reference;
		typedef T			value_type;

		typedef std::deque<pointer> ObjectQueue;

		UObjectAllocator(int initSize = 0, int growSize = 1) throw() : m_totalSize(0), m_growSize(growSize), m_allocCount(0), m_allocFreq(0)
		{
			UAutoLock<MT> lock(m_mutex);
			inflate(initSize);
		}

		~UObjectAllocator() throw()
		{
			UAutoLock<MT> lock(m_mutex);

			for (typename ObjectQueue::iterator itr = m_objects.begin(); itr != m_objects.end(); ++itr)
			{
				::operator delete (*itr);
			}
			m_objects.clear();
		}

		pointer address(reference __x) const
		{
			return &__x;
		}

		const_pointer address(const_reference __x) const
		{
			return &__x;
		}

		T* allocate()
		{
			m_mutex.Lock();
			++m_allocFreq;
			if (m_objects.empty())
			{
				this->inflate(m_growSize);
			}

			T * ret = m_objects.front();
			m_objects.pop_front();
			m_mutex.Unlock();
			return ret;
		}

		void deallocate(pointer __p)
		{
			m_mutex.Lock();
			// �����Ҫ�ͷ��ڴ棬���ڴ������£������ͷŵĹ���Ϊ��ǰʹ�õ��ڴ�Ϊ������ڴ��1/10ʱ���ͷŵ�50%֮��
			// Ҳ����ģ���м�һ��������ʶ�Ƿ��ͷ�
			// �ͷ�����ܼ򵥣������Ҫ�ͷŵ�������һ������m_objects���ó�����������OK��
			m_objects.push_back(__p);
			m_mutex.Unlock();
		}

		inline size_type read_size() const
		{
			UAutoLock<MT> lock(m_mutex);
			return m_objects.size();
		}

		inline size_type write_size() const throw()
		{
			UAutoLock<MT> lock(m_mutex);
			return m_objects.size();
		}

		size_type total_size() const
		{
			UAutoLock<MT> lock(m_mutex);
			return m_totalSize;
		}
		size_type alloc_count() const
		{
			UAutoLock<MT> lock(m_mutex);
			return m_allocCount;
		}
		size_type alloc_freq() const
		{
			UAutoLock<MT> lock(m_mutex);
			size_type ret = m_allocFreq;
			m_allocFreq = 0;
			return ret;
		}
	private:
		void inflate(int count = POOL_INFLATE_SIZE)
		{
			for (int i = 0; i < count; ++i)
			{
				pointer _p = static_cast<pointer>(::operator new (sizeof(value_type)));
				m_objects.push_back(_p);
			}
			m_totalSize += count;
			++m_allocCount;
		}

		UObjectAllocator(const UObjectAllocator&) throw(){}
		template<typename T1>
		UObjectAllocator(const UObjectAllocator<T1>&) throw(){}
		template<typename T1>
		void operator = (const UObjectAllocator<T1>&) throw(){}

		ObjectQueue m_objects;
		mutable MT m_mutex;
		int m_totalSize;
		int m_growSize;
		int m_allocCount;
		mutable int m_allocFreq;
	};
}

