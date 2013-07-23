// @werat

#ifndef SCENE_H
#define SCENE_H

#include <SDL.h>
#include <vector>
// #include <string>

#include "common.h"
#include "game_object.h"
#include "physics_engine.h"

class Scene
{
public:
   Scene() {}
   virtual ~Scene() {}

   PhysicsEngine& physics() { return _physics; }

   virtual void Init();
   virtual void Destroy();

   virtual void Update(float delta);
   virtual void Render(float delta, SDL_Renderer *renderer);

   void Add(GameObject* object);
   void Remove(GameObject* object);

protected:
   std::vector<GameObject*> _gameObjects;

private:
   bool _initialized = false;
   bool _destroyed = false;

   PhysicsEngine _physics;
   
   DISALLOW_COPY_AND_ASSIGN(Scene);
};

#endif   