// @werat

#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

#include "rigid_body.h"

#include <vector>

class PhysicsEngine
{
private:
   // _steps - number of steps we take during one usual Update(delta)
   // dt would be (delta / _steps) if delta is fixed
   const int _steps = 5;

   std::vector<RigidBody*> _rigidBodys;

public:
   Vector2 gravity;
public:
   PhysicsEngine();
   ~PhysicsEngine();

   // TODO: very dangerous as rigidBody can be soon destroyed and pointer would be invalid
   void AttachRigidBody(RigidBody* rigidBody);
   void DetachRigidBody(RigidBody* rigidBody);

   void Update(float delta);

private:
   void ResolveXThenY(RigidBody& rigidBody, std::vector<RigidBody*>& c_rigidBodys, const Vector2& resolution);
   void ResolveYThenX(RigidBody& rigidBody, std::vector<RigidBody*>& c_rigidBodys, const Vector2& resolution);
   void UpdateRigidBodyState(RigidBody& rigidBody, const Vector2& resolution);
};

#endif