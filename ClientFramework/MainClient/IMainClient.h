#pragma once

class IMainClient
{
public:
	virtual ~IMainClient(){}

	virtual bool Start() = 0;
	virtual void Stop() = 0;

private:

};
