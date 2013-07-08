// @werat

#ifndef RIGID_BODY_H
#define RIGID_BODY_H 

#include "vector2.h"

#include <SDL.h>
#include <cmath>

#include <functional>

// Forward declarations
class PhysicsEngine;

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

   void AcceptCategory(unsigned int categoryMask) { maskBits |= categoryMask; }
   void IgnoreCategory(unsigned int categoryMask) { maskBits &= ~categoryMask; }
};

// static: zero mass, zero velocity, may be manually moved
// kinematic: zero mass, non-zero velocity set by user, moved by engine
// dynamic: positive mass, non-zero velocity determined by forces, moved by engine
enum rBodyType 
{
   r_staticBody,
   r_kinematicBody,
   r_dynamicBody
};

class RigidBody 
{
   friend class PhysicsEngine;

private:
   PhysicsEngine* _engine;

   cFilter _filterData;
   rBodyType _type = r_staticBody;

   Vector2 _force = Vector2::Zero;

public:
   // positional properties
   Vector2 position;
   double half_width, half_height;

   // static physics properties
   double inv_mass = 0.0;
   double inv_inertia = 1.0;

   double restitution = 0.0; // should be in [0; 1]
   double friction = 0.3; // should be in [0; 1]

   // linear properties
   Vector2 velocity = Vector2::Zero;

   // TODO (werat): somewhere in future we will use this
   // angular properties
   double rotation;
   double angular_velocity;
   double torque;

   double gravity_scale = 1.0;

   std::function<void(RigidBody*, RigidBody*)> onCollision;

protected:
   RigidBody();

   void ClearForces();

public:
   ~RigidBody() {}

   double width() const { return half_width * 2; }
   double height() const { return half_height * 2; }

   // TODO (werat): move this, it's needed for drawing rectangles only
   SDL_Rect bounds() const  
   { 
      return SDL_Rect
      {
         (int)std::round(position.x - half_width), (int)std::round(position.y - half_height),
         (int)std::round(width()), (int)std::round(height())
      }; 
   }

   rBodyType type() const { return _type; }
   void setType(rBodyType type);

   const cFilter& filterData() const { return _filterData; }
   void setFilterData(const cFilter& filter);

   void ApplyImpulse(const Vector2& impulse);
   void ApplyForce(const Vector2& force);

   bool Intersects(const RigidBody* other, Vector2* resolution = nullptr);
   bool ContainsPoint(const Vector2& point);
};

#endif