// @werat

#include "rigid_body.h"
#include "physics_engine.h"

#include <SDL.h>

#include <vector>

class World
{
private:
   PhysicsEngine physicsEngine;
   
   std::vector<RigidBody*> tiles;
   RigidBody* player;
   RigidBody* platform;
   
   Vector2 input;

public:
   int width, height;
      
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
};