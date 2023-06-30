#pragma once

#include <iostream>

#include "SDL_ttf.h"
#include <memory>
#include "BulletManager/BulletManager.h"
#include "Tests/ITest.h"

struct SDL_Window;
struct SDL_Renderer;
class BulletManager;

class App
{
	// sdl

	SDL_Window* window = nullptr; 
	SDL_Renderer* renderer = nullptr;
	TTF_Font* font = nullptr;

	// time

	clock_t tCurrentFrame = NULL;
	float fpsTimer = NULL;
	float dt = NULL;
	int fpsCounter = NULL;
	int fps = NULL;

	//

	std::shared_ptr<BulletManager> bulletManager;
	std::unique_ptr<ITest> test;

	//

	bool Init();
	void Loop();
	void Shutdown();
	void DrawHint();
	void UpdateTime();

public:

	App();
	void Run();

};

