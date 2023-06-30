#pragma once

#include "ITest.h"

class PerfomanceTest2 : public ITest
{
	//// Inherited via ITest
	//virtual void Run() override;

	// Inherited via ITest
	virtual void BackgroundUpdate() override;

public:

	PerfomanceTest2(const std::shared_ptr<BulletManager>& bm);
};

