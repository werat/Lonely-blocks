// @werat


#include <cmath>
#include <stdexcept>
#include <limits>

#include "rigid_body.h"

RigidBody::RigidBody()
{
}

void RigidBody::ClearForces()
{
   this->_force = Vector2::Zero;
}

void RigidBody::setType(rBodyType type)
{
   if (this->_type == type) return;

   this->_type = type;

   if (this->_type == r_staticBody || this->_type == r_kinematicBody)
   {
      this->inv_mass = 0.0;
   }
   if (this->_type == r_staticBody)
   {
      this->velocity = Vector2::Zero;
   }
   this->ClearForces();
}
void RigidBody::setFilterData(const cFilter& filter)
{
   this->_filterData = filter;
}
void RigidBody::setMass(double mass)
{
   if (this->_type != r_dynamicBody) return;

   if (mass <= 0) throw std::invalid_argument("Mass of the dynamic body must be positive.");
   else           this->inv_mass = 1.0 / mass;
}


void RigidBody::ApplyImpulse(const Vector2& impulse)
{
   if (this->_type != r_dynamicBody) return;

   this->velocity += impulse * this->inv_mass;
}
void RigidBody::ApplyForce(const Vector2& force)
{
   if (this->_type != r_dynamicBody) return;

   this->_force += force;
}

bool RigidBody::ShouldCollide(const RigidBody* other)
{
   const cFilter& filterA = this->filterData();
   const cFilter& filterB = other->filterData();

   if (filterA.groupIndex == filterB.groupIndex && filterA.groupIndex != 0)
   {
      return filterA.groupIndex > 0;
   }

   bool collide = (filterA.maskBits & filterB.categoryBits) != 0 
               && (filterB.maskBits & filterA.categoryBits) != 0;
   return collide;
}

bool RigidBody::Intersects(const RigidBody* other, Vector2* resolution)
{
   Vector2 n = other->position - this->position;

   double x_overlap = this->half_width + other->half_width - std::abs(n.x);
   double y_overlap = this->half_height + other->half_height - std::abs(n.y);

   bool intersect = x_overlap > 0 && y_overlap > 0;
   if (intersect && resolution != nullptr)
   {
      resolution->x = n.x > 0 ? -x_overlap : x_overlap;
      resolution->y = n.y > 0 ? -y_overlap : y_overlap;
   }
   return intersect;
}
bool RigidBody::ContainsPoint(const Vector2& point)
{
   bool containsX = (point.x >= this->position.x - this->half_width)
                 && (point.x <= this->position.x + this->half_width);
   bool containsY = (point.y >= this->position.y - this->half_height)
                 && (point.y <= this->position.y + this->half_height);

   return containsX && containsY;
}
bool RigidBody::Raycast(const _Internal_RaycastIn& input, _Internal_RaycastOut* output)
{
   Vector2 inv_d = { 1.0 / input.direction.x, 1.0 / input.direction.y };

   // Real-Time Collision Detection, p179

   double tmin = 0.0;
   double tmax = input.max_distance;
   Vector2 normal;

   // check for X-axis
   if (input.direction.x == 0.0) // TODO (werat): |x| < EPSILON
   {
      // slight modification from the original alrotithm: non-strict comparison instead of strict
      if (position.x - half_width >= input.origin.x
       || position.x + half_width <= input.origin.x)
         return false;
   }
   else
   {
      double tx1 = (position.x - half_width - input.origin.x) * inv_d.x;
      double tx2 = (position.x + half_width - input.origin.x) * inv_d.x;

      double txmin = std::min(tx1, tx2);
      double txmax = std::max(tx1, tx2);

      if (txmin > tmin) 
      {
         tmin = txmin;
         normal = { tx1 > tx2 ? 1.0 : -1.0, 0 };
      }
      if (txmax < tmax) tmax = txmax;

      if (tmin > tmax) return false;
   }

   // check for Y-axis
   if (input.direction.y == 0.0) // TODO (werat): |y| < EPSILON
   {
      if (position.y - half_height >= input.origin.y
       || position.y + half_height <= input.origin.y)
         return false;
   }
   else
   {
      double ty1 = (position.y - half_height - input.origin.y) * inv_d.y;
      double ty2 = (position.y + half_height - input.origin.y) * inv_d.y;

      double tymin = std::min(ty1, ty2);
      double tymax = std::max(ty1, ty2);

      if (tymin > tmin) 
      {
         tmin = tymin;
         normal = { 0, ty1 > ty2 ? 1.0 : -1.0 };
      }
      if (tymax < tmax) tmax = tymax;

      if (tmin > tmax) return false;
   }

   // intersection with ray occured
   if (output != nullptr)
   {
      output->distance = tmin;
      output->normal = normal;
   }
   return true;
}