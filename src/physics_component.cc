// @werat

#include "physics_component.h"

PhysicsComponent::PhysicsComponent(PhysicsEngine* engine)
{
   _engine = engine;
   rigidBody = _engine->CreateBody();
}

PhysicsComponent::~PhysicsComponent()
{
   _engine->DestroyBody(rigidBody);
}

void PhysicsComponent::Init()
{
   rigidBody->gameObject = gameObject;
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