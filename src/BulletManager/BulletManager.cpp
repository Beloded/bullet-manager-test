#include "BulletManager.h"
#include "float2.h"
#include <iostream>
#include "SDL.h"

#include "../Extensions.h"

BulletManager::BulletManager()
{
	srand(time(NULL));

	windowSize = { 0, 0 };

	bullets.reserve(10000);
	newBullets.reserve(1000);

	walls.reserve(1000);
	newWalls.reserve(1000);

	std::cout << "BM init " << this << std::endl;
}

BulletManager::~BulletManager()
{
	std::cout << "BM destroy " << this << std::endl;
}

void BulletManager::Reset()
{
	resetAll = true;
}

void BulletManager::UpdateSpawn(float dt)
{
	if (resetAll)
	{
		bullets.clear();
		walls.clear();
	}

	// spawn new bullets
	{
		mxNewBullets.lock();

		if (resetAll)
			newBullets.clear();

		if (newBullets.size() > 0)
		{
			for (int i = newBullets.size() - 1; i >= 0; ) // remove order
			{
				Bullet& b = newBullets[i];

				if ((b.spawnTime -= dt) <= 0)
				{
					bullets.push_back(b);

					Extensions::EraseQuick(newBullets, i);

					if (i == newBullets.size())
						--i;
				}
				else
					--i;
			}
		}

		mxNewBullets.unlock();
	}

	// spawn new segments
	{
		mxNewWalls.lock();

		if (resetAll)
			newWalls.clear();

		if (newWalls.size() > 0)
		{
			int cnt = walls.size();

			// copy new items into array end

			walls.resize(cnt + newWalls.size());
			memcpy(&walls[cnt], &newWalls[0], sizeof(Wall) * newWalls.size());

			newWalls.clear();
		}

		mxNewWalls.unlock();
	}

	resetAll = false;
}

void BulletManager::UpdateBullets(float dt)
{
	// move bullets
	{
		for (int bi = bullets.size() - 1; bi >= 0; )
		{
			Bullet& b = bullets[bi];

			if (//
				(b.lifeTime -= dt) <= 0
				||
				//remove out of screen bounds
				(b.position.x < 0 || b.position.x > windowSize.x || b.position.y < 0 || b.position.y > windowSize.y)
				)
			{
				Extensions::EraseQuick(bullets, bi);

				if (bi == bullets.size())
					--bi;
			}
			else
			{
				--bi;

				float mBulletDelta = b.speed * dt;
				auto vBulletDelta = b.direction * mBulletDelta;

				bool reflected = false;

				for (int wi = walls.size() - 1; wi >= 0; --wi)
				{
					Wall& w = walls[wi];

					float2 pIntersection;
					if (float2::GetIntersectPoint(pIntersection, b.position, vBulletDelta, w.p0, w.p1))
					{
						auto perpenicular = (w.p1 - w.p0).Normalized().Perpendicular();

						b.direction = float2::Reflect(b.direction, perpenicular);

						// calculate proper position after hit

						float mToIntersection = (pIntersection - b.position).Magnitude();
						float mToNewPoint = mBulletDelta - mToIntersection;

						b.position = pIntersection + b.direction * mToNewPoint;

						//

						Extensions::EraseQuick(walls, wi);
						reflected = true;

						break;
					}
				}

				//

				if (!reflected)
				{
					b.position = b.position + vBulletDelta;
				}
			}
		}
	}
}

void BulletManager::Update(float dt)
{
	UpdateSpawn(dt);

	UpdateBullets(dt);
}

void BulletManager::Fire(float2 pos, float2 dir, float speed, float time, float life_time)
{
	mxNewBullets.lock();

	newBullets.push_back({ pos, dir, speed, time, life_time });

	mxNewBullets.unlock();
}

void BulletManager::Render(SDL_Renderer* r)
{
	// draw walls

	{
		SDL_SetRenderDrawColor(r, 0, 255, 0, 255);

		for (int i = 0; i < walls.size(); ++i)
		{
			Wall& s = walls[i];

			SDL_RenderDrawLine(r, s.p0.x, s.p0.y, s.p1.x, s.p1.y);
		}
	}

	// draw bullets

	{
		const float kTrailTime = 0.05f;

		// draw trail

		SDL_SetRenderDrawColor(r, 255, 0, 0, 255);

		for (int i = 0; i < bullets.size(); ++i)
		{
			Bullet& b = bullets[i];

			auto pEnd = b.position - b.direction * b.speed * kTrailTime;

			SDL_RenderDrawLine(r, b.position.x, b.position.y, pEnd.x, pEnd.y);
		}

		// draw head

		SDL_SetRenderDrawColor(r, 255, 255, 255, 255);

		for (int i = 0; i < bullets.size(); ++i)
		{
			Bullet& b = bullets[i];

			SDL_RenderDrawPoint(r, b.position.x, b.position.y);
		}
		
	}
	
}

int BulletManager::GetBulletsCount() const
{
	return bullets.size();
}

int BulletManager::GetWallsCount() const
{
	return walls.size();
}

int BulletManager::GetNewBulletsCount() const
{
	return newBullets.size();
}

int BulletManager::GetNewWallsCount() const
{
	return newWalls.size();
}

void BulletManager::SetWindowSize(int w, int h)
{
	windowSize.x = (float)w;
	windowSize.y = (float)h;
}

void BulletManager::AddRandomBullet()
{
	float2 pos = float2(rand() % (int)windowSize.x, rand() % (int)windowSize.y);

	float2 direction{
		Extensions::Random(1.0f, 10.0f),
		Extensions::Random(1.0f, 10.0f)
	}; // non-zero

	// random flip

	if (rand() % 2 == 0)
		direction.x = -direction.x;

	if (rand() % 2 == 0)
		direction.y = -direction.y;

	direction.Normalize();

	float speed = Extensions::Random(50.0f, 100.0f);
	float spawntime = Extensions::Random(0.0f, 3.0f);
	float lifetime = Extensions::Random(5.0f, 15.0f);

	Fire(pos, direction, speed, spawntime, lifetime);
}

void BulletManager::AddRandomSegment()
{
	float x0 = Extensions::Random(0.0f, windowSize.x);
	float y0 = Extensions::Random(0.0f, windowSize.y);

	float x1 = x0 + Extensions::Random(-20.0f, 20.0f);
	float y1 = y0 + Extensions::Random(-20.0f, 20.0f);

	mxNewWalls.lock();

	newWalls.push_back(Wall{ float2(x0, y0), float2(x1, y1) });

	mxNewWalls.unlock();
}
