#pragma once

#include "uobject_allocator.h"

namespace UNIQS
{
	template <typename T, typename MT = UNonMutex, typename _Alloc = UObjectAllocator<T, MT> >
	class UObjectPool
	{
	public:

		UObjectPool(unsigned dwInitCount = 0, unsigned dwGrouCount = 1)
			:m_allocator(dwInitCount, dwGrouCount)
		{
		}

		inline T* Alloc()
		{
			return new(m_allocator.allocate())T;
		}

		template<typename P1>
		T * Alloc(P1 &p1)
		{
			return new(m_allocator.allocate())T(p1);
		}

		template<typename P1, typename P2>
		T* Alloc(P1 &p1, P2 &p2)
		{
			return new(m_allocator.allocate())T(p1, p2);
		}

		template<typename P1, typename P2, typename P3>
		T* Alloc(P1 &p1, P2 &p2, P3 &p3)
		{
			return new(m_allocator.allocate())T(p1, p2, p3);
		}

		template<typename P1, typename P2, typename P3, typename P4>
		T* Alloc(P1 &p1, P2 &p2, P3 &p3, P4 &p4)
		{
			return new(m_allocator.allocate())T(p1, p2, p3, p4);
		}

		template<typename P1, typename P2, typename P3, typename P4, typename P5>
		T* Alloc(P1 &p1, P2 &p2, P3 &p3, P4 &p4, P5 &p5)
		{
			return new(m_allocator.allocate())T(p1, p2, p3, p4, p5);
		}

		template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
		T* Alloc(P1 &p1, P2 &p2, P3 &p3, P4 &p4, P5 &p5, P6 &p6)
		{
			return new(m_allocator.allocate())T(p1, p2, p3, p4, p5, p6);
		}

		void  Free(T * p)
		{
			p->~T();
			m_allocator.deallocate(p);
		}

		_Alloc & GetAllocator()
		{
			return m_allocator;
		}

	private:
		_Alloc m_allocator;

	};
}

