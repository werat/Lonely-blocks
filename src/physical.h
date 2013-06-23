// @werat

#ifndef PHYSICAL_H
#define PHYSICAL_H 

#include "vector2.h"

#include <SDL.h>

#include <functional>

// Maybe store a pointer to PhysicsEngine?
// Then we could change isStatic on fly.

class Physical
{
public:
   Vector2 position;
   int width, height;

   Vector2 velocity;
   Vector2 acceleration;

   bool onGround;
   bool atCeiling;

   bool isStatic;

   std::function<bool(Physical&, Physical&)> onCollision;

public:
   Physical(const Vector2& position, int w, int h);
   ~Physical() {}

   SDL_Rect bounds() const  
   { 
      return { (int)(position.x - width / 2), (int)(position.y - height / 2), width, height }; 
   }

   void Translate(const Vector2& delta);

   // TODO: maybe move to physical engine?
   bool Intersects(const Physical& other, Vector2* resolution = nullptr);
};

#endif