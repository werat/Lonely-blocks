// @werat

#include <SDL.h>

#include <vector>

#include "common.h"
#include "game_object.h"
#include "rigid_body.h"
#include "physics_engine.h"
#include "physics_component.h"

class World
{
public:
   World(int width, int height) { this->width = width; this->height = height; }
   World() : World(0, 0) {}
   ~World();

public:
   void Init();
   void Update(float delta);
   void Render(float delta, SDL_Renderer *renderer);

private:
   void HandleInput();
   void UpdatePlayer(float delta);

public:
   int width, height;
   
private:
   PhysicsEngine physicsEngine;
   
   std::vector<RigidBody*> tiles;
   
   RigidBody* platform;
   RigidBody* moving, *elevator;

   GameObject* g_player;

   std::vector<Vector2> points;
   std::vector<Vector2> normals;

   bool draw_ray = false;
   
   Vector2 input;
   bool slow_mo = false;
   bool fast_mo = false;

   DISALLOW_COPY_AND_ASSIGN(World);
};