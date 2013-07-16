// @werat

#include "physics_component.h"

PhysicsComponent::PhysicsComponent()
{
   // _engine = engine;
   rigidBody = Physics().CreateBody();
}

PhysicsComponent::~PhysicsComponent()
{
   Physics().DestroyBody(rigidBody);
}

void PhysicsComponent::Init()
{
   rigidBody->gameObject = gameObject;

   // TODO (werat): move to PlatformController or smth
   rigidBody->onCollision = [&](const CollisionInfo& info)
   {
      // TODO (werat): improve for non-aabb collisions and jumpers
      if (info.normal.y < 0 && info.moveDirection.y > 0)
      {
         onGround = true;
      }
   };
}

void PhysicsComponent::PrePhysicsUpdate(float delta)
{
   onGround = false;
}


void PhysicsComponent::Update(float delta)
{
}