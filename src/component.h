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

   // TODO (werat): make GameObject have reference to Scene
   // and from Scene we can access any other objects such as Physics Engine
   static void Initialize(PhysicsEngine* engine);

   virtual void Init() {}
   virtual void PrePhysicsUpdate(float delta) {}
   virtual void Update(float delta) {}
   virtual void Render(float delta, SDL_Renderer *renderer) {}
   
public:
   GameObject* gameObject;

   static PhysicsEngine& Physics();

private:
   static PhysicsEngine* _physicsEngine;
   static bool _global_initialized;

   DISALLOW_COPY_AND_ASSIGN(Component);
};

#endif