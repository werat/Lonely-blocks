// @werat

#include "vector2.h"

const Vector2 Vector2::Zero = Vector2(0, 0); 
const Vector2 Vector2::UnitX = Vector2(1, 0); 
const Vector2 Vector2::UnitY = Vector2(0, 1); 

void Vector2::Normalize()
{
   double ilen = 1.0 / this->length();
   this->x *= ilen;
   this->y *= ilen;
}

Vector2 Vector2::Normalized()
{
   double ilen = 1.0 / this->length();
   return Vector2 { x * ilen, y * ilen };
}

Vector2 Vector2::operator-() const
{
   return Vector2(-x, -y);
}
Vector2 Vector2::operator+(const Vector2& other) const
{
   return Vector2(x + other.x, y + other.y);
}
Vector2& Vector2::operator+=(const Vector2& other)
{
   this->x += other.x;
   this->y += other.y;
   return *this;
}
Vector2 Vector2::operator-(const Vector2& other) const
{
   return Vector2(x - other.x, y - other.y);
}
Vector2& Vector2::operator-=(const Vector2& other)
{
   this->x -= other.x;
   this->y -= other.y;
   return *this;
}
Vector2 Vector2::operator*(double mul)
{
   return Vector2(x * mul, y * mul);
}
Vector2& Vector2::operator*=(double mul)
{
   this->x *= mul;
   this->y *= mul;
   return *this;
}

bool Vector2::operator==(const Vector2& other)
{
   return this->x == other.x && this->y == other.y;
}
bool Vector2::operator!=(const Vector2& other)
{
   return !(this->x == other.x && this->y == other.y);
}


std::ostream& operator<< (std::ostream& os, const Vector2& vector)
{
   os << "{ " << vector.x << ", " << vector.y << " }";
   return os;
}
