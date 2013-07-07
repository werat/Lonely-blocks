// @werat

#include "vector2.h"

const Vector2 Vector2::Zero = Vector2(0, 0); 
const Vector2 Vector2::UnitX = Vector2(1, 0); 
const Vector2 Vector2::UnitY = Vector2(0, 1); 

std::ostream& operator<< (std::ostream& os, const Vector2& vector)
{
   os << "{ " << vector.x << ", " << vector.y << " }";
   return os;
}
