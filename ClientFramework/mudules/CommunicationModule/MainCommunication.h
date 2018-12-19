#pragma once

#include "ICommunication.h"

class MainCommunication : public ICommunication
{
public:
	MainCommunication(IMainClient *main = nullptr);
	~MainCommunication();


	bool Start() override;
	void Stop() override;

private:


};

