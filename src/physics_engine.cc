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
      throw std::invalid_argument("The rigidBody argument can't be null.");

   auto body_it = std::find(begin(_rigidBodies), end(_rigidBodies), rigidBody);
   if (body_it == end(_rigidBodies))
      throw std::logic_error("The rigidBody is not attached.");

   _rigidBodies.erase(body_it);
   delete rigidBody;
}

bool PhysicsEngine::Raycast(const RaycastIn& input, RaycastOut* output)
{
   double Max_Distance = input.max_distance > 0 
                           ? input.max_distance 
                           : std::numeric_limits<double>::max();

   RigidBody* out_body = nullptr;
   double out_distance = std::numeric_limits<double>::max();
   Vector2 out_normal;

   Vector2 inv_d = { 1.0 / input.direction.x, 1.0 / input.direction.y };

   for (auto b : _rigidBodies)
   {
      if ((b->filterData().categoryBits & input.categoryMask) == 0)
      {
         // Ingore body, as it's
         continue;
      }
      if (b->ContainsPoint(input.origin))
      {
         // Ignore body if origin inside it
         continue;
      }

      // Real-Time Collision Detection, p179

      double tmin = 0.0;
      double tmax = Max_Distance;
      Vector2 normal;

      // check for X-axis
      if (input.direction.x == 0.0) // TODO (werat): |x| < EPSILON
      {
         // slight modification from the original alrotithm: non-strict comparison instead of strict
         if (b->position.x - b->half_width >= input.origin.x
          || b->position.x + b->half_width <= input.origin.x)
            continue;
      }
      else
      {
         double tx1 = (b->position.x - b->half_width - input.origin.x) * inv_d.x;
         double tx2 = (b->position.x + b->half_width - input.origin.x) * inv_d.x;

         double txmin = std::min(tx1, tx2);
         double txmax = std::max(tx1, tx2);

         if (txmin > tmin) 
         {
            tmin = txmin;
            normal = { tx1 > tx2 ? 1.0 : -1.0, 0 };
         }
         if (txmax < tmax) tmax = txmax;

         if (tmin > tmax) continue;
      }

      // check for Y-axis
      if (input.direction.y == 0.0) // TODO (werat): |y| < EPSILON
      {
         if (b->position.y - b->half_height >= input.origin.y
          || b->position.y + b->half_height <= input.origin.y)
            continue;
      }
      else
      {
         double ty1 = (b->position.y - b->half_height - input.origin.y) * inv_d.y;
         double ty2 = (b->position.y + b->half_height - input.origin.y) * inv_d.y;
   
         double tymin = std::min(ty1, ty2);
         double tymax = std::max(ty1, ty2);
   
         if (tymin > tmin) 
         {
            tmin = tymin;
            normal = { 0, ty1 > ty2 ? 1.0 : -1.0 };
         }
         if (tymax < tmax) tmax = tymax;
   
         if (tmin > tmax) continue;
      }

      // intersection with ray occured, check if this object is nearer
      if (tmin < out_distance)
      {
         out_body = b;
         out_distance = tmin;
         out_normal = normal;
      }
   }
   bool success = out_body != nullptr;
   if (success && output != nullptr)
   {
      output->body = out_body;
      output->contact_point = input.origin + out_distance * input.direction;
      output->normal = out_normal;
      output->distance = out_distance;
   }
   return success;
}


void PhysicsEngine::Update(float delta)
{
   const double dt = delta / _steps; // delta is fixed, so everything works fine
   
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
            if (contact.penetration > 0 && rigidBodyA->ShouldCollide(rigidBodyB))
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
            CollisionInfo info;
            if (contact.first->onCollision != nullptr)
            {
               info.self = contact.first;
               info.other = contact.second;
               info.normal = contact.normal;
               info.moveDirection = (contact.first->velocity - contact.second->velocity).Normalized();
               contact.first->onCollision(info);
            }
            if (contact.second->onCollision != nullptr)
            {
               info.self = contact.second;
               info.other = contact.first;
               info.normal = -contact.normal;
               info.moveDirection = (contact.second->velocity - contact.first->velocity).Normalized();
               contact.second->onCollision(info);
            }

            ResolveContact(contact);
         }
      }
      // Intergrate positions and velocities
      for (auto body : _rigidBodies)
      {
         if (body->type() == r_staticBody) continue;

         body->position += body->velocity * dt;

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
   // check if objects moving away from each other
   if (alongNormal < 0) return;

   // calculate restitution coefficient
   double e = MixRestitution(first->restitution, second->restitution);

   // calculate impulse
   double j = -(1.0 + e) * alongNormal / inv_mass_sum;

   // apply impulse along collision normal
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
   double jt = -Dot(rv, t) / inv_mass_sum;

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
}
void PhysicsEngine::CorrectPosition(const ContactData& contact)
{
   RigidBody* first = contact.first;
   RigidBody* second = contact.second;

   double percent = 0.6; // usually 20% to 80%
   double slop = 0.03; // TODO (werat): do we need this slop?
   Vector2 correction = contact.normal * (std::max(contact.penetration - slop, 0.0) / (first->inv_mass + second->inv_mass) * percent);
   first->position += correction * first->inv_mass;
   second->position -= correction * second->inv_mass;
}