// 3d vectors
// (C) 1999 by A'r-Ci-O'oh, rco@hotmail.com

#ifndef __VECTOR3D_H__
#define __VECTOR3D_H__

class Vector3d;

#include "../trohs3d.h"

class Vector3d
{
   public:
     double x,y,z;

     // Empty Constructor
     Vector3d() {};

     // Init Constructor
     Vector3d(double xx,double yy,double zz=0):x(xx),y(yy),z(zz) {};

     // Copy Constructor
     Vector3d(const Vector3d& v):x(v.x),y(v.y),z(v.z) {};

     // Various Operations
     inline double Norm() const { return sqrt(x*x+y*y+z*z); };

     inline Vector3d Normalize()
       { double m=sqrt(x*x+y*y+z*z); x/=m; y/=m; z/=m; return *this; };

     // Add two vectors.
     inline friend Vector3d operator+ (const Vector3d& v1, const Vector3d& v2)
     { return Vector3d(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z); }

     // Subtract two vectors.
     inline friend Vector3d operator- (const Vector3d& v1, const Vector3d& v2)
     { return Vector3d(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z); }

     // Take the dot product of two vectors.
     inline friend double operator* (const Vector3d& v1, const Vector3d& v2)
     { return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z; }

     // Take the cross product of two vectors.
     inline friend Vector3d operator% (const Vector3d& v1, const Vector3d& v2)
     { return Vector3d (v1.y*v2.z-v1.z*v2.y,v1.z*v2.x-v1.x*v2.z,v1.x*v2.y-v1.y*v2.x); };

     // Multiply a vector and a scalar.
     inline friend Vector3d operator* (const Vector3d& v, double f)
     { return Vector3d(v.x*f, v.y*f, v.z*f); }

     // Multiply a vector and a scalar.
     inline friend Vector3d operator* (double f, const Vector3d& v)
     { return Vector3d(v.x*f, v.y*f, v.z*f); }

     // Divide a vector by a scalar.
     inline friend Vector3d operator/ (const Vector3d& v, double f)
     { f = 1.0/f; return Vector3d(v.x*f, v.y*f, v.z*f); }

     // Check if two vectors are equal.
     inline friend bool operator== (const Vector3d& v1, const Vector3d& v2)
     { return v1.x==v2.x && v1.y==v2.y && v1.z==v2.z; }

     // Check if two vectors are not equal.
     inline friend bool operator!= (const Vector3d& v1, const Vector3d& v2)
     { return v1.x!=v2.x || v1.y!=v2.y || v1.z!=v2.z; }

     // Add another vector to this vector.
     inline Vector3d& operator+= (const Vector3d& v)
     { x += v.x; y += v.y; z += v.z; return *this; }

     // Subtract another vector from this vector.
     inline Vector3d& operator-= (const Vector3d& v)
     { x -= v.x;y -= v.y; z -= v.z; return *this; }

     // Multiply this vector by a scalar.
     inline Vector3d& operator*= (double f) { x *= f; y *= f; z *= f; return *this; }

     // Divide this vector by a scalar.
     inline Vector3d& operator/= (double f) { x /= f; y /= f; z /= f; return *this; }

     // Unary + operator.
     inline Vector3d operator+ () const { return *this; }

     // Unary - operator.
     inline Vector3d operator- () const { return Vector3d(-x,-y,-z); }

     // Set the value of this vector.
     inline void Set (double sx, double sy, double sz) { x = sx; y = sy; z = sz; }

     // Returns the unit vector in the direction of this vector.
     Vector3d Unit () const { return (*this)/(this->Norm()); }

     // Returns the norm (magnitude) of a vector.
     inline static double Norm (const Vector3d& v) { return v.Norm(); }

     // Normalizes a vector to a unit vector.
     inline static Vector3d Unit (const Vector3d& v) { return v.Unit(); }
  /// Test if each component of a vector is less than a small epsilon value.
  inline friend bool operator< (const Vector3d& v, float f)
  { return ABS(v.x)<f && ABS(v.y)<f && ABS(v.z)<f; }

  /// Test if each component of a vector is greater than a small epsilon value.
  inline friend bool operator> (float f, const Vector3d& v)
  { return ABS(v.x)<f && ABS(v.y)<f && ABS(v.z)<f; }
};

#endif

