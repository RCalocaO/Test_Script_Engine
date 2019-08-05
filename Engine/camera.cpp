// Camera
// (C) 1999 by A'r-Ci-O'oh (rco@hotmail.com)

#include "camera.h"
#include "polygon3d.h"
#include "sector.h"
#include "matrix.h"
#include "../world.h"

int Camera::aspect = 0;
double Camera::inv_aspect = 0;

Camera::Camera():OrthoTransform()
{
  sector = NULL;
}

Camera::~Camera () {}

Polygon3d* Camera::GetHit(Vector3d& v)
{
//  return sector->HitBeam(v_o2t, v);
	return NULL;
}

void Camera::MoveWorld(const Vector3d& v)
{
  Vector3d new_position = GetOrigin () + v;
  if (sector) sector = sector->FollowSegment(*this, new_position);
  SetOrigin(new_position);
}

void Camera::RotateWorld (const Vector3d& v, double angle)
{
  Vector3d u = v;
  double ca, sa;
  u = Vector3d::Unit (u);
  ca = cos (angle);
  sa = sin (angle);
  SetT2O ( 

   Matrix(
         ca+(1-ca)*u.x*u.x, u.x*u.y*(1-ca)-u.z*sa, u.x*u.z*(1-ca)+u.y*sa,
     u.x*u.y*(1-ca)+u.z*sa,     ca+(1-ca)*u.y*u.y, u.y*u.z*(1-ca)-u.x*sa,
     u.x*u.z*(1-ca)-u.y*sa, u.y*u.z*(1-ca)+u.x*sa,     ca+(1-ca)*u.z*u.z)

   * GetT2O () );
}

void Camera::Rotate (const Vector3d& v,double angle)
{
  Vector3d u = v;
  double ca, sa;
  u = Vector3d::Unit (u);
  ca = cos (angle);
  sa = sin (angle);

  SetT2O (GetT2O () *

   Matrix(
         ca+(1-ca)*u.x*u.x, u.x*u.y*(1-ca)-u.z*sa, u.x*u.z*(1-ca)+u.y*sa,
     u.x*u.y*(1-ca)+u.z*sa,     ca+(1-ca)*u.y*u.y, u.y*u.z*(1-ca)-u.x*sa,
     u.x*u.z*(1-ca)-u.y*sa, u.y*u.z*(1-ca)+u.x*sa,     ca+(1-ca)*u.z*u.z)

   );
}

void Camera::LookAt (const Vector3d& v, const Vector3d& up)
{
  Matrix m; /* initialized to be the identity matrix */
  Vector3d w1, w2, w3 = v;
  double r;
  
  if ( (r=sqrt(v*v)) > SMALL_EPSILON )
  {
   w3 /= r;
   w1 = w3 % up;
   if ( (r=sqrt(w1*w1)) < SMALL_EPSILON )
   {
     w1 = w3 % Vector3d(0,0,-1);
     if ( (r=sqrt(w1*w1)) < SMALL_EPSILON )
     {
      w1 = w3 % Vector3d(0,-1,0);
      r = sqrt(w1*w1);
     }
   }
   w1 /= r;
   w2 = w3 % w1;

   m.m11 = w1.x;  m.m12 = w2.x;  m.m13 = w3.x;
   m.m21 = w1.y;  m.m22 = w2.y;  m.m23 = w3.y;
   m.m31 = w1.z;  m.m32 = w2.z;  m.m33 = w3.z;
  }

//  SetT2O (m);
}

//---------------------------------------------------------------------------
#ifdef ENOITA
Transform Transform::GetReflect (const Plane& pl)
{
  // Suppose that n is the plane normal in the direction of th reflection.
  // Suppose that u is the unit vector in the direction of the reflection
  // normal.  For any vector v, the component of v in the direction of
  // u is equal to (v * u) * u.  Thus, if v is reflected across a plane
  // through the origin with the given normal, the resulting vector is
  //  v' = v - 2 * [ (v * u) * u ] = v - 2 [ (v * n) * n ] / (n * n)
  //
  // x = <1,0,0>  =>  x' = <1,0,0> - 2 ( n.x * n ) / (n*n)
  // y = <0,1,0>  =>  y' = <0,1,0> - 2 ( n.y * n ) / (n*n)
  // z = <0,0,1>  =>  z' = <0,0,1> - 2 ( n.z * n ) / (n*n)
  //
  // 3x3 transformation matrix = [x' y' z']

  float i_normsq = 1/(pl.norm*pl.norm);
  Vector3d xvec = (-2 * pl.norm.x * i_normsq) * pl.norm;
  Vector3d yvec = (-2 * pl.norm.y * i_normsq) * pl.norm;
  Vector3d zvec = (-2 * pl.norm.z * i_normsq) * pl.norm;
  xvec.x+=1;  yvec.y+=1;  zvec.z+=1;

  return Transform (Matrix ( xvec.x, yvec.x, zvec.x,
                                  xvec.y, yvec.y, zvec.y,
                                  xvec.z, yvec.z, zvec.z ),
                /* neworig = */ (-2 * pl.DD * i_normsq ) * pl.norm );
}

