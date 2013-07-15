// @werat

#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

#include "rigid_body.h"

#include <cmath>
#include <vector>

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

struct RaycastIn
{
   Vector2 origin;
   Vector2 direction;
   double max_distance = 0.0; // 0.0 or less the for infinite ray
   unsigned int categoryMask = 0xFFFFFFFF; // accept all categoties 
};
struct RaycastOut
{
   RigidBody* body;
   Vector2 contact_point;
   Vector2 normal;
   double distance;
};

class PhysicsEngine
{
private:
   // _steps - number of steps we take during one usual Update(delta)
   // dt would be (delta / _steps) if delta is fixed
   int _steps = 5;

   std::vector<RigidBody*> _rigidBodies;

public:
   Vector2 gravity;
   
public:
   PhysicsEngine();
   ~PhysicsEngine();

   RigidBody* CreateBody();
   void DestroyBody(RigidBody* rigidBody);

   bool Raycast(const RaycastIn& input, RaycastOut* output = nullptr);

   void Update(float delta);

private:
   ContactData CreateContactData(RigidBody* first, RigidBody* second);
   void ResolveContact(const ContactData& contact);
   void IntegrateLinearProperties(RigidBody* body, float dt);
   void CorrectPosition(const ContactData& contact);

   double MixFriction(double f1, double f2);
   double MixRestitution(double r1, double r2);
};

inline double PhysicsEngine::MixFriction(double f1, double f2)
{
   return std::sqrt(f1 * f2);
}

inline double PhysicsEngine::MixRestitution(double r1, double r2)
{
   return r1 > r2 ? r1 : r2;
}

#endif