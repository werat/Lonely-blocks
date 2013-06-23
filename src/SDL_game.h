// @werat

#ifndef SDL_GAME_H
#define SDL_GAME_H

#include "app.h"
#include "physical.h"
#include "world.h"

#include <SDL.h>

#include <vector>

class SDLGame : public App
{
private:
   World world;

public:
   SDLGame();
   virtual ~SDLGame() {}

protected:
   virtual void OnInit();
   virtual void OnEvent(SDL_Event* event);
   virtual void OnUpdate(float delta);
   virtual void OnRender(float delta);
   virtual void OnCleanup();
};

#endif