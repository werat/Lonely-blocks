// @werat

#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

#include <cmath>
#include <vector>

#include "common.h"
#include "rigid_body.h"
#include "collision.h"

struct RaycastIn
{
   Vector2 origin;
   Vector2 direction;
   double max_distance = 0; // 0 or less the for the infinite ray
   unsigned int categoryMask = 0xFFFFFFFF; // accept all categoties 
};
struct RaycastOut
{
   RigidBody* body;
   double distance;
   Vector2 normal;
   Vector2 contact_point;
};

class PhysicsEngine
{
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
   
public:
   Vector2 gravity;
   
private:
   // _steps - number of steps we take during one usual Update(delta)
   // dt would be (delta / _steps) if delta is fixed
   int _steps = 5;
   float _minimal_dt = 1.0 / 60 / _steps;

   // accumulated time from previous updates
   float _accumulator = 0.0;

   std::vector<RigidBody*> _rigidBodies;

   DISALLOW_COPY_AND_ASSIGN(PhysicsEngine);
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