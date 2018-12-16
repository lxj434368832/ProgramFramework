#include "MLock.h"

MLock::MLock()
{
	InitializeCriticalSection(&cs);
}


MLock::~MLock()
{
	DeleteCriticalSection(&cs);
}

void MLock::lock()
{
	EnterCriticalSection(&cs);
}

void MLock::unlock()
{
	LeaveCriticalSection(&cs);
}
