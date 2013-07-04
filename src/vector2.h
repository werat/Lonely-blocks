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

   double length() { return std::sqrt(x*x + y*y); }
   double lengthSquared() { return x*x + y*y; }

   void Normalize();
   Vector2 Normalized();

   double Dot(const Vector2& other) { return x * other.x + y * other.y; }

   // TODO: maybe do all this functions inline
   Vector2 operator-() const;
   Vector2 operator+(const Vector2& other) const;
   Vector2& operator+=(const Vector2& other);
   Vector2 operator-(const Vector2& other) const;
   Vector2& operator-=(const Vector2& other);
   Vector2 operator*(double mul);
   Vector2& operator*=(double mul);

   bool operator==(const Vector2& other);
   bool operator!=(const Vector2& other);

   friend std::ostream& operator<< (std::ostream& os, const Vector2& vector);
};

#endif

