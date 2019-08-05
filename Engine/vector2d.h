// 2d vectors
// (C) 1999 by A'r-Ci-O'oh, rco@hotmail.com

#ifndef __VECTOR2D_H__
#define __VECTOR2D_H__

class Vector2d;
#include "../trohs3d.h"

class Vector2d
{
   public:
     double x,y;

     // Empty Constructor At Origin
     Vector2d() {};

     // Init Constructor
     Vector2d(double xx,double yy):x(xx),y(yy) {};

     // Copy Constructor
     Vector2d(const Vector2d& v):x(v.x),y(v.y) {};

     // Various Operations
     inline double Norm() { return sqrt(x*x+y*y); };

     inline Vector2d Normalize()
       { double m=sqrt(x*x+y*y); x/=m; y/=m; return *this; };

     // Set vector to given values.
     inline void Set (double ix, double iy)
     { x = ix; y = iy; }

     // Return the norm (magnitude) of a 2D vector.
     static double Norm (const Vector2d& v)
     { return sqrt (v*v); }

     // Rotate vector around the origin by a given angle in radians.
     void Rotate (double angle)
     {
       double s = sin (angle); double c = cos (angle);
       double nx = x * c + y * s;
       y = -x * s + y * c;
       x = nx;
     }

     // Add another vector to this vector.
     Vector2d& operator+= (const Vector2d& v)
     { x += v.x;  y += v.y;  return *this; }

     // Subtract another vector from this vector.
     Vector2d& operator-= (const Vector2d& v)
     { x -= v.x;  y -= v.y;  return *this; }

     // Multiply this vector by a scalar.
     Vector2d& operator*= (double f) { x *= f;  y *= f;  return *this; }

     // Divide this vector by a scalar.
     Vector2d& operator/= (double f) { x /= f;  y /= f;  return *this; }

     // Unary + operator.
     inline Vector2d operator+ () const { return *this; }

     // Unary - operator.
     inline Vector2d operator- () const { return Vector2d(-x,-y); }

     // Add two vectors.
     friend Vector2d operator+ (const Vector2d& v1, const Vector2d& v2)
     { return Vector2d(v1.x+v2.x, v1.y+v2.y); }

     // Subtract two vectors.
     friend Vector2d operator- (const Vector2d& v1, const Vector2d& v2)
     { return Vector2d(v1.x-v2.x, v1.y-v2.y); }

     // Take the dot product of two vectors.
     friend double operator* (const Vector2d& v1, const Vector2d& v2)
     { return v1.x*v2.x+v1.y*v2.y; }

     // Multiply a vector and a scalar.
     friend Vector2d operator* (const Vector2d& v, double f)
     { return Vector2d(v.x*f, v.y*f); }

     // Multiply a vector and a scalar.
     friend Vector2d operator* (double f, const Vector2d& v)
     { return Vector2d(v.x*f, v.y*f); }

     // Divide a vector by a scalar.
     friend Vector2d operator/ (const Vector2d& v, double f)
     { return Vector2d(v.x/f, v.y/f); }

     // Check if two vectors are equal.
     friend bool operator== (const Vector2d& v1, const Vector2d& v2)
     { return (v1.x==v2.x) && (v1.y==v2.y); }

     // Check if two vectors are not equal.
     friend bool operator!= (const Vector2d& v1, const Vector2d& v2)
     { return (v1.x!=v2.x) || (v1.y!=v2.y); }

};

#endif

