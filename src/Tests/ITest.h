#pragma once

#include <memory>

#include "../BulletManager/BulletManager.h"

/// <summary>
/// Interface for test case used in app.
/// </summary>
class ITest
{
	std::thread bgThread;
	std::atomic<bool> bgTerminate = false;
	std::atomic<bool> bgSyncFPS = false;

	void BackgroundLoop();

protected:

	std::shared_ptr<BulletManager> bulletManager;

	bool runTest = false;

	/// <summary>
	/// background update for simulating access to bullet manager from background thread
	/// </summary>
	virtual void BackgroundUpdate();

public:

	ITest(const std::shared_ptr<BulletManager>& bm);
	virtual ~ITest();

	/// <summary>
	/// Launch test event. May be called multiple for same test.
	/// </summary>
	void Run();

	/// <summary>
	/// Sync background thread to avoid exceeding main thread FPS.
	/// </summary>
	void SyncBackgroundThread();
};

