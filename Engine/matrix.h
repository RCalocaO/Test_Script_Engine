// Matrix Class for Trohs 3D Engine
// (C) 1999 by A'r-Ci-O'oh (rco@hotmail.com)

// Description:
// This file contains the Matrix Class, a 3x3 Matrix

#ifndef __MATRIX_H__
#define __MATRIX_H__


class Matrix;
#include "../trohs3d.h"
#include "vector3d.h"

class Matrix
{
  public:
    double m11,m12,m13,
           m21,m22,m23,
           m31,m32,m33;
    // Identity
    Matrix();

    Matrix(double m11,double m12,double m13,double m21,double m22,
           double m23,double m31,double m32,double m33);

    inline void Set(double m11,double m12,double m13,double m21,double m22,
             double m23,double m31,double m32,double m33)
      {
        Matrix::m11=m11; Matrix::m12=m12; Matrix::m13=m13;
        Matrix::m21=m21; Matrix::m22=m22; Matrix::m23=m23;
        Matrix::m31=m31; Matrix::m32=m32; Matrix::m33=m33;
      }

    Matrix& operator+=(const Matrix& m);

    Matrix& operator-=(const Matrix& m);

    Matrix& operator*=(const Matrix& m);

    Matrix& operator*=(double scalar);

    Matrix& operator/=(double scalar);

    Matrix& operator+() { return *this; };

    Matrix& operator-() const
      {  Matrix(-m11,-m12,-m13,-m21,-m22,-m23,-m31,-m32,-m33); };

    void Transpose();

    Matrix GetTranspose() const;

    Matrix GetInverse() const
      {
        Matrix C(
          (m22*m33-m23*m32),-(m12*m33-m13*m32),(m12*m23-m13*m22),
          -(m21*m33-m23*m31),(m11*m33-m13*m31),-(m11*m23-m13*m21),
          (m21*m32-m22*m31),-(m11*m32-m12*m31),(m11*m22-m12*m21)
                );
        double s=(double)1.0/(m11*C.m11+m12*C.m21+m13*C.m31);
        C*=s;
        return C;
      };

    void Invert() { *this=GetInverse(); };

    double Determinant() const;

    void Identity();

    friend Matrix operator+(const Matrix& v1,const Matrix& v2);

    friend Matrix operator-(const Matrix& v1,const Matrix& v2);

    friend Matrix operator*(const Matrix& v1,const Matrix& v2);

    friend Vector3d operator*(const Matrix& m,const Vector3d& v)
      {
        return Vector3d(m.m11*v.x+m.m12*v.y+m.m13*v.z,
                      m.m21*v.x+m.m22*v.y+m.m23*v.z,
                      m.m31*v.x+m.m32*v.y+m.m33*v.z
                     );
      }
    friend Matrix operator*(const Matrix& m,double v);

    friend Matrix operator*(double v,const Matrix& m);

    friend Matrix operator/(const Matrix& m,double v);

    friend bool operator==(const Matrix& m1,const Matrix& m2);

    friend bool operator!=(const Matrix& m1,const Matrix& m2);
};

class XRotMatrix:public Matrix
{
  public:
    XRotMatrix(double angle);
};

class YRotMatrix:public Matrix
{
  public:
    YRotMatrix(double angle);
};

class ZRotMatrix:public Matrix
{
  public:
    ZRotMatrix(double angle);
};


#endif