// @werat

#include "rigid_body.h"
#include "physics_component.h"

PhysicsComponent::PhysicsComponent()
{
   _rigidBody = Physics().CreateBody();
}

PhysicsComponent::~PhysicsComponent()
{
   Physics().DestroyBody(_rigidBody);
}

void PhysicsComponent::Init()
{
   _rigidBody->_gameObject = gameObject;

   // TODO (werat): move to PlayerController or smth
   _rigidBody->onCollision = [&](const CollisionInfo& info)
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