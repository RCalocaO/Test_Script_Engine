// Camera
// (C) 1999 by A'r-Ci-O'oh, rco@hotmail.com

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "../trohs3d.h"
#include "matrix.h"
class Vector3d;
class Plane;
class Sector;

#define VEC_FORWARD   Vector3d(0,0,1)
#define VEC_BACKWARD  Vector3d(0,0,-1)
#define VEC_RIGHT     Vector3d(1,0,0)
#define VEC_LEFT      Vector3d(-1,0,0)
#define VEC_UP        (-Vector3d(0,-1,0))
#define VEC_DOWN      (-Vector3d(0,1,0))

#define VEC_ROT_RIGHT  Vector3d(0,1,0)
#define VEC_ROT_LEFT   Vector3d(0,-1,0)
#define VEC_TILT_RIGHT (-Vector3d(0,0,1))
#define VEC_TILT_LEFT  (-Vector3d(0,0,-1))
#define VEC_TILT_UP    (-Vector3d(1,0,0))
#define VEC_TILT_DOWN  (-Vector3d(-1,0,0))

class Sector;
class Polygon3d;
class World;

class ReversibleTransform;

/**
 * A class which defines a transformation from one coordinate system to
 * another. The two coordinate systems are refered to as 'other'
 * and 'this'. The transform defines a transformation from 'other'
 * to 'this'.
 */
class Transform
{
protected:
  /// Transformation matrix from 'other' space to 'this' space.
  Matrix m_o2t;
  /// Location of the origin for 'this' space.
  Vector3d v_o2t;

public:
  /**
   * Initialize with the identity transformation.
   */
  Transform () : m_o2t (), v_o2t (0, 0, 0) {}

  /**
   * Initialize with the given transformation. The transformation
   * is given as a 3x3 matrix and a vector. The transformation
   * is defined to mean T=M*(O-V) with T the vector in 'this' space,
   * O the vector in 'other' space, M the transformation matrix and
   * V the transformation vector.
   */
  Transform (const Matrix& other2this, const Vector3d& origin_pos) :
  	m_o2t (other2this), v_o2t (origin_pos) {}

  /**
   * Get 'other' to 'this' transformation matrix. This is the 3x3
   * matrix M from the transform equation T=M*(O-V).
   */
  inline const Matrix& GetO2T () const { return m_o2t; }

  /**
   * Get 'world' to 'this' translation. This is the vector V
   * from the transform equation T=M*(O-V).
   */
  inline const Vector3d& GetO2TTranslation () const { return v_o2t; }

  /**
   * Get origin of transformed coordinate system.
   */
  inline const Vector3d& GetOrigin () const { return v_o2t; }

  /**
   * Set 'other' to 'this' transformation matrix.
   * this is the 3x3 matrix M from the transform equation T=M*(O-V).
   */
  virtual void SetO2T (const Matrix& m) { m_o2t = m; }

  /**
   * Set 'world' to 'this' translation. This is the vector V
   * from the transform equation T=M*(O-V).
   */
  inline void SetO2TTranslation (const Vector3d& v) { v_o2t = v; }

  /**
   * Set origin of transformed coordinate system.
   */
  inline void SetOrigin (const Vector3d& v) { v_o2t = v; }

  /**
   * Move the 'other' to 'this' translation by a specified amount.
   */
  inline void Translate (const Vector3d& v) { v_o2t += v; }

  /**
   * Transform vector in 'other' space v to a vector in 'this' space.
   * This is the basic transform function.
   */
  inline Vector3d Other2This (const Vector3d& v) const
  {
      return m_o2t * (v - v_o2t);
  }

  /**
   * Convert vector v in 'other' space to a vector in 'this' space.
   * Use the origin of 'other' space.
   */
  Vector3d Other2ThisRelative (const Vector3d& v) const
  { return m_o2t * v; }

  /**
   * Convert a plane in 'other' space to 'this' space.
   */
  Plane Other2This (const Plane& p) const;

  /**
   * Convert a plane in 'other' space to 'this' space. This is an optimized version
   * for which a point on the new plane is known (point). The result
   * is stored in 'result'.
   */
  void Other2This (const Plane& p, const Vector3d& point, Plane& result) const;

  /**
   * Apply a transformation to a 3D vector.
   */
  friend Vector3d operator* (const Vector3d& v, const Transform& t);

