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

            RigidBody* r = physicsEngine.CreateBody();
            r->position = center;
            r->half_width = w / 2;
            r->half_height = h / 2;
            r->setType(r_staticBody);

            tiles.push_back(r);

            if (tiles_map[i][j] == 2)
            {
               double j_w = 48;
               double j_h = 6;
               Vector2 j_c = { w / 2 + w * j, j_h / 2 + h * i };

               auto jump_platform = physicsEngine.CreateBody();
               jump_platform->position = j_c;
               jump_platform->half_width = j_w / 2;
               jump_platform->half_height = j_h / 2;
               jump_platform->setType(r_staticBody);
               jump_platform->onCollision = [](RigidBody* self, RigidBody* other) {
                  other->velocity.y = -400;
               };

               tiles.push_back(jump_platform);
            }
            if (tiles_map[i][j] == 3)
            {
                cFilter filter = r->filterData();
                filter.IgnoreCategory(1 << 0);
                r->setFilterData(filter);
            }
            if (tiles_map[i][j] == 4)
            {
               platform = r;
               r->position += { 10, 10 };
               r->inv_mass = 1.0 / 5;
               r->friction = 0.5;
               r->setType(r_dynamicBody);
            }
         }
      }
   }

   player = physicsEngine.CreateBody();
   player->position = { 120, 500 };
   player->half_width = 30 / 2;
   player->half_height = 50 / 2;
   player->inv_mass = 1.0 / 5;
   player->friction = 0.5;
   player->setType(r_dynamicBody);

   RigidBody* floor = physicsEngine.CreateBody();
   floor->position = { 400, 600 - 15};
   floor->half_width = 800 / 2;
   floor->half_height = 30 / 2;
   floor->setType(r_staticBody);
   tiles.push_back(floor);

   moving = physicsEngine.CreateBody();
   moving->position = { 260, 300 };
   moving->half_width = 100 / 2;
   moving->half_height = 30 / 2;
   moving->velocity.x = 100;
   moving->setType(r_kinematicBody);
   tiles.push_back(moving);   
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

   SDL_Rect bounds = player->bounds();
   SDL_SetRenderDrawColor(renderer, 0, 128, 128, 255); // not red
   SDL_RenderFillRect(renderer, &bounds);
   SDL_SetRenderDrawColor(renderer, 0, 225, 35, 255); // for border
   SDL_RenderDrawRect(renderer, &bounds);
}

void World::HandleInput()
{
   input = Vector2::Zero;

   auto state = SDL_GetKeyboardState(NULL);
   if (state[SDL_SCANCODE_LEFT]) input.x = -1;
   if (state[SDL_SCANCODE_RIGHT]) input.x = 1;
   if (state[SDL_SCANCODE_UP]) input.y = -1;
   if (state[SDL_SCANCODE_DOWN]) input.y = 1;

   if (state[SDL_SCANCODE_A]) player->position.x -= 0.3;
   if (state[SDL_SCANCODE_D]) player->position.x += 0.3;
}

void World::UpdatePlayer(float delta)
{
   if (input.y < 0) {
      // player->velocity.y += -40; // apply jumping force
      player->ApplyForce({0, -10000});
   }
   if (input.x != 0 ) {
      // player->velocity.x += 1000 * delta * input.x;
      player->ApplyForce({5000 * input.x, 0});
   }

   auto state = SDL_GetKeyboardState(NULL);
   if (state[SDL_SCANCODE_J]) platform->ApplyForce({-5000, 0});
   if (state[SDL_SCANCODE_K]) platform->ApplyForce({5000, 0});

   if (moving->position.x - moving->half_width < 0) moving->velocity.x = 100;
   if (moving->position.x + moving->half_width > 800) moving->velocity.x = -100;
}