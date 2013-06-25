// @werat

#include "physics_engine.h"

#include <cmath>

#include <vector>
#include <algorithm>
#include <limits>
#include <iterator>
#include <stdexcept>

PhysicsEngine::PhysicsEngine()
{
   gravity = Vector2(0, 1000);
}

PhysicsEngine::~PhysicsEngine()
{
}

void PhysicsEngine::AttachRigidBody(RigidBody* rigidBody)
{
   // TODO: check if already attached;
   if (rigidBody == nullptr)
      throw std::invalid_argument("rigidBody can't be null.");

   if (std::find(begin(_rigidBodys), end(_rigidBodys), rigidBody) != end(_rigidBodys))
      throw std::logic_error("rigidBody is already attached.");
   
   _rigidBodys.push_back(rigidBody);
}
void PhysicsEngine::DetachRigidBody(RigidBody* rigidBody)
{
   // TODO: check if exist
   if (rigidBody == nullptr)
      throw std::invalid_argument("rigidBody can't be null");

   if (std::find(begin(_rigidBodys), end(_rigidBodys), rigidBody) == end(_rigidBodys))
      throw std::logic_error("physial is not attached.");

   _rigidBodys.erase(std::remove(begin(_rigidBodys), end(_rigidBodys), rigidBody), end(_rigidBodys));
}

void PhysicsEngine::Update(float delta)
{
   const double dt = delta / _steps; // delta is fixed, otherwise we should use fix this
   
   for (int i = 0; i < _steps; ++i)
   {
      for (auto rigidBodyA : _rigidBodys)
      {
         if (rigidBodyA->isStatic) continue;

         if (rigidBodyA->isGravityApplied)
            rigidBodyA->velocity += gravity * dt;
         rigidBodyA->position += rigidBodyA->velocity * dt; // should use more accurate intergration

         rigidBodyA->onGround = false;
         rigidBodyA->atCeiling = false;

         std::vector<RigidBody*> c_rigidBodys;
         Vector2 zero, resolution(std::numeric_limits<double>::max());
         bool shouldResolve = false;
         int priorityX = 0, priorityY = 0;
         for (auto rigidBodyB : _rigidBodys)
         {
            if (rigidBodyA == rigidBodyB) continue;

            bool collided = rigidBodyA->Intersects(*rigidBodyB, &zero);
            if (collided)
            {
               if (rigidBodyA->onCollision != nullptr)
                  collided &= rigidBodyA->onCollision(*rigidBodyA, *rigidBodyB);
               if (rigidBodyB->onCollision != nullptr)
                  collided &= rigidBodyB->onCollision(*rigidBodyB, *rigidBodyA);
            }
            shouldResolve |= collided;
            if (collided)
            {
               c_rigidBodys.push_back(rigidBodyB);

               if (std::abs(zero.x) < std::abs(resolution.x)) resolution.x = zero.x;
               if (std::abs(zero.y) < std::abs(resolution.y)) resolution.y = zero.y;

               if (zero.x < 0) --priorityX;
               else            ++priorityX;

               if (zero.y < 0) --priorityY;
               else            ++priorityY;
            }
         }
         if (shouldResolve)
         {
            if (std::abs(priorityX) > std::abs(priorityY)) 
            {
               resolution.y = 0;
               ResolveXThenY(*rigidBodyA, c_rigidBodys, resolution);
            }
            else if (std::abs(priorityX) < std::abs(priorityY))
            {
               resolution.x = 0;
               ResolveYThenX(*rigidBodyA, c_rigidBodys, resolution);
            }
            else
            {
               if (std::abs(resolution.x) <= std::abs(resolution.y))
               {
                  resolution.y = 0;
                  ResolveXThenY(*rigidBodyA, c_rigidBodys, resolution);
               }
               else
               {
                  resolution.x = 0;
                  ResolveYThenX(*rigidBodyA, c_rigidBodys, resolution);
               }
            }

            UpdateRigidBodyState(*rigidBodyA, resolution);
         }
      }   
   }
}

void PhysicsEngine::ResolveXThenY(RigidBody& rigidBody, std::vector<RigidBody*>& c_rigidBodys, const Vector2& resolution)
{
   Vector2 zero = Vector2::Zero;
   bool collided = false;
   rigidBody.position += resolution;
   Vector2 res = Vector2(0, std::numeric_limits<double>::max());
   for (auto rigidBodyB : c_rigidBodys)
   {
      if (rigidBody.Intersects(*rigidBodyB, &zero))
      {
         collided = true;
         res.y = std::min(res.y, zero.y);
      }
   }
   if (collided)
   {
      rigidBody.position += res;
      UpdateRigidBodyState(rigidBody, res);
   }
}

void PhysicsEngine::ResolveYThenX(RigidBody& rigidBody, std::vector<RigidBody*>& c_rigidBodys, const Vector2& resolution)
{
   Vector2 zero = Vector2::Zero;
   bool collided = false;
   rigidBody.position += resolution;
   Vector2 res = Vector2(std::numeric_limits<double>::max(), 0);
   for (auto rigidBodyB : c_rigidBodys)
   {
      if (rigidBody.Intersects(*rigidBodyB, &zero))
      {
         collided = true;
         res.x = std::min(res.x, zero.x);
      }
   }
   if (collided)
   {
      rigidBody.position += res;
      UpdateRigidBodyState(rigidBody, res);
   }
}

void PhysicsEngine::UpdateRigidBodyState(RigidBody& rigidBody, const Vector2& resolution)
{
   bool onGround = resolution.y < 0 && rigidBody.velocity.y > 0;
   bool atCeiling = resolution.y > 0 && rigidBody.velocity.y < 0;

   rigidBody.onGround = onGround;
   rigidBody.atCeiling = atCeiling;

   if ((resolution.x < 0 && rigidBody.velocity.x > 0) ||
       (resolution.x > 0 && rigidBody.velocity.x < 0))
   {
      rigidBody.velocity.x = 0;
   }

   if (onGround || atCeiling) // maybe we want also to reset velocity if atCeiling?
   {
      rigidBody.velocity.y = 0;
   }
}