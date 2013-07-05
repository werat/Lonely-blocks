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
      { 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
      { 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1 },
      { 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
      { 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 1, 0, 0, 3, 0, 1, 0, 1, 0, 0, 0, 1 },
      { 1, 0, 0, 0, 0, 0, 2, 0, 3, 1, 0, 0, 0, 1, 0, 1 },
      { 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1 },
      { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
   };

   for (int i = 0; i < n; ++i)
   {
      for (int j = 0; j < m; ++j)
      {
         if (tiles_map[i][j] > 0)
         {
            double w = 50;
            double h = 30;
            Vector2 center = { w / 2 + w * j, h / 2 + h * i };
            RigidBody* r = new RigidBody(center, w, h);
            r->isStatic = true;
            tiles.push_back(r);
            physicsEngine.AttachRigidBody(r);

            if (tiles_map[i][j] == 2)
            {
               double j_w = 48;
               double j_h = 6;
               Vector2 j_c = { w / 2 + w * j, j_h / 2 + h * i };

               auto jump_platform = new RigidBody(j_c, j_w, j_h);
               jump_platform->isStatic = true;
               jump_platform->onCollision = [](RigidBody* self, RigidBody* other) {
                  other->velocity.y = -400;
                  return true;
               };

               tiles.push_back(jump_platform);
               physicsEngine.AttachRigidBody(jump_platform);
            }
            if (tiles_map[i][j] == 3)
            {
                cFilter filter = r->filterData();
                filter.IgnoreCategory(1 << 0);
                r->setFilterData(filter);
            }
         }
      }
   }
   Vector2 center = { 120, 500 };
   player = RigidBody(center, 30, 60);
   player.inv_mass = 0.1;
   player.restitution = 0.0;
   player.friction = 0.5;
   player.onGround = true;
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

   if (player.onGround) SDL_SetRenderDrawColor(renderer, 0, 128, 128, 255); // not red
   else                 SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // red
   SDL_Rect bounds = player.bounds();
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

   if (state[SDL_SCANCODE_A]) player.position.x -= 0.3;
   if (state[SDL_SCANCODE_D]) player.position.x += 0.3;
}

void World::UpdatePlayer(float delta)
{
   if (input.y < 0) {
      player.velocity.y += -40; // apply jumping force
   }
   if (input.x != 0 ) {
      player.velocity.x += 1000 * delta * input.x;
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