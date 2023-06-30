#pragma once

#include "ITest.h"

class PerfomanceTest1 : public ITest
{
	//// Inherited via ITest
	//virtual void Run() override;

	// Inherited via ITest
	virtual void BackgroundUpdate() override;

public:

	PerfomanceTest1(const std::shared_ptr<BulletManager>& bm);

};