//---------------------------------------------------------------------------

Plane Transform::Other2This (const Plane& p) const
{
  Vector3d newnorm = m_o2t*p.norm;
// let N represent norm <A,B,C>, and X represent point <x,y,z>
//
// Old plane equation: N*X + D = 0
// There exists point X = <r*A,r*B,r*C> = r*N which satisfies the
// plane equation.  
//  => r*(N*N) + D = 0
//  => r = -D/(N*N)
//
// New plane equation: N'*X' + D' = 0
// If M is the transformation matrix, and V the transformation vector,
// N' = M*N, and X' = M*(X-V).  Assume that N' is already calculated.
//  => N'*(M*(X-V)) + D' = 0
//  => D' = -N'*(M*X) + N'*(M*V)
//        = -N'*(M*(r*N)) + N'*(M*V)
//        = -r*(N'*N') + N'*(M*V) = D*(N'*N')/(N*N) + N'*(M*V)
// Since N' is a rotation of N, (N'*N') = (N*N), thus
//  D' = D + N'*(M*V)
//
  return Plane (newnorm, p.DD + newnorm * (m_o2t*v_o2t) );
}
void Transform::Other2This (const Plane& p, const Vector3d& point,
                           Plane& result) const
{
  result.norm = m_o2t*p.norm;
  result.DD = - (result.norm * point);
}
#endif

Vector3d operator* (const Vector3d& v, const Transform& t)
{ return t.Other2This (v); }

Vector3d operator* (const Transform& t, const Vector3d& v)
{ return t.Other2This (v); }

Vector3d& operator*= (Vector3d& v, const Transform& t)
{ v = t.Other2This (v); return v; }
/*
Plane operator* (const Plane& p, const Transform& t)
{ return t.Other2This (p); }

Plane operator* (const Transform& t, const Plane& p)
{ return t.Other2This (p); }

Plane& operator*= (Plane& p, const Transform& t)
{
  p.norm = t.m_o2t * p.norm;
  p.DD += p.norm * (t.m_o2t*t.v_o2t);
  return p;
}
*/
Matrix operator* (const Matrix& m, const Transform& t)
{ return m*t.m_o2t; }

Matrix operator* (const Transform& t, const Matrix& m) 
{ return t.m_o2t*m; }

Matrix& operator*= (Matrix& m, const Transform& t) 
{ return m *= t.m_o2t; }

//---------------------------------------------------------------------------
/*
Plane ReversibleTransform::This2Other (const Plane& p) const
{
  Vector3d newnorm = m_t2o * p.norm;
  return Plane (newnorm, p.DD - p.norm * (m_o2t*v_o2t) );
}

void ReversibleTransform::This2Other (const Plane& p, const Vector3d& point,
                           Plane& result) const
{
  result.norm = m_t2o*p.norm;
  result.DD = - (result.norm * point);
}
*/
Vector3d operator/ (const Vector3d& v, const ReversibleTransform& t)
{ return t.This2Other (v); }

Vector3d& operator/= (Vector3d& v, const ReversibleTransform& t)
{ v = t.This2Other (v); return v; }
/*
Plane operator/ (const Plane& p, const ReversibleTransform& t)
{ return t.This2Other (p); }

Plane& operator/= (Plane& p, const ReversibleTransform& t)
{
  p.DD -= p.norm * (t.m_o2t*t.v_o2t);
  p.norm = t.m_t2o * p.norm;
  return p;
}
*/
ReversibleTransform& operator*= (ReversibleTransform& t1,
                                 const ReversibleTransform& t2)
{
  t1.v_o2t = t2.v_o2t + t2.m_t2o*t1.v_o2t;
  t1.m_o2t *= t2.m_o2t;
  t1.m_t2o = t2.m_t2o*t1.m_t2o;
  return t1;
}

ReversibleTransform operator* (const ReversibleTransform& t1,
                               const ReversibleTransform& t2)
{
  return ReversibleTransform (t1.m_o2t*t2.m_o2t, t2.m_t2o*t1.m_t2o, 
                             t2.v_o2t + t2.m_t2o*t1.v_o2t); 
}

Transform operator* (const Transform& t1, const ReversibleTransform& t2)
{
  return Transform (t1.m_o2t*t2.m_o2t, t2.v_o2t + t2.m_t2o*t1.v_o2t);
}

ReversibleTransform& operator/=(ReversibleTransform& t1,
                                const ReversibleTransform& t2)
{
 t1.v_o2t = t2.m_o2t*(t1.v_o2t - t2.v_o2t);
 t1.m_o2t *= t2.m_t2o;
 t1.m_t2o = t2.m_o2t*t1.m_t2o;
 return t1;
}

ReversibleTransform operator/ (const ReversibleTransform& t1,
                               const ReversibleTransform& t2)
{
  return ReversibleTransform (t1.m_o2t*t2.m_t2o, t2.m_o2t*t1.m_t2o, 
                   t2.m_o2t*(t1.v_o2t - t2.v_o2t)); 
}

