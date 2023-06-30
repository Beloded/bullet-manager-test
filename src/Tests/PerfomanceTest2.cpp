#include "PerfomanceTest2.h"
#include <iostream>

PerfomanceTest2::PerfomanceTest2(const std::shared_ptr<BulletManager>& bm) : ITest(bm)
{

}

void PerfomanceTest2::BackgroundUpdate()
{
	const int kNumber = 1000;

	if (!runTest)
		return;

	runTest = false;

	// create bullets

	for (int i = 0; i < kNumber; ++i)
	{
		bulletManager->AddRandomBullet();
	}

	// create segments

	for (int i = 0; i < kNumber; ++i)
	{
		bulletManager->AddRandomSegment();
	}
}
