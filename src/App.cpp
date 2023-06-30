#include "App.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_net.h"
#include "SDL_ttf.h"
#include <vector>
#include <format>
#include "Tests/StabilityTest.h"
#include "Tests/PerfomanceTest1.h"
#include "Tests/PerfomanceTest2.h"

App::App()
{
    bulletManager = std::make_shared<BulletManager>();
}

bool App::Init()
{
    bool result = true;

    SDL_Init(SDL_INIT_VIDEO | SDL_RENDERER_PRESENTVSYNC);

    window = SDL_CreateWindow("Bullet Manager", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!window)
    {
        result = false;
        std::cout << "ERROR : SDL_CreateWindow" << std::endl;
    }

    //auto bla = SDL_GetNumRenderDrivers();

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        result = false;
        std::cout << "ERROR : SDL_CreateRenderer" << std::endl;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

    /*SDL_RendererInfo info = {};
    SDL_GetRendererInfo(renderer, &info);*/

    if (TTF_Init() != 0)
    {
        result = false;
        std::cout << "ERROR : TTF_Init" << std::endl;
    }

    font = TTF_OpenFont("assets/arial.ttf", 15);

    if (font == nullptr)
    {
        result = false;
        std::cout << "ERROR : TTF_OpenFont" << std::endl;
    }

    return result;
}

void App::Loop()
{
    bool runLoop = true;

    int currentTest = 0;
    int nextTest = 0;

    tCurrentFrame = clock();

    SDL_Event event;
    while (runLoop)
    {
        bool runTest = false;

        // Process input for switching between tests
        {
            while (SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                case SDL_QUIT: // Close window
                    runLoop = false;
                    break;
                case SDL_KEYDOWN:
                {
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_1:
                        nextTest = 1;
                        runTest = true;
                        break;
                    case SDLK_2:
                        nextTest = 2;
                        runTest = true;
                        break;
                    case SDLK_3:
                        nextTest = 3;
                        runTest = true;
                        break;
                    case SDLK_0:
                        nextTest = 0;
                        break;
                    case SDLK_ESCAPE:
                        runLoop = false;
                        break;
                    case SDLK_SPACE:
                        bulletManager->Reset();
                        break;
                    default:
                        break;
                    }
                    break; // from keydown
                }
                default:
                    break;
                }
            }

        }

        // Test manage
        {
            if (currentTest != nextTest)
            {
                currentTest = nextTest;

                test = nullptr; // shutdown first

                // change test but keep same bullet manager

                if (nextTest == 1)
                {
                    test = std::make_unique<StabilityTest>(bulletManager);
                    //test->SetBulletManager(bulletManager);
                }
                else if (nextTest == 2)
                {
                    test = std::make_unique<PerfomanceTest1>(bulletManager);
                    //test->SetBulletManager(bulletManager);
                }
                else if (nextTest == 3)
                {
                    test = std::make_unique<PerfomanceTest2>(bulletManager);
                    //test->SetBulletManager(bulletManager);
                }
            }

            if (test != nullptr)
            {
                if (runTest)
                    test->Run();

                test->SyncBackgroundThread();
            }
        }

        // Update
        if (dt > 0)
        {
            bulletManager->Update(dt);
        }

        // SDL render
        {
            int w, h;
            SDL_GetWindowSize(window, &w, &h);
            bulletManager->SetWindowSize(w, h);

            //

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            bulletManager->Render(renderer);

            DrawHint();

            SDL_RenderPresent(renderer);
        }

        UpdateTime();

    }
}

void App::Shutdown()
{
    TTF_CloseFont(font);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}

void App::DrawHint()
{
    // Draw Text & FPS

    {
        SDL_Color cText = { 255, 255, 255, 255 };

        std::vector<std::string> hints;

        hints.push_back("Press next key to run a test:");
        hints.push_back("[1] +20 Bullets each frame");
        hints.push_back("[2] +100 Bullets +100 Walls");
        hints.push_back("[3] +1000 Bullets +1000 Walls");
        hints.push_back("[Space] Reset Bullet Manager");

        //hints.push_back("");
        hints.push_back(std::format("FPS: {} Bullets: {}/{} Segments: {}",
            fps,
            bulletManager->GetBulletsCount(), bulletManager->GetNewBulletsCount(),
            bulletManager->GetWallsCount()));

        //hints.push_back("[1] for MT Stability Stress Testing");

        const float kOffset = 20.0f;
        float textY = kOffset;
        for (auto line : hints)
        {
            SDL_Surface* text = TTF_RenderText_Blended(font, line.c_str(), cText);
            SDL_Rect textRect = { 0, 0, text->w, text->h };
            SDL_Rect destRect = { kOffset, textY, text->w, text->h };
            textY += text->h;

            SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text);

            SDL_RenderCopy(renderer, text_texture, &textRect, &destRect);

            SDL_DestroyTexture(text_texture);
            SDL_FreeSurface(text);
        }
    }
}

void App::UpdateTime()
{
    ++fpsCounter;

    auto tFrame = clock();

    dt = (tFrame - tCurrentFrame) / (float)(CLOCKS_PER_SEC);

    fpsTimer += dt;
    tCurrentFrame = tFrame;

    if (fpsTimer >= 1.0f)
    {
        fpsTimer -= 1.0f;

        fps = fpsCounter;
        fpsCounter = 0;
    }
}

void App::Run()
{
    if (Init())
    {
        Loop();
    }

    Shutdown();
}
