// @werat

#include "physical.h"

#include <cmath>

Physical::Physical(const Vector2& position, int w, int h)
{
   this->position = position;
   this->velocity = Vector2::Zero;
   this->width = w;
   this->height = h;
   this->isStatic = false;
}

void Physical::Translate(const Vector2& delta)
{
   this->position += delta;
}
bool Physical::Intersects(const Physical& other, Vector2* resolution)
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
