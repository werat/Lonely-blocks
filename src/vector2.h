// @werat

#ifndef VECTOR2_H
#define VECTOR2_H

#include <cmath>
#include <iostream>


class Vector2
{
public:
   static const Vector2 Zero;
   static const Vector2 UnitX;
   static const Vector2 UnitY;

public:
   double x, y;

public:
   Vector2() { this->x = 0; this->y = 0; }
   explicit Vector2(double d) { this->x = d; this->y = d; }
   Vector2(double x, double y) { this->x = x; this->y = y; }
   
   // Copy and assignment are not necessary, let the compiler do all the work

   double length() const { return std::sqrt(x*x + y*y); }
   double lengthSquared() const { return x*x + y*y; }

   double Normalize()
   {
      double len = length();
      if (len == 0.0) return 0.0;

      double ilen = 1.0 / len;
      this->x *= ilen;
      this->y *= ilen;
      return len;
   }
   Vector2 Normalized() const
   {
      double len = length();
      if (len == 0) return Vector2::Zero;
      
      double ilen = 1.0 / len;
      return Vector2 { x * ilen, y * ilen };
   }

   Vector2 operator-() const;
   Vector2& operator+=(const Vector2& other);
   Vector2& operator-=(const Vector2& other);
   Vector2& operator*=(double mul);

   friend std::ostream& operator<< (std::ostream& os, const Vector2& vector);
};

inline Vector2 Vector2::operator-() const
{
   return Vector2(-x, -y);
}
inline Vector2& Vector2::operator+=(const Vector2& other)
{
   this->x += other.x;
   this->y += other.y;
   return *this;
}
inline Vector2& Vector2::operator-=(const Vector2& other)
{
   this->x -= other.x;
   this->y -= other.y;
   return *this;
}
inline Vector2& Vector2::operator*=(double mul)
{
   this->x *= mul;
   this->y *= mul;
   return *this;
}

inline Vector2 operator+(const Vector2& a, const Vector2& b)
{
   return Vector2(a.x + b.x, a.y + b.y);
}
inline Vector2 operator-(const Vector2& a, const Vector2& b)
{
   return Vector2(a.x - b.x, a.y - b.y);
}
inline Vector2 operator*(const Vector2& a, double mul)
{
   return Vector2(a.x * mul, a.y * mul);
}
inline Vector2 operator*(double mul, const Vector2& a)
{
   return Vector2(a.x * mul, a.y * mul);
}

inline bool operator==(const Vector2& a, const Vector2& b)
{
   return a.x == b.x && a.y == b.y;
}
inline bool operator!=(const Vector2& a, const Vector2& b)
{
   return a.x != b.x || a.y != b.y;
}

inline double Dot(const Vector2& a, const Vector2& b)
{
   return a.x * b.x + a.y * b.y;
}
inline double Cross(const Vector2& a, const Vector2& b)
{
   return a.x * b.y - a.y * b.x;
}
inline Vector2 Cross(const Vector2& a, float s)
{
   return Vector2(s * a.y, -s * a.x);
}
inline Vector2 Cross(float s, const Vector2& a)
{
   return Vector2(-s * a.y, s * a.x);
}

#endif

