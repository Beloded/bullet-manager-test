#pragma once

#include "ITest.h"

class StabilityTest : public ITest
{	
	virtual void BackgroundUpdate() override;

public:

	StabilityTest(const std::shared_ptr<BulletManager>& bm);
	virtual ~StabilityTest();
};

