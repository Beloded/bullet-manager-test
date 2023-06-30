#pragma once

struct float2;

#include <vector>
#include "Bullet.h"
#include "Wall.h"
#include <mutex>

struct SDL_Renderer;
struct SDL_Window;

class BulletManager
{
	std::vector<Wall> walls;
	std::vector<Bullet> bullets;

	std::vector<Bullet> newBullets;
	std::vector<Wall> newWalls;

	std::mutex mxNewBullets;
	std::mutex mxNewWalls;

	float2 windowSize;

	bool resetAll = false;

public:

	BulletManager();
	~BulletManager();

	void Update(float time);

private:

	void UpdateSpawn(float dt);
	void UpdateBullets(float dt);

public:

	/// <summary>
	/// may be called async
	/// </summary>
	void Fire(float2 pos, float2 dir, float speed, float time, float life_time);

	void AddRandomBullet();
	void AddRandomSegment();
	void Reset();

	void Render(SDL_Renderer* r);
	void SetWindowSize(int w, int h);

	int GetBulletsCount() const;
	int GetWallsCount() const;
	int GetNewBulletsCount() const;
	int GetNewWallsCount() const;
};

