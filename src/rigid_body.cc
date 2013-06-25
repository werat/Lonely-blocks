// @werat

#include "rigid_body.h"

#include <cmath>

RigidBody::RigidBody()
   : RigidBody(Vector2::Zero, 0, 0)
{
}

RigidBody::RigidBody(const Vector2& position, int w, int h)
{
   this->position = position;
   this->width = w;
   this->height = h;
   this->velocity = Vector2::Zero;
   this->isStatic = false;
   this->isGravityApplied = true;
}

void RigidBody::Translate(const Vector2& delta)
{
   this->position += delta;
}
bool RigidBody::Intersects(const RigidBody& other, Vector2* resolution)
{
   Vector2 n = other.position - this->position;

   double x_overlap = this->width / 2 + other.width / 2 - std::abs(n.x);
   double y_overlap = this->height / 2 + other.height / 2 - std::abs(n.y);

   bool intersect = x_overlap > 0 && y_overlap > 0;
   if (intersect && resolution != nullptr)
   {
      resolution->x = n.x > 0 ? -x_overlap : x_overlap;
      resolution->y = n.y > 0 ? -y_overlap : y_overlap;
   }
   return intersect;
}