  /// Apply a transformation to a 3D vector.
  friend Vector3d operator* (const Transform& t, const Vector3d& v);
  /// Apply a transformation to a 3D vector.
  friend Vector3d& operator*= (Vector3d& v, const Transform& t);
  /// Apply a transformation to a Plane.
  friend Plane operator* (const Plane& p, const Transform& t);
  /// Apply a transformation to a Plane.
  friend Plane operator* (const Transform& t, const Plane& p);
  /// Apply a transformation to a Plane.
  friend Plane& operator*= (Plane& p, const Transform& t);
  /// Multiply a matrix with the transformation matrix.
  friend Matrix operator* (const Matrix& m, const Transform& t);
  /// Multiply a matrix with the transformation matrix.
  friend Matrix operator* (const Transform& t, const Matrix& m);
  /// Multiply a matrix with the transformation matrix.
  friend Matrix& operator*= (Matrix& m, const Transform& t);
  /// Combine two transforms, rightmost first.
  friend Transform operator* (const Transform& t1,
                              const ReversibleTransform& t2);

  /**
   * Return a transform that represents a mirroring across a plane.
   * This function will return a Transform which represents a reflection
   * across the plane pl.
   */
  static Transform GetReflect (const Plane& pl);
};

/**
 * A class which defines a reversible transformation from one coordinate
 * system to another by maintaining an inverse transformation matrix.
 * This version is similar to Transform (in fact, it is a sub-class)
 * but it is more efficient if you plan to do inverse transformations
 * often.
 */
class ReversibleTransform : public Transform
{
protected:
  /// Inverse transformation matrix ('this' to 'other' space).
  Matrix m_t2o;
  
  /**
   * Initialize transform with both transform matrix and inverse tranform.
   */
  ReversibleTransform (const Matrix& o2t, const Matrix& t2o, 
    const Vector3d& pos) : Transform (o2t,pos), m_t2o (t2o) {}

public:
  /**
   * Initialize with the identity transformation.
   */
  ReversibleTransform () : Transform (), m_t2o () {}

  /**
   * Initialize with the given transformation. The transformation
   * is given as a 3x3 matrix and a vector. The transformation
   * is defined to mean T=M*(O-V) with T the vector in 'this' space,
   * O the vector in 'other' space, M the transformation matrix and
   * V the transformation vector.
   */
  ReversibleTransform (const Matrix& o2t, const Vector3d& pos) :
   Transform (o2t,pos) { m_t2o = m_o2t.GetInverse (); }

  /**
   * Initialize with the given transformation.
   */
  ReversibleTransform (const Transform& t) :
    Transform (t) { m_t2o = m_o2t.GetInverse (); }

  /**
   * Get 'this' to 'other' transformation matrix.
   */
  inline const Matrix& GetT2O () const { return m_t2o; }

  /**
   * Get 'this' to 'other' translation.
   */
  inline Vector3d GetT2OTranslation () const { return -m_o2t*v_o2t; }

  /**
   * Get the inverse of this transform.
   */
  ReversibleTransform GetInverse () const 
  { return ReversibleTransform (m_t2o, m_o2t, -m_o2t*v_o2t); } 

  /**
   * Set 'other' to 'this' transformation matrix.
   */
  virtual void SetO2T (const Matrix& m) 
  { m_o2t = m;  m_t2o = m_o2t.GetInverse (); }

  /**
   * Set 'this' to 'other' transformation matrix.
   */
  virtual void SetT2O (const Matrix& m) 
  { m_t2o = m;  m_o2t = m_t2o.GetInverse (); }

  /**
   * Convert vector v in 'this' space to 'other' space.
   * This is the basic inverse transform operation.
   */
  Vector3d This2Other (const Vector3d& v) const
  { return v_o2t + m_t2o * v; }

  /**
   * Convert vector v in 'this' space to a vector in 'other' space,
   * relative to local origin.
   */
  inline Vector3d This2OtherRelative (const Vector3d& v) const
  { return m_t2o * v; }

  /**
   * Convert a plane in 'this' space to 'other' space.
   */
  Plane This2Other (const Plane& p) const;

  /**
   * Convert a plane in 'this' space to 'other' space. This is an optimized version
   * for which a point on the new plane is known (point). The result
   * is stored in 'result'.
   */
  void This2Other (const Plane& p, const Vector3d& point, Plane& result) const;

  /// Reverse a transformation on a 3D vector.
  friend Vector3d operator/ (const Vector3d& v, const ReversibleTransform& t);
  /// Reverse a transformation on a 3D vector.
  friend Vector3d& operator/= (Vector3d& v, const ReversibleTransform& t);
  /// Reverse a transformation on a Plane.
  friend Plane operator/ (const Plane& p, const ReversibleTransform& t);
  /// Reverse a transformation on a Plane.
  friend Plane& operator/= (Plane& p, const ReversibleTransform& t);
  /// Combine two transforms, with the rightmost being applied first.
  friend ReversibleTransform& operator*= (ReversibleTransform& t1,
                                          const ReversibleTransform& t2);
  /// Combine two transforms, with the rightmost being applied first.
  friend ReversibleTransform operator* (const ReversibleTransform& t1,
                                        const ReversibleTransform& t2);
  /// Combine two transforms, with the rightmost being applied first.
  friend Transform operator* (const Transform& t1, 
                              const ReversibleTransform& t2);
  /// Combine two transforms, reversing t2 then applying t1.
  friend ReversibleTransform& operator/= (ReversibleTransform& t1,
                                          const ReversibleTransform& t2);
  /// Combine two transforms, reversing t2 then applying t1.
  friend ReversibleTransform operator/ (const ReversibleTransform& t1,
                                        const ReversibleTransform& t2);
};

