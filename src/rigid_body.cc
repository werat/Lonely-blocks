// @werat


#include <cmath>
#include <stdexcept>

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