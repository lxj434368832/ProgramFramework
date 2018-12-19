#pragma once

#include "IModelInterface.h"

class MainModel : public IModelInterface
{
public:
	MainModel(IMainClient *_main = nullptr);
	~MainModel();

	bool Start() override;
	void Stop() override;

};

