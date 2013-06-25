// @werat

#ifndef RIGID_BODY_H
#define RIGID_BODY_H 

#include "vector2.h"

#include <SDL.h>

#include <functional>

// Maybe store a pointer to PhysicsEngine?
// Then we could change isStatic on fly.

class RigidBody 
{
public:
   Vector2 position;
   int width, height;

   Vector2 velocity;
   Vector2 acceleration;

   bool onGround;
   bool atCeiling;

   bool isStatic;
   bool isGravityApplied;

   std::function<bool(RigidBody&, RigidBody&)> onCollision;

public:
   RigidBody();
   RigidBody(const Vector2& position, int w, int h);
   ~RigidBody() {}

   SDL_Rect bounds() const  
   { 
      return { (int)(position.x - width / 2), (int)(position.y - height / 2), width, height }; 
   }

   void Translate(const Vector2& delta);

   // TODO: maybe move to physics engine?
   bool Intersects(const RigidBody& other, Vector2* resolution = nullptr);
};

#endif