// @werat

#include <SDL.h>
#include <string>
#include <stdexcept>
#include <sstream>

#include <chrono>
#include <thread>

#include "app.h"
#include "renderer.h"

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

   auto targetElapsedTime = std::chrono::milliseconds(1000 / MaxFPS);
   auto elapsedTimer = std::chrono::high_resolution_clock::now(); 
   auto accumulatedElapsedTimer = std::chrono::milliseconds::zero();
   
   isRunning = true;
   while (isRunning)
   {

   // General idea taken from MonoGame (https://github.com/mono/MonoGame)
   // https://github.com/mono/MonoGame/blob/develop/MonoGame.Framework/Game.cs#L467

   RetryTick:

      // Advance the accumulated elapsed time.
      accumulatedElapsedTimer += std::chrono::duration_cast<std::chrono::milliseconds>(
         std::chrono::high_resolution_clock::now() - elapsedTimer
      );
      elapsedTimer = std::chrono::high_resolution_clock::now();

      if (accumulatedElapsedTimer < targetElapsedTime)
      {
         auto sleepTime = targetElapsedTime - accumulatedElapsedTimer;
         std::this_thread::sleep_for(sleepTime);

         goto RetryTick;
      }

      if (accumulatedElapsedTimer > std::chrono::milliseconds(500))
         accumulatedElapsedTimer = std::chrono::milliseconds(500);

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
         OnUpdate(targetElapsedTime.count() / 1000.0); // in seconds
      }

      // auto renderInterpolationFactor = accumulatedElapsedTimer / targetElapsedTime;
      auto totalElapsedTime = targetElapsedTime * stepCount;
      OnRender(totalElapsedTime.count() / 1000.0); // in seconds
   }

   OnCleanup();
}

void App::Exit()
{
   isRunning = false;
}

// Implementation taken from SDL2_gfx library (http://sourceforge.net/projects/sdl2gfx/)
// http://sourceforge.net/p/sdl2gfx/code/5/tree/trunk/test/common.c#l1003
void App::TakeScreenshot(std::string path)
{
   SDL_Rect viewport;
   SDL_Surface *surface;

   SDL_RenderGetViewport(renderer, &viewport);
   surface = SDL_CreateRGBSurface(0, viewport.w, viewport.h, 24,
   #if SDL_BYTEORDER == SDL_LIL_ENDIAN
                 0x00FF0000, 0x0000FF00, 0x000000FF,
   #else
                 0x000000FF, 0x0000FF00, 0x00FF0000,
   #endif
                 0x00000000);

   if (!surface) {
      fprintf(stderr, "Couldn't create surface: %s\n", SDL_GetError());
      return;
   }

   if (SDL_RenderReadPixels(renderer, NULL, surface->format->format,
                            surface->pixels, surface->pitch) < 0) {
      fprintf(stderr, "Couldn't read screen: %s\n", SDL_GetError());
      return;
   }

   if (path.empty())
   {
      std::ostringstream s;
      s << "screenshot" << SDL_GetTicks() << ".bmp";
      path = std::string(s.str());
   }

   if (SDL_SaveBMP(surface, path.c_str()) < 0) {
      fprintf(stderr, "Couldn't save screenshot.bmp: %s\n", SDL_GetError());
      return;
   }
   SDL_FreeSurface(surface);
}