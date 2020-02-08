#pragma once

#include <Windows.h>

class MLock
{
public:
	MLock();
	~MLock();
	void lock();
	void unlock();
private:
	CRITICAL_SECTION cs;
};

class MAutoLock
{
public:
	MAutoLock() = delete;
	MAutoLock(MLock &lock)
		:m_lock(lock)
	{
		m_lock.lock();
	}

	~MAutoLock()
	{
		m_lock.unlock();
	}

private:
	MLock &m_lock;
};
