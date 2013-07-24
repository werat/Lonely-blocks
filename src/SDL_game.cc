// @werat

#include <SDL_image.h>
#include <SDL.h>

#include "SDL_game.h"

#include "scene.h"
#include "game_object.h"
#include "rigid_body.h"
#include "physics_component.h"
#include "block_renderer.h"

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

   const int n = 20;
   const int m = 16;
   int tiles_map[n][m] = 
   {
      { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
      { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1 },
      { 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1 },
      { 1, 0, 1, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 1 },
      { 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1 },
      { 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
      { 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 1, 0, 0, 3, 0, 0, 0, 1, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 1, 0, 1 },
      { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
      { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
   };

   for (int i = 0; i < n-1; ++i)
   {
      for (int j = 0; j < m; ++j)
      {
         if (tiles_map[i][j] > 0)
         {
            double w = 50;
            double h = 30;
            Vector2 center = { w / 2 + w * j, h / 2 + h * i };

            auto obj = new GameObject(&scene, "solid_tile");
            auto r = obj->AddComponent<RigidBody>();
            r->position = center;
            r->half_width = w / 2;
            r->half_height = h / 2;
            r->setType(r_staticBody);
            auto b = obj->AddComponent<BlockRenderer>();
            b->r = 100; b->g = 75; b->b = 10;

            if (tiles_map[i][j] == 2)
            {
               double j_w = 48;
               double j_h = 6;
               Vector2 j_c = { w / 2 + w * j, j_h / 2 + h * i - 3 };

               auto jumpy = new GameObject(obj, "jumpy_tile");
               auto r = jumpy->AddComponent<RigidBody>();
               r->position = j_c;
               r->half_width = j_w / 2;
               r->half_height = j_h / 2;
               r->setType(r_staticBody);
               r->onCollision = [](const CollisionInfo& info) {
                  info.other->velocity.y = -600;
               };
               auto b = jumpy->AddComponent<BlockRenderer>();
               b->r = 10; b->g = 125; b->b = 220;
            }
            if (tiles_map[i][j] == 4)
            {
               r->setType(r_dynamicBody);
               r->setMass(5);
               r->friction = 0.5;
            }
         }
      }
   }

   auto obj = new GameObject(&scene, "player");
   auto r = obj->AddComponent<RigidBody>();
   r->setType(r_dynamicBody);
   r->position = { 120, 500 };
   r->half_width = 30 / 2;
   r->half_height = 50 / 2;
   r->setMass(5);
   r->friction = 0.5;
   obj->AddComponent<PhysicsComponent>();
   obj->AddComponent<BlockRenderer>();

   obj = new GameObject(&scene, "floor");
   r = obj->AddComponent<RigidBody>();
   r->position = { 400, 600 - 15};
   r->half_width = 800 / 2;
   r->half_height = 30 / 2;
   r->setType(r_staticBody);
   auto b = obj->AddComponent<BlockRenderer>();
   b->r = 100; b->g = 75; b->b = 10;

   scene.Init();
}
void SDLGame::OnUpdate(float delta) 
{
   scene.Update(delta);

   App::OnUpdate(delta);
}
void SDLGame::OnRender(float delta)
{
   SDL_SetRenderDrawColor(renderer, 70, 180, 220, 255);
   SDL_RenderClear(renderer); 

   scene.Render(delta, renderer);

   App::OnRender(delta);
}
void SDLGame::OnCleanup()
{
   scene.Destroy();
   
   App::OnCleanup();
}
