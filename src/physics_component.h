// @werat

#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

#include "component.h"
#include "rigid_body.h"
#include "physics_engine.h"

class PhysicsComponent : public Component
{
private:
   PhysicsEngine* _engine;

public:
   RigidBody* rigidBody;
   bool onGround;

public:
   PhysicsComponent(PhysicsEngine* engine);
   virtual ~PhysicsComponent();

   virtual void Init();

   virtual void PrePhysicsUpdate(float delta);
   virtual void Update(float delta);
};

#endif