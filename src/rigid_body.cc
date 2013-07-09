// @werat

#include "rigid_body.h"

#include <cmath>

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