#ifndef __SWARTZ_MUTEXEX_H_
#define __SWARTZ_MUTEXEX_H_

#include "swartz_mutex.h"

class CMutex
{
public:
	inline CMutex() : m_mutex(0)
	{
		swartz_mutex_create(&m_mutex, SWARTZ_MUTEX_ATTR_RECURSIVE);
	}

	~CMutex()
	{
		if (m_mutex != 0) {
			swartz_mutex_destroy(m_mutex);
		}
	}

	void Lock() const
	{
		if (m_mutex != 0) {
			swartz_mutex_lock(m_mutex);
		}
	}

	void UnLock() const
	{
		if (m_mutex != 0) {
			swartz_mutex_unlock(m_mutex);
		}
	}

private:

	// noncopyable
	CMutex(const CMutex&);
	const CMutex& operator=(const CMutex&);

	swartz_mutex_t* m_mutex;
};

template<typename T>
class CGuard
{
public:
	// = Initialization and termination methods.
	CGuard(T& m) : mutex_(m)
	{
		mutex_.Lock();
	};
	~CGuard()
	{
		try{
			mutex_.UnLock();
		} 
		catch (...){
		}
	};
private:
	CGuard();
	// noncopyable
	CGuard(const CGuard&);
	const CGuard& operator=(const CGuard&);

	T& mutex_;
};


#endif

