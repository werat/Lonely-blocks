// @werat

#ifndef COMPONENT_H
#define COMPONENT_H

#include <SDL.h>

#include "common.h"
#include "physics_engine.h"

class GameObject;

class Component
{
public:
   Component() {}
   virtual ~Component() {}

   GameObject& gameObject() { return *_gameObject; }
   PhysicsEngine& physics();

   virtual void Init() {}
   virtual void Destroy() {}

   virtual void PrePhysicsUpdate(float delta) {}
   virtual void Update(float delta) {}
   virtual void Render(float delta, SDL_Renderer *renderer) {}
   
private:
   GameObject* _gameObject;
   
   DISALLOW_COPY_AND_ASSIGN(Component);

public:
   friend class GameObject;
};

#endif