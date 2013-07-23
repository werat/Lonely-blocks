// @werat

#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

#include "component.h"
#include "rigid_body.h"

class PhysicsComponent : public Component
{
public:
   virtual void Init() override;
   virtual void PrePhysicsUpdate(float delta) override;
   virtual void Update(float delta) override;

public:
   bool onGround;
};

#endif