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

RigidBody* PhysicsEngine::CreateBody()
{
   RigidBody* body = new RigidBody();

   _rigidBodies.push_back(body);

   return body;
}
void PhysicsEngine::DestroyBody(RigidBody* rigidBody)
{
   if (rigidBody == nullptr)
      throw std::invalid_argument("rigidBody can't be null");

   auto body_it = std::find(begin(_rigidBodies), end(_rigidBodies), rigidBody);
   if (body_it == end(_rigidBodies))
      throw std::logic_error("rigidBody is not attached.");

   _rigidBodies.erase(body_it);
}


void PhysicsEngine::Update(float delta)
{
   const double dt = delta / _steps; // delta is fixed, otherwise we should fix this
   
   std::vector<ContactData> contacts;

   // TODO (werat): eliminate all this "if (body->type() == r_staticBody) continue;"
   for (int i = 0; i < _steps; ++i)
   {
      // This is BROADPHASE.
      // Here we iterate over objects constructing pairs of potential contacts
      // *** at the moment there are only AABBs, so we don't need narrowphase ***
      // TODO (werat): make quad-tree
      for (auto rigidBodyA : _rigidBodies)
      {
         if (rigidBodyA->type() != r_dynamicBody) continue;

         for (auto rigidBodyB : _rigidBodies)
         {
            if (rigidBodyA == rigidBodyB) continue;

            ContactData contact = CreateContactData(rigidBodyA, rigidBodyB);

            // check if two AABBs collided and whether they pass each other's filters 
            if (contact.penetration > 0 && ShouldCollide(rigidBodyA, rigidBodyB))
            {
               // check if these two bodies already contacted
               if (std::find(begin(contacts), end(contacts), contact) == end(contacts))
                  contacts.emplace_back(contact);
            }
         }
      }
      // Intergrate velocities
      for (auto body : _rigidBodies)
      {
         IntegrateLinearProperties(body, dt / 2.0);
      }
      // Resolve contacts aka apply impulses
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
      // Intergrate positions and velocities
      for (auto body : _rigidBodies)
      {
         if (body->type() == r_staticBody) continue;

         body->position += body->velocity * dt;
         body->rotation += body->angular_velocity * dt;

         IntegrateLinearProperties(body, dt / 2.0);
      }
      // Correct position
      for (auto contact : contacts)
      {
         CorrectPosition(contact);
      }
      contacts.clear();
   }
   // Clear forces
   for (auto body : _rigidBodies)
   {
      if (body->type() == r_staticBody) continue;

      body->ClearForces();
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

   double x_overlap = first->half_width + second->half_width - std::abs(n.x);
   double y_overlap = first->half_height + second->half_height - std::abs(n.y);

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

   double alongNormal = Dot(rv, normal);
   // check if separating velocity
   if (alongNormal < 0) return;

   // TODO (werat): optimize to cool inline functions
   // smth like (restitution1 > restitution2 : restitution2 ? restitution1)
   double e = MixRestitution(first->restitution, second->restitution);

   // calculate impulse
   double j = -(1.0 + e) * alongNormal;
   j /= inv_mass_sum;

   // apply impulse
   Vector2 impulse = normal * j;
   first->velocity -= impulse * first->inv_mass;
   second->velocity += impulse * second->inv_mass;

   // calculate friction impulse
   rv = second->velocity - first->velocity;
   Vector2 t = rv - (normal * Dot(rv, normal));

   if (t == Vector2::Zero) return;
   t.Normalize();

   // calculate friction coefficient
   double sf = MixFriction(first->friction, second->friction);

   // j tangent magnitude
   double jt = -Dot(rv, t);
   jt /= inv_mass_sum;

   Vector2 tangentImpulse;
   if (std::abs(jt) < -j * sf) tangentImpulse = t * jt;
   else                        tangentImpulse = t * j * sf;

   first->velocity -= tangentImpulse * first->inv_mass;
   second->velocity += tangentImpulse * second->inv_mass;
}
void PhysicsEngine::IntegrateLinearProperties(RigidBody* body, float dt)
{
   if (body->type() == r_staticBody || body->type() == r_kinematicBody) return;

   body->velocity += (body->_force * body->inv_mass + gravity * body->gravity_scale) * dt;
   // body->angular_velocity += body->torque * body->inv_inertia * dt;
}
void PhysicsEngine::CorrectPosition(const ContactData& contact)
{
   // let second is static
   RigidBody* first = contact.first;
   RigidBody* second = contact.second;

   double percent = 0.7; // usually 20% to 80%
   double slop = 0.0; // TODO (werat): do we need this slop?
   Vector2 correction = contact.normal * (std::max(contact.penetration - slop, 0.0) / (first->inv_mass + second->inv_mass) * percent);
   first->position += correction * first->inv_mass;
   second->position -= correction * second->inv_mass;
}