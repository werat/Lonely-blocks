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

   // player = physicsEngine.CreateBody();
   // player->position = { 120, 500 };
   // player->width = 50;
   // player->height = 30;
   // player->inv_mass = 0.2;
   // player->isStatic = false;
   // player->onGround = true;
   // player->restitution = 0.0;
   // player->friction = 0.5;

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
            r->width = w;
            r->height = h;
            r->isStatic = true;

            tiles.push_back(r);

            if (tiles_map[i][j] == 2)
            {
               double j_w = 48;
               double j_h = 6;
               Vector2 j_c = { w / 2 + w * j, j_h / 2 + h * i };

               auto jump_platform = physicsEngine.CreateBody();
               jump_platform->position = j_c;
               jump_platform->width = j_w;
               jump_platform->height = j_h;
               jump_platform->isStatic = true;
               jump_platform->onCollision = [](RigidBody* self, RigidBody* other) {
                  other->velocity.y = -400;
                  return true;
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
               r->position += { 10, 10 };
               r->inv_mass = 0.05;
               r->isStatic = false;
            }
         }
      }
   }

   player = physicsEngine.CreateBody();
   player->position = { 120, 500 };
   player->width = 50;
   player->height = 30;
   player->inv_mass = 0.2;
   player->isStatic = false;
   player->onGround = true;
   player->restitution = 0.0;
   player->friction = 0.5;
   
   RigidBody* floor = physicsEngine.CreateBody();
   floor->position = { 400, 600 - 15};
   floor->width = 800;
   floor->height = 30;
   floor->isStatic = true;
   tiles.push_back(floor);
   
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

   if (player->onGround) SDL_SetRenderDrawColor(renderer, 0, 128, 128, 255); // not red
   else                 SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // red
   SDL_Rect bounds = player->bounds();
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
      player->velocity.y += -40; // apply jumping force
   }
   if (input.x != 0 ) {
      player->velocity.x += 1000 * delta * input.x;
   }

   // if (input.y > 0 ) {
   //    std::cout << "==============================" << std::endl;
   //    std::cout << "Position = " << player.position << std::endl;
   //    std::cout << "Left = " << player.bounds().x << std::endl;
   //    std::cout << "Left calc = " << player.position.x - player.width / 2 << std::endl;
   //    std::cout << "Left calc(int) = " << (int)std::round(player.position.x - player.width / 2) << std::endl;

   //    std::cout << "Position.x = " << player.position.x << std::endl;
   //    std::cout << "width = " << player.width << std::endl;
   //    std::cout << "width / 2 = " << player.width / 2 << std::endl;
   // }

   // player.position.x += input.x * 200 * delta;
}