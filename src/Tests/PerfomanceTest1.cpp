#include "PerfomanceTest1.h"
#include <iostream>

PerfomanceTest1::PerfomanceTest1(const std::shared_ptr<BulletManager>& bm) : ITest(bm)
{

}


void PerfomanceTest1::BackgroundUpdate()
{
	const int kNumber = 100;

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

	std::cout << clock() << std::endl;
}
