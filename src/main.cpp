#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Input.hpp"
#include "Time.hpp"

#include <string>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main(int argc, char* argv[])
{
    SDL_Log("SDL init...");

    bool useDeltaTime = false;

    if(argc > 1)
    {
        useDeltaTime = (strcmp(argv[1], "-dt") == 0);
        SDL_Log("Flag=%s, UseDeltaTime=%s", argv[1], useDeltaTime ? "true" : "false");
    }        

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "sdl init failed with error: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
    int imgInitted = IMG_Init(imgFlags);

    if((imgInitted&imgFlags) != imgFlags)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "sdl img init failed with error: %s", IMG_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window* window = SDL_CreateWindow("SDL2 Gravity", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    if(window == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "sdl create window failed with error: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if(renderer == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "sdl create renderer failed with error: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    float x = (WINDOW_WIDTH / 2) - 25;
    float y = 0.0;

    SDL_Rect object = { (int)x, (int)y, 50, 50 };
    SDL_FPoint speed = { 1.0f, 1.0f };
    SDL_FPoint gravity = { 0.0f, 1.0f };

    float frameTime = 0;
    float lastFrameTime = 0;
    float deltaTime = 1.5f;

    Time* time = new Time();

    std::string title = "";

    Time* started = new Time();

    while(Input::Instance()->Listen())
    {
        Input::Instance()->Demo();

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_TRANSPARENT);
        SDL_RenderClear(renderer);

        if (useDeltaTime)
            y += (speed.y * deltaTime);
        else
            y += speed.y;

        object.y = y;

        if(object.y + 50 >= WINDOW_HEIGHT)
        {
            y = WINDOW_HEIGHT - 50;
            speed.y *= -1;
            Time* finished = new Time();
            double elpased = Time::Since(started);
            SDL_Log("Started=%s, Finished=%s, Elapsed=%lf", started->String().c_str(), finished->String().c_str(), elpased);
            if(elpased >= 10)
                break;
        }

        if(useDeltaTime)
            speed.y += (gravity.y * deltaTime);
        else
            speed.y += gravity.y;

        title = "SDL2 Gravity | ";
        title += time->String() + " | ";
        title += "FrameTime=" + std::to_string(frameTime) + " | ";
        title += "DeltaTime=" + std::to_string(deltaTime) + " | ";
        title += "Y=" + std::to_string(y);
        
        SDL_SetWindowTitle(window, title.c_str());
        time = new Time();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_TRANSPARENT);
        SDL_RenderFillRect(renderer, &object);

        SDL_RenderPresent(renderer);

        frameTime = SDL_GetTicks() - lastFrameTime;
        lastFrameTime = SDL_GetTicks();
        
        deltaTime = frameTime * (60.0f / 1000.0f);
        if(deltaTime > 1.5f)
            deltaTime = 1.5f;

        if(!useDeltaTime)
            SDL_Delay(16.67);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    IMG_Quit();
    SDL_Quit();

    SDL_Log("SDL quit.");

    return EXIT_SUCCESS;
}
