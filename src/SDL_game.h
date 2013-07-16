// @werat

#ifndef SDL_GAME_H
#define SDL_GAME_H

#include <SDL.h>

#include <vector>

#include "app.h"
#include "world.h"

class SDLGame : public App
{
public:
   SDLGame();
   virtual ~SDLGame() {}

protected:
   virtual void OnInit() override;
   virtual void OnEvent(SDL_Event* event) override;
   virtual void OnUpdate(float delta) override;
   virtual void OnRender(float delta) override;
   virtual void OnCleanup() override;
   
private:
   World world;
};

#endif