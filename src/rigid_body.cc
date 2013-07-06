// @werat

#include "rigid_body.h"

#include <cmath>

RigidBody::RigidBody()
{
}

void RigidBody::setFilterData(const cFilter& filter)
{
   _filterData = filter;
}

bool RigidBody::Intersects(const RigidBody* other, Vector2* resolution)
{
   Vector2 n = other->position - this->position;

   // TODO (werat): can be optimized
   // calculate y_overlap after checking x_overlap > 0
   double x_overlap = this->width / 2 + other->width / 2 - std::abs(n.x);
   double y_overlap = this->height / 2 + other->height / 2 - std::abs(n.y);

   bool intersect = x_overlap > 0 && y_overlap > 0;
   if (intersect && resolution != nullptr)
   {
      resolution->x = n.x > 0 ? -x_overlap : x_overlap;
      resolution->y = n.y > 0 ? -y_overlap : y_overlap;
   }
   return intersect;
}
