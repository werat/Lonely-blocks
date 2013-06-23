// @werat

#include "physics_engine.h"

#include <cmath>

#include <vector>
#include <algorithm>
#include <limits>

#include <fstream>
extern std::ofstream logfile;

PhysicsEngine::PhysicsEngine()
{
}

PhysicsEngine::~PhysicsEngine()
{
}

void PhysicsEngine::AttachPhysical(Physical& physical)
{
   // TODO: check if already attached;

   physicals.push_back(&physical);
}
void PhysicsEngine::DetachPhysical(Physical& physical)
{
   // TODO: check if exist

   physicals.erase(std::remove(begin(physicals), end(physicals), &physical), end(physicals));
}

void PhysicsEngine::Update(float delta)
{
   const int steps = 5;
   const double dt = delta / steps; // delta is fixed, although we should use fix this
   
   for (int i = 0; i < steps; ++i)
   {
      for (auto physicalA : physicals)
      {
         if (physicalA->isStatic) continue;

         physicalA->velocity += Vector2(0, 1000) * dt; // gravity
         physicalA->position += physicalA->velocity * dt;

         physicalA->onGround = false;
         physicalA->atCeiling = false;

         std::vector<Physical*> c_physicals;
         Vector2 resolution(std::numeric_limits<double>::max());
         Vector2 zero;
         int priorityX = 0, priorityY = 0;
         for (auto physicalB : physicals)
         {
            if (physicalA == physicalB) continue;

            if (physicalA->Intersects(*physicalB, &zero))
            {
               c_physicals.push_back(physicalB);

               if (std::abs(zero.x) < std::abs(resolution.x)) resolution.x = zero.x;
               if (std::abs(zero.y) < std::abs(resolution.y)) resolution.y = zero.y;

               if (zero.x < 0) --priorityX;
               else            ++priorityX;

               if (zero.y < 0) --priorityY;
               else            ++priorityY;
            }
         }
         if (!c_physicals.empty())
         {
            if (std::abs(priorityX) > std::abs(priorityY)) 
            {
               resolution.y = 0;
               ResolveXThenY(*physicalA, c_physicals, resolution);
            }
            else if (std::abs(priorityX) < std::abs(priorityY))
            {
               resolution.x = 0;
               ResolveYThenX(*physicalA, c_physicals, resolution);
            }
            else
            {
               if (std::abs(resolution.x) <= std::abs(resolution.y))
               {
                  resolution.y = 0;
                  ResolveXThenY(*physicalA, c_physicals, resolution);
               }
               else
               {
                  resolution.x = 0;
                  ResolveYThenX(*physicalA, c_physicals, resolution);
               }
            }

            UpdatePhysicalState(*physicalA, resolution);
         }
      }   
   }
}

void PhysicsEngine::ResolveXThenY(Physical& physical, std::vector<Physical*>& c_physicals, const Vector2& resolution)
{
   Vector2 zero = Vector2::Zero;
   bool collided = false;
   physical.position += resolution;
   Vector2 res = Vector2(0, std::numeric_limits<double>::max());
   for (auto physicalB : c_physicals)
   {
      if (physical.Intersects(*physicalB, &zero))
      {
         collided = true;
         res.y = std::min(res.y, zero.y);
      }
   }
   if (collided)
   {
      physical.position += res;
      UpdatePhysicalState(physical, res);
   }
}

void PhysicsEngine::ResolveYThenX(Physical& physical, std::vector<Physical*>& c_physicals, const Vector2& resolution)
{
   Vector2 zero = Vector2::Zero;
   bool collided = false;
   physical.position += resolution;
   Vector2 res = Vector2(std::numeric_limits<double>::max(), 0);
   for (auto physicalB : c_physicals)
   {
      if (physical.Intersects(*physicalB, &zero))
      {
         collided = true;
         res.x = std::min(res.x, zero.x);
      }
   }
   if (collided)
   {
      physical.position += res;
      UpdatePhysicalState(physical, res);
   }
}

void PhysicsEngine::UpdatePhysicalState(Physical& physical, const Vector2& resolution)
{
   bool onGround = resolution.y < 0 && physical.velocity.y > 0;
   bool atCeiling = resolution.y > 0 && physical.velocity.y < 0;

   physical.onGround = onGround;
   physical.atCeiling = atCeiling;

   if ((resolution.x < 0 && physical.velocity.x > 0) ||
       (resolution.x > 0 && physical.velocity.x < 0))
   {
      physical.velocity.x = 0;
   }

   if (onGround) // maybe we want also to reset velocity if atCeiling?
   {
      physical.velocity.y = 0;
   }
}