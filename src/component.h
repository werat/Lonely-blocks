// @werat

#ifndef COMPONENT_H
#define COMPONENT_H

#include "physics_engine.h"

class GameObject;

class Component
{
public:
   GameObject* gameObject;

public:
   Component() {}
   virtual ~Component() {}

   virtual void Init() {}
   virtual void PrePhysicsUpdate(float delta) {}
   virtual void Update(float delta) {}
};

#endif