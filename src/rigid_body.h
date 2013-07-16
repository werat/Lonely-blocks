// @werat

#ifndef RIGID_BODY_H
#define RIGID_BODY_H 

#include <cmath>
#include <functional>

#include "common.h"
#include "vector2.h"

// Forward declarations
class PhysicsEngine;
struct CollisionInfo;
class GameObject;

struct cFilter
{
   cFilter()
   {
      categoryBits = 1 << 0; // 0x00000001
      maskBits = 0xFFFFFFFF; // accept all categories
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
public:
   double width() const { return half_width * 2; }
   double height() const { return half_height * 2; }

   rBodyType type() const { return _type; }
   void setType(rBodyType type);

   const cFilter& filterData() const { return _filterData; }
   void setFilterData(const cFilter& filter);

   double mass() const { return 1.0 / inv_mass;}
   void setMass(double mass);

   void ApplyImpulse(const Vector2& impulse);
   void ApplyForce(const Vector2& force);

   bool ShouldCollide(const RigidBody* other);

   bool Intersects(const RigidBody* other, Vector2* resolution = nullptr);
   bool ContainsPoint(const Vector2& point);

private:
   RigidBody();
   ~RigidBody() {}

   void ClearForces();

public:
   // positional properties
   Vector2 position;
   double half_width, half_height;

   // static physics properties
   double inv_mass = 0.0;

   double restitution = 0.0; // should be in [0; 1]
   double friction = 0.3; // should be in [0; 1]

   // linear properties
   Vector2 velocity = Vector2::Zero;

   double gravity_scale = 1.0;

   std::function<void(const CollisionInfo&)> onCollision;

   // can't be null, as rigidbody is always attached to GameObject via PhysicsComponent
   GameObject* gameObject;

private:
   cFilter _filterData;
   rBodyType _type = r_staticBody;

   Vector2 _force = Vector2::Zero;

   DISALLOW_COPY_AND_ASSIGN(RigidBody);

public:
   friend class PhysicsEngine;
};

#endif