// @werat

#ifndef RIGID_BODY_H
#define RIGID_BODY_H 

#include "vector2.h"

#include <SDL.h>

#include <functional>

struct cFilter
{
   cFilter()
   {
      categoryBits = 1 << 0; // 0x00000001
      maskBits = ~0;         // 0xFFFFFFFF
      groupIndex = 0;
   }

   // The collision category bits. Normally you would just set one bit.
   unsigned int categoryBits;

   // The collision mask bits. This states the categories that this
   // shape would accept for collision.
   unsigned int maskBits;

   // Collision groups allow a certain group of objects to never collide (negative)
   // or always collide (positive). Zero means no collision group. Non-zero group
   // filtering always wins against the mask bits.
   int groupIndex;

   void AcceptCategory(unsigned int category) { maskBits |= category; }
   void IgnoreCategory(unsigned int category) { maskBits &= ~category; }
};

// TODO (werat): store a pointer to PhysicsEngine ->
//             - raycasting
//             - dynamic change of static/dynamic

class RigidBody 
{
protected:
   cFilter _filterData;

public:
   // positional properties
   Vector2 position;
   int width, height;

   // static physics properties
   double inv_mass = 0.0;
   double restitution = 1.0; // should be in [0; 1]
   double static_friction = 0.1; // should be in [0; 1]
   double dynamic_friction = 0.1; // super-friction

   // dynamic physics properties
   Vector2 velocity = Vector2::Zero;
   Vector2 acceleration = Vector2::Zero;

   bool onGround;
   bool atCeiling;

   bool isStatic = false;
   bool isGravityApplied = true;

   std::function<bool(RigidBody*, RigidBody*)> onCollision;

public:
   RigidBody();
   RigidBody(const Vector2& position, int w, int h);
   ~RigidBody() {}

   SDL_Rect bounds() const  
   { 
      return SDL_Rect { (int)std::round(position.x - width / 2), (int)std::round(position.y - height / 2), width, height }; 
   }

   const cFilter& filterData() const { return _filterData; }
   void setFilterData(const cFilter& filter);

   bool Intersects(const RigidBody* other, Vector2* resolution = nullptr);
};

#endif