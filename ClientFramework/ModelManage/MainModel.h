#pragma once

#include "IModelInterface.h"

class MainModel : public IModelInterface
{
public:
	MainModel();
	~MainModel();

	bool Start() override;
	void Stop() override;

};

