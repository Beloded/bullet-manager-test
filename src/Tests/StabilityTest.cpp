#include "StabilityTest.h"

StabilityTest::StabilityTest(const std::shared_ptr<BulletManager>& bm) : ITest(bm)
{
	bulletManager->Reset();
}

StabilityTest::~StabilityTest()
{
	bulletManager->Reset();
}

void StabilityTest::BackgroundUpdate()
{
	const int kNumber = 20;

	if (!runTest)
		return;

	// create bullets

	for (int i = 0; i < kNumber; ++i)
	{
		bulletManager->AddRandomBullet();
	}
}