/**
 * A class which defines a reversible transformation from one coordinate
 * system to another by maintaining an inverse transformation matrix.
 * This is a variant which only works on orthonormal transformations (like
 * the camera transformation) and is consequently much more optimal.
 */
class OrthoTransform : public ReversibleTransform
{
public:
  /**
   * Initialize with the identity transformation.
   */
  OrthoTransform () : ReversibleTransform () {}

  /**
   * Initialize with the given transformation.
   */
  OrthoTransform (const Matrix& o2t, const Vector3d& pos) :
    ReversibleTransform (o2t, o2t.GetTranspose (), pos) { }

  /**
   * Initialize with the given transformation.
   */
  OrthoTransform (const Transform& t) :
    ReversibleTransform (t.GetO2T (), t.GetO2T ().GetTranspose (), t.GetO2TTranslation ()) { }

  /**
   * Set 'other' to 'this' transformation matrix.
   */
  virtual void SetO2T (const Matrix& m) 
  { m_o2t = m;  m_t2o = m_o2t.GetTranspose (); }

  /**
   * Set 'this' to 'other' transformation matrix.
   */
  virtual void SetT2O (const Matrix& m) 
  { m_t2o = m;  m_o2t = m_t2o.GetTranspose (); }
};


// ------------------------------------------
class Camera:public OrthoTransform
{
   public:
     static int aspect;
     static double inv_aspect;
     Camera();
     virtual ~Camera();
     // Return polygon in front, in direction 'v' (world space coordinates)
     Polygon3d* GetHit (Vector3d& v);

     static int GetFOV() { return aspect; }
     static void SetFOV(int fov)
       { aspect = fov; inv_aspect = 1.0/aspect; }


     // Set sector that camera is in. Doesn't check if it's in that sector
     void SetSector(Sector *s) { sector = s; }
     Sector* GetSector() { return sector; }
	 const Sector* GetSector() const { return sector; }

     // Set abs pos of camera in sector. Doesn't check if it's in that sector
     // v is world coords
     inline void SetPosition(const Vector3d& v)
       { SetOrigin (v); }

     // Move relative world coordinates.
     void MoveWorld (const Vector3d& v);

     // Move relative camera coordinates.
     void Move (const Vector3d& v)
       { MoveWorld (m_t2o * v); }

     // Rotate camera by angle (radians) around the vector in world coords
     void RotateWorld(const Vector3d& v, double angle);

     // Rotate camera by angle (radians) around the vector in camera coords
     void Rotate(const Vector3d& v, double angle);

     // ReOrient Camera world space
     void RotateWorld(Matrix& m)
       { SetT2O (m * m_t2o); }

     // ReOrient Camera Camera space
     void Rotate(Matrix& m);

     // Look @x,y,z using up as up vector
     void LookAt(const Vector3d& v,const Vector3d& up);

//   * Set the world to camera transformation matrix.
//   * This basicly defines the direction that the camera looks.
  inline void SetW2C (const Matrix& m) { SetO2T (m); }

//   * Set the camera to world transformation matrix.
//   * This basicly defines the direction that the camera looks.
  inline void SetC2W (const Matrix& m) { SetT2O (m); }

//   * Return the world to camera transformation matrix.
  inline Matrix GetW2C () const { return GetO2T (); }

//   * Return the camera to world transformation matrix.
  inline Matrix GetC2W () const { return GetT2O (); }

//   * Return the world to camera translation.
  inline Vector3d GetW2CTranslation () const { return GetO2TTranslation (); }

//   * Transform a worldspace point to camera space.
  inline Vector3d World2Camera (const Vector3d& v) const { return Other2This (v); }

//   * Transform a camera space point to world space.
  inline Vector3d Camera2World (const Vector3d& v) const { return This2Other (v); }

//   * Transform a camera space point to worldspace, relative to camera position.
  inline Vector3d Camera2WorldRelative (const Vector3d& v) const { return This2OtherRelative (v); }

   protected:
     // The sector the camera is in.
     Sector* sector;
};

#endif

