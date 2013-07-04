// @werat

#include "physics_engine.h"

#include <cmath>

#include <iostream>
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
   if (rigidBody == nullptr)
      throw std::invalid_argument("rigidBody can't be null.");

   if (std::find(begin(_rigidBodies), end(_rigidBodies), rigidBody) != end(_rigidBodies))
      throw std::logic_error("rigidBody is already attached.");
   
   _rigidBodies.push_back(rigidBody);
}
void PhysicsEngine::DetachRigidBody(RigidBody* rigidBody)
{
   if (rigidBody == nullptr)
      throw std::invalid_argument("rigidBody can't be null");

   if (std::find(begin(_rigidBodies), end(_rigidBodies), rigidBody) == end(_rigidBodies))
      throw std::logic_error("physial is not attached.");

   _rigidBodies.erase(std::remove(begin(_rigidBodies), end(_rigidBodies), rigidBody), end(_rigidBodies));
}


void PhysicsEngine::Update(float delta)
{
   const double dt = delta / _steps; // delta is fixed, otherwise we should fix this
   
   std::vector<ContactData> contacts;

   for (int i = 0; i < _steps; ++i)
   {
      // Find contacts
      for (auto rigidBodyA : _rigidBodies)
      {
         if (rigidBodyA->isStatic) continue;

         // TODO (werat): this is bad, because with several dynamic objects we'll have pairs of contacts
         for (auto rigidBodyB : _rigidBodies)
         {
            if (rigidBodyA == rigidBodyB) continue;

            // TODO (werat): make some broad phase (even if current version IS a broad phase)
            ContactData contact = CreateContactData(rigidBodyA, rigidBodyB);
            if (contact.penetration > 0 && ShouldCollide(rigidBodyA, rigidBodyB)) {
               contacts.emplace_back(contact);
            }
         }
      }
      // Apply impulses
      // for (int i = 0; i < 10; ++i) // TODO (werat): check if need more iterations
      {
         for (auto contact : contacts)
         {
            if (contact.first->onCollision != nullptr)
               contact.first->onCollision(contact.first, contact.second);
            if (contact.second->onCollision != nullptr)
               contact.second->onCollision(contact.second, contact.first);
            
            ResolveContact(contact);
         }
      }
      // Intergrate velocities TODO (werat): maybe should split in two steps
      for (auto rigidBodyA : _rigidBodies)
      {
         if (rigidBodyA->isStatic) continue;

         if (rigidBodyA->isGravityApplied)
            rigidBodyA->velocity += gravity * dt;
         rigidBodyA->position += rigidBodyA->velocity * dt; // TODO (werat): should use more accurate intergration
      }
      // Correct position
      for (auto contact : contacts)
      {
         CorrectPosition(contact);
      }
      contacts.clear();
   }
}

bool PhysicsEngine::ShouldCollide(RigidBody* first, RigidBody* second)
{
   // if (first == second) return false;

   const cFilter& filterA = first->filterData();
   const cFilter& filterB = second->filterData();

   if (filterA.groupIndex == filterB.groupIndex && filterA.groupIndex != 0)
   {
      return filterA.groupIndex > 0;
   }

   bool collide = (filterA.maskBits & filterB.categoryBits) != 0 
               && (filterB.maskBits & filterA.categoryBits) != 0;
   return collide;
}

ContactData PhysicsEngine::CreateContactData(RigidBody* first, RigidBody* second)
{
   ContactData contact;
   contact.first = first;
   contact.second = second;
   contact.penetration = 0.0;

   Vector2 n = second->position - first->position;

   double x_overlap = first->width / 2 + second->width / 2 - std::abs(n.x);
   double y_overlap = first->height / 2 + second->height / 2 - std::abs(n.y);

   if (x_overlap > 0 && y_overlap > 0)
   {
      // TODO (werat): optimize to cool inline functions instead of copysign
      if (x_overlap < y_overlap)
      {
         contact.normal = Vector2(-std::copysign(1.0, n.x), 0);
      }
      else
      {
         contact.normal = Vector2(0, -std::copysign(1.0, n.y));
      }

      contact.penetration = std::min(x_overlap, y_overlap);
   }

   return contact;
}

void PhysicsEngine::ResolveContact(const ContactData& contact)
{
   RigidBody* first = contact.first;
   RigidBody* second = contact.second;
   Vector2 normal = contact.normal;

   double inv_mass_sum = first->inv_mass + second->inv_mass;

   Vector2 rv = second->velocity - first->velocity;

   double alongNormal = rv.Dot(normal);
   // check if separating velocity
   if (alongNormal < 0) return;

   // TODO (werat): optimize to cool inline functions
   // smth like (restitution1 > restitution2 : restitution2 ? restitution1)
   double e = std::min(first->restitution, second->restitution);

   // calculate impulse
   double j = -(1.0 + e) * alongNormal;
   j /= inv_mass_sum;

   // apply impulse
   Vector2 impulse = normal * j;
   first->velocity -= impulse * first->inv_mass;
   second->velocity += impulse * second->inv_mass;

   // calculate friction impulse
   rv = second->velocity - first->velocity;
   Vector2 t = rv - (normal * rv.Dot(normal));

   if (t == Vector2::Zero) return;
   t.Normalize();

   // calculate friction coefficient
   double sf = std::sqrt(first->static_friction * second->static_friction);
   double df = std::sqrt(first->dynamic_friction * second->dynamic_friction);

   // j tangent magnitude
   double jt = -rv.Dot(t);
   jt /= inv_mass_sum;

   Vector2 tangentImpulse;
   if (std::abs(jt) < -j * sf)
      tangentImpulse = t * jt;
   else
      tangentImpulse = t * j * df;

   first->velocity -= tangentImpulse * first->inv_mass;
   second->velocity += tangentImpulse * second->inv_mass;
}
void PhysicsEngine::CorrectPosition(const ContactData& contact)
{
   // let second is static
   RigidBody* first = contact.first;
   RigidBody* second = contact.second;
   Vector2 normal = contact.normal;

   double percent = 0.2; // usually 20% to 80%
   double slop = 0.0; // TODO (werat): do we need this slop?
   Vector2 correction = normal * (std::max(contact.penetration - slop, 0.0) / (first->inv_mass + second->inv_mass) * percent);
   first->position += correction * first->inv_mass;
   second->position += correction * second->inv_mass;

   // contact.first->position += normal * contact.penetration;
}