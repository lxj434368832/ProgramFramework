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
	MAutoLock(MLock *pLock)
	{
		m_pLock = pLock;
		pLock->lock();
	}

	~MAutoLock()
	{
		m_pLock->lock();
	}

private:
	MLock *m_pLock;
};