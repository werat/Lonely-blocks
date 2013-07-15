// @werat

#include "world.h"
#include "component.h"

#include <SDL.h>

World::~World()
{
   for (auto tile : tiles)
      physicsEngine.DestroyBody(tile);

   delete g_player;
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
               jump_platform->onCollision = [](const CollisionInfo& info) {
                  info.other->velocity.y = -400;
               };

               tiles.push_back(jump_platform);
            }
            if (tiles_map[i][j] == 3)
            {
                cFilter filter = r->filterData();
                filter.categoryBits = 1 << 2;
                filter.IgnoreCategory(1 << 0);
                r->setFilterData(filter);
            }
            if (tiles_map[i][j] == 4)
            {
               platform = r;
               r->setType(r_dynamicBody);
               r->setMass(10);
               r->position += { 10, 10 };
               r->friction = 0.5;
            }
         }
      }
   }

   g_player = new GameObject("player");
   g_player->AddComponent(new PhysicsComponent(&physicsEngine));
   g_player->Init();
   auto pc = g_player->GetComponent<PhysicsComponent>();
   pc->rigidBody->setType(r_dynamicBody);
   pc->rigidBody->position = { 120, 500 };
   pc->rigidBody->half_width = 30 / 2;
   pc->rigidBody->half_height = 50 / 2;
   pc->rigidBody->setMass(5);
   pc->rigidBody->friction = 0.5;

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

   elevator = physicsEngine.CreateBody();
   elevator->position = { 350, 50 };
   elevator->half_width = 50 / 2;
   elevator->half_height = 30 / 2;
   elevator->velocity.y = 100;
   elevator->setType(r_kinematicBody);
   tiles.push_back(elevator); 
}

void World::Update(float delta)
{
   HandleInput();

   g_player->PrePhysicsUpdate(delta);

   physicsEngine.Update(delta);

   g_player->Update(delta);

   UpdatePlayer(delta);
}
void World::Render(float delta, SDL_Renderer *renderer)
{
   for (auto tile : tiles)
   {
      SDL_Rect bounds = Bounds(tile);
      SDL_SetRenderDrawColor(renderer, 100, 75, 10, 255); // brown color for tiles
      SDL_RenderFillRect(renderer, &bounds);
      SDL_SetRenderDrawColor(renderer, 0, 255, 230, 255); // for border
      SDL_RenderDrawRect(renderer, &bounds);
   } 

   SDL_Rect bounds = Bounds(g_player->GetComponent<PhysicsComponent>()->rigidBody);
   if (g_player->GetComponent<PhysicsComponent>()->onGround) SDL_SetRenderDrawColor(renderer, 0, 128, 128, 255); // not red
   else                  SDL_SetRenderDrawColor(renderer, 255, 128, 128, 255); // not red
   SDL_RenderFillRect(renderer, &bounds);
   SDL_SetRenderDrawColor(renderer, 0, 225, 35, 255); // for border
   SDL_RenderDrawRect(renderer, &bounds);

   if (draw_ray)
   {
      for (size_t i = 0; i < points.size() - 1; ++i)
      {
         Vector2 from = points[i];
         Vector2 to = points[i+1];
         SDL_SetRenderDrawColor(renderer, 0, 225, 35, 255); // for border
         SDL_RenderDrawLine(renderer, from.x, from.y, to.x, to.y);

         Vector2 to_normal = to + normals[i] * 10;
         SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
         SDL_RenderDrawLine(renderer, to.x, to.y, to_normal.x, to_normal.y);
      }
   }
}

void World::HandleInput()
{
   input = Vector2::Zero;

   auto state = SDL_GetKeyboardState(NULL);
   if (state[SDL_SCANCODE_LEFT]) input.x = -1;
   if (state[SDL_SCANCODE_RIGHT]) input.x = 1;
   if (state[SDL_SCANCODE_UP]) input.y = -1;
   if (state[SDL_SCANCODE_DOWN]) input.y = 1;
}

void World::UpdatePlayer(float delta)
{
   PhysicsComponent* physics = g_player->GetComponent<PhysicsComponent>();
   
   if (input.y < 0 && physics->onGround) {
      physics->rigidBody->ApplyImpulse({0, -2300});
   }
   if (input.x != 0 ) {
      physics->rigidBody->ApplyForce({5000 * input.x, 0});
   }

   auto state = SDL_GetKeyboardState(NULL);
   if (state[SDL_SCANCODE_J]) platform->ApplyForce({-5000, 0});
   if (state[SDL_SCANCODE_K]) platform->ApplyForce({5000, 0});

   if (moving->position.x - moving->half_width < 0) moving->velocity.x = 100;
   if (moving->position.x + moving->half_width > 800) moving->velocity.x = -100;

   if (elevator->position.y - elevator->half_height < 0) elevator->velocity.y = 100;
   if (elevator->position.y + elevator->half_height > 600) elevator->velocity.y = -100;

   // Test raycast
   int mx, my;
   auto m_state = SDL_GetMouseState(&mx, &my);
   if (m_state & SDL_BUTTON(1))
   {
      points.clear();
      normals.clear();

      Vector2 from = physics->rigidBody->position;
      Vector2 to = { (double)mx, (double)my };

      points.push_back(from);

      RaycastOut out;
      RaycastIn in;
      in.origin = from;
      in.direction = (to - from).Normalized();
      in.categoryMask &= ~(1 << 2);

      for (int i = 0; i < 5; ++i)
      {
         if (physicsEngine.Raycast(in, &out))
         {
            points.push_back(out.contact_point);
            normals.push_back(out.normal);

            in.origin = out.contact_point;

            Vector2 v = in.direction;
            Vector2 reflection = 2 * Dot(v, out.normal) * out.normal - v;
            in.direction = -reflection;
         }
         else
         {
            to = from + in.max_distance * in.direction;
            points.push_back(to);
            break;
         }
      }

      draw_ray = true;
   }
}

SDL_Rect World::Bounds(RigidBody* body)
{
   return SDL_Rect
   {
      (int)std::round(body->position.x - body->half_width),
      (int)std::round(body->position.y - body->half_height),
      (int)std::round(body->width()),
      (int)std::round(body->height())
   };
}
