// @werat

#include <SDL_image.h>
#include <SDL.h>

#include "SDL_game.h"

SDLGame::SDLGame()
{
}

void SDLGame::OnEvent(SDL_Event* event)
{
   if (event->type == SDL_KEYDOWN)
   {
      if (event->key.keysym.sym == SDLK_ESCAPE)
      {
         Exit();
      }
   }

   App::OnEvent(event);
}

void SDLGame::OnInit()  
{
   width = 800;
   height = 600;

   App::OnInit();

   SDL_SetWindowTitle(window, "Future SDL platform game");

   world.Init();
}
void SDLGame::OnUpdate(float delta) 
{
   world.Update(delta);

   App::OnUpdate(delta);
}
void SDLGame::OnRender(float delta)
{
   SDL_SetRenderDrawColor(renderer, 70, 180, 220, 255);
   SDL_RenderClear(renderer); 

   world.Render(delta, renderer);

   App::OnRender(delta);
}
void SDLGame::OnCleanup()
{
   App::OnCleanup();
}
