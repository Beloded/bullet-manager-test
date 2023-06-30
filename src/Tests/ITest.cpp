#include "ITest.h"
#include <iostream>
#include <thread>

ITest::ITest(const std::shared_ptr<BulletManager>& bm) : bgThread{ }, bgTerminate(false)
{
	this->bulletManager = bm;
	bgThread = std::thread(&ITest::BackgroundLoop, this);
	
	std::cout << "Create test " << this << std::endl;
}


ITest::~ITest()
{
	// shutdown bg thread and wait

	bgTerminate = true;

	if (bgThread.joinable())
		bgThread.join();

	std::cout << "Destroy test " << this << std::endl;
}

//void ITest::SetBulletManager(const std::shared_ptr<BulletManager>& bm)
//{
//	
//}

void ITest::Run()
{
	runTest = true;
}

void ITest::SyncBackgroundThread()
{
	bgSyncFPS = true;
}

void ITest::BackgroundLoop()
{	
	while (!bgTerminate)
	{
		if (!bgSyncFPS)
		{
			// limit bg thread fps with main thread fps

			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			continue;
		}

		BackgroundUpdate();

		//std::cout << "BG test " << this << std::endl;

		bgSyncFPS = false;
	}
}

void ITest::BackgroundUpdate()
{	

}
