// @werat

#include "world.h"

#include <SDL.h>

World::~World()
{
   for (auto tile : tiles) delete tile;
}

void World::Init()
{
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
      { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 1 },
      { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
      { 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1 },
      { 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
      { 1, 0, 0, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 1, 0, 0, 0, 0, 4, 0, 1, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 0, 0, 2, 0, 0, 4, 0, 0, 0, 1, 0, 1 },
      { 1, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 1 },
      { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
   };

   for (int i = 0; i < n; ++i)
   {
      for (int j = 0; j < m; ++j)
      {
         if (tiles_map[i][j] > 0)
         {
            int w = 50;
            int h = 30;
            Vector2 center = { (float)(w / 2 + w * j), (float)(h / 2 + h * i) };
            RigidBody* r = new RigidBody(center, w, h);
            r->isStatic = true;
            tiles.push_back(r);
            physicsEngine.AttachRigidBody(r);

            if (tiles_map[i][j] == 2)
            {
               r->onCollision = [](RigidBody& self, RigidBody& other) {
                  other.velocity.y = -400;
                  return true;
               };
            }
            if (tiles_map[i][j] == 3)
            {
               r->onCollision = [](RigidBody& self, RigidBody& other) {
                  self.isStatic = false;
                  return true;
               };
            }
            if (tiles_map[i][j] == 4)
            {
               r->isStatic = false;
            }
         }
      }
   }
   Vector2 center = { 120, 500 };
   player = RigidBody(center, 30, 60);
   physicsEngine.AttachRigidBody(&player);
}
void World::Update(float delta)
{
   HandleInput();
   UpdatePlayer(delta);
   physicsEngine.Update(delta);
}
void World::Render(float delta, SDL_Renderer *renderer)
{
   for (auto tile : tiles)
   {
      SDL_Rect bounds = tile->bounds();
      SDL_SetRenderDrawColor(renderer, 100, 75, 10, 255); // brown color for tiles
      SDL_RenderFillRect(renderer, &bounds);
      SDL_SetRenderDrawColor(renderer, 0, 255, 230, 255); // for border
      SDL_RenderDrawRect(renderer, &bounds);
   } 

   if (!player.onGround) SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // red
   else                  SDL_SetRenderDrawColor(renderer, 0, 128, 128, 255); // not red
   SDL_Rect bounds = player.bounds();
   SDL_RenderFillRect(renderer, &bounds);
}

void World::HandleInput()
{
   input = Vector2::Zero;

   auto state = SDL_GetKeyboardState(NULL);
   if (state[SDL_SCANCODE_LEFT]) input.x = -1;
   if (state[SDL_SCANCODE_RIGHT]) input.x = 1;
   if (state[SDL_SCANCODE_UP]) input.y = -1;
   if (state[SDL_SCANCODE_DOWN]) input.y = 1;

   if (state[SDL_SCANCODE_A]) player.position.x -= 0.3;
   if (state[SDL_SCANCODE_D]) player.position.x += 0.3;
}

void World::UpdatePlayer(float delta)
{
   if (input.y < 0 &&  player.onGround) {
      player.velocity.y = -400; // jump
   }

   player.position.x += input.x * 200 * delta;
}