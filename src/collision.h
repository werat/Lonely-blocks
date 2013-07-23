// @werat

#ifndef COLLISION_H
#define COLLISION_H

#include "vector2.h"

class RigidBody;

struct ContactData
{
   RigidBody* first;
   RigidBody* second;

   Vector2 normal;
   double penetration;

   bool operator==(const ContactData& other)
   {
      return (first == other.first && second == other.second)
          || (first == other.second && second == other.first);
   }
};

struct CollisionInfo
{
   RigidBody* self;
   RigidBody* other;

   Vector2 normal;
   Vector2 moveDirection;
};

struct _Internal_RaycastIn
{
   Vector2 origin;
   Vector2 direction;
   double max_distance;
};
struct _Internal_RaycastOut
{
   double distance;
   Vector2 normal;
};

#endif