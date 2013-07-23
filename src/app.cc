// @werat

#include <string>
#include <stdexcept>
#include <sstream>

#include <SDL.h>

#include "app.h"
#include "renderer.h"

#define FORMAT  SDL_PIXELFORMAT_ARGB8888
#define AMASK   0xff000000 // < Alpha bit mask.
#define RMASK   0x00ff0000 // < Red bit mask.
#define GMASK   0x0000ff00 // < Green bit mask.
#define BMASK   0x000000ff // < Blue bit mask.

void App::OnInit()
{
   if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
      throw std::runtime_error("SDL Init failed: " + std::string(SDL_GetError()));
   }
   SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_OPENGL, &window, &renderer);
   if (window == nullptr) {
      throw std::runtime_error("SDL CreateWindow failed: " + std::string(SDL_GetError()));
   }
   if (renderer == nullptr) {
      throw std::runtime_error("SDL CreateRenderer failed: " + std::string(SDL_GetError()));
   }
}

void App::OnEvent(SDL_Event* event)
{
   if (event->type == SDL_QUIT)
   {
      Exit();
   }
}
void App::OnUpdate(float delta)
{
}
void App::OnRender(float delta)
{
   SDL_RenderPresent(renderer);
}
void App::OnCleanup()
{
   SDL_DestroyRenderer(renderer);
   SDL_DestroyWindow(window);
   SDL_Quit();
}

App::App()
{
   this->width = 800;
   this->height = 600;

   this->MaxFPS = 60;
}

void App::Run()
{
   OnInit();

   SDL_Event Event;

   float targetElapsedTime = 1000.0 / MaxFPS;
   float elapsedTimer = SDL_GetTicks();
   float accumulatedElapsedTimer = 0;

   isRunning = true;
   while (isRunning)
   {

   RetryTick:

      // Advance the accumulated elapsed time.
      accumulatedElapsedTimer += SDL_GetTicks() - elapsedTimer;
      elapsedTimer = SDL_GetTicks();

      // If we're in the fixed timestep mode and not enough time has elapsed
      // to perform an update we sleep off the the remaining time
      if (accumulatedElapsedTimer < targetElapsedTime)
      {
         int sleepTime = targetElapsedTime - accumulatedElapsedTimer;
         SDL_Delay(sleepTime);

         goto RetryTick;
      }

      // if (accumulatedElapsedTimer > targetElapsedTime*2) accumulatedElapsedTimer = targetElapsedTime*2;
      if (accumulatedElapsedTimer > 500) accumulatedElapsedTimer = 500;

      int stepCount = 0;
      // Perform as many full fixed length time steps as we can.
      while (accumulatedElapsedTimer >= targetElapsedTime)
      {
         accumulatedElapsedTimer -= targetElapsedTime;
         ++stepCount;

         while (SDL_PollEvent(&Event))
         {
            OnEvent(&Event);
         }
         OnUpdate(targetElapsedTime / 1000.0); // in seconds
      }

      int totalElapsedTime = targetElapsedTime * stepCount;
      OnRender(totalElapsedTime / 1000.0); // in seconds
   }

   OnCleanup();
}

void App::Exit()
{
   isRunning = false;
}

void App::TakeScreenshot(std::string path)
{
   unsigned char pixels[width * height * 4];
   SDL_Rect rect = { 0, 0, width, height };

   SDL_RenderReadPixels(renderer, &rect, FORMAT, pixels, width * 4);
   SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(pixels, width, height, 32, width * 4,
                                                   RMASK, GMASK, BMASK, AMASK);

   if (path.empty())
   {
      std::ostringstream s;
      s << "screenshot" << SDL_GetTicks() << ".bmp";
      path = std::string(s.str());
   }

   SDL_SaveBMP(surface, path.c_str());
   SDL_FreeSurface(surface);
}
