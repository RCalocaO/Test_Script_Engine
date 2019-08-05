// Matrix Class 3D Engine
// (C) 1999 by A'r-Ci-O'oh (rco@hotmail.com)

#include "matrix.h"
#include <time.h>

Matrix::Matrix()
{
   m12=m13=m21=m23=m31=m32=0.0;
   m11=m22=m33=1.0;
}

Matrix::Matrix(double m11,double m12,double m13,double m21,double m22,
           double m23,double m31,double m32,double m33)
{
  Matrix::m11=m11; Matrix::m12=m12; Matrix::m13=m13;
  Matrix::m21=m21; Matrix::m22=m22; Matrix::m23=m23;
  Matrix::m31=m31; Matrix::m32=m32; Matrix::m33=m33;
}

Matrix& Matrix::operator+=(const Matrix& m)
{
  m11+=m.m11; m12+=m.m12; m13+=m.m13;
  m21+=m.m11; m22+=m.m22; m23+=m.m23;
  m31+=m.m11; m32+=m.m32; m33+=m.m33;
  return *this;
}

Matrix& Matrix::operator-=(const Matrix& m)
{
  m11-=m.m11; m12-=m.m12; m13-=m.m13;
  m21-=m.m11; m22-=m.m22; m23-=m.m23;
  m31-=m.m11; m32-=m.m32; m33-=m.m33;
  return *this;
}

Matrix& Matrix::operator*=(const Matrix& m)
{
  Matrix r;
  r.m11=m11*m.m11+m12*m.m21+m13*m.m31;
  r.m12=m11*m.m12+m12*m.m22+m13*m.m32;
  r.m13=m11*m.m13+m12*m.m23+m13*m.m33;
  r.m21=m21*m.m11+m22*m.m21+m23*m.m31;
  r.m22=m21*m.m12+m22*m.m22+m23*m.m32;
  r.m23=m21*m.m13+m22*m.m23+m23*m.m33;
  r.m31=m31*m.m11+m32*m.m21+m33*m.m31;
  r.m32=m31*m.m12+m32*m.m22+m33*m.m32;
  r.m33=m31*m.m13+m32*m.m23+m33*m.m33;
  return *this;
}

Matrix& Matrix::operator*=(double scalar)
{
  m11*=scalar; m12*=scalar; m13*=scalar;
  m21*=scalar; m22*=scalar; m23*=scalar;
  m31*=scalar; m32*=scalar; m33*=scalar;
  return *this;
}

Matrix& Matrix::operator/=(double scalar)
{
  m11/=scalar; m12/=scalar; m13/=scalar;
  m21/=scalar; m22/=scalar; m23/=scalar;
  m31/=scalar; m32/=scalar; m33/=scalar;
  return *this;
}

void Matrix::Identity()
{
   m12=m13=m21=m23=m31=m32=0.0;
   m11=m22=m33=1.0;
}

void Matrix::Transpose()
{
  double swap;
  swap=m12; m12=m21; m21=swap;
  swap=m13; m13=m31; m31=swap;
  swap=m23; m23=m32; m32=swap;
}

Matrix Matrix::GetTranspose() const
{
  Matrix t;
  t.m12=m21; t.m21=m12;
  t.m13=m31; t.m31=m13;
  t.m23=m32; t.m32=m23;
  t.m11=m11; t.m22=m22; t.m33=m33;
  return t;
}

double Matrix::Determinant() const
{
   return (m11*(m22*m33-m23*m32)-m12*(m21*m33-m23*m31)+m13*(m21*m32-m22*m31));
}

Matrix operator+(const Matrix& m1,const Matrix& m2)
{
  return Matrix (m1.m11+m2.m11,m1.m12+m2.m12,m1.m13+m2.m13,
                 m1.m21+m2.m21,m1.m22+m2.m22,m1.m23+m2.m23,
                 m1.m31+m2.m31,m1.m32+m2.m32,m1.m33+m2.m33);
}

Matrix operator-(const Matrix& m1,const Matrix& m2)
{
  return Matrix (m1.m11-m2.m11,m1.m12-m2.m12,m1.m13-m2.m13,
                 m1.m21-m2.m21,m1.m22-m2.m22,m1.m23-m2.m23,
                 m1.m31-m2.m31,m1.m32-m2.m32,m1.m33-m2.m33);
}

Matrix operator*(const Matrix& m1,const Matrix& m2)
{
  return Matrix(
                 m1.m11*m2.m11 + m1.m12*m2.m21 + m1.m13*m2.m31,
                 m1.m11*m2.m12 + m1.m12*m2.m22 + m1.m13*m2.m32,
                 m1.m11*m2.m13 + m1.m12*m2.m23 + m1.m13*m2.m33,
                 m1.m21*m2.m11 + m1.m22*m2.m21 + m1.m23*m2.m31,
                 m1.m21*m2.m12 + m1.m22*m2.m22 + m1.m23*m2.m32,
                 m1.m21*m2.m13 + m1.m22*m2.m23 + m1.m23*m2.m33,
                 m1.m31*m2.m11 + m1.m32*m2.m21 + m1.m33*m2.m31,
                 m1.m31*m2.m12 + m1.m32*m2.m22 + m1.m33*m2.m32,
                 m1.m31*m2.m13 + m1.m32*m2.m23 + m1.m33*m2.m33
               );
}

Matrix operator*(const Matrix& m,double f)
{
  return Matrix (m.m11*f, m.m12*f, m.m13*f,
                 m.m21*f, m.m22*f, m.m23*f,
                 m.m31*f, m.m32*f, m.m33*f);
}

Matrix operator*(double f,const Matrix& m)
{
  return Matrix (m.m11*f, m.m12*f, m.m13*f,
                 m.m21*f, m.m22*f, m.m23*f,
                 m.m31*f, m.m32*f, m.m33*f);
}

Matrix operator/(const Matrix& m,double f)
{
  return Matrix (m.m11/f, m.m12/f, m.m13/f,
                 m.m21/f, m.m22/f, m.m23/f,
                 m.m31/f, m.m32/f, m.m33/f);
}

bool operator==(const Matrix& m1,const Matrix& m2)
{
  if (m1.m11 != m2.m11 || m1.m12 != m2.m12 || m1.m13 != m2.m13) return false;
  if (m1.m21 != m2.m21 || m1.m22 != m2.m22 || m1.m23 != m2.m23) return false;
  if (m1.m31 != m2.m31 || m1.m32 != m2.m32 || m1.m33 != m2.m33) return false;
  return true;
}

bool operator!=(const Matrix& m1,const Matrix& m2)
{
  if (m1.m11 != m2.m11 || m1.m12 != m2.m12 || m1.m13 != m2.m13) return true;
  if (m1.m21 != m2.m21 || m1.m22 != m2.m22 || m1.m23 != m2.m23) return true;
  if (m1.m31 != m2.m31 || m1.m32 != m2.m32 || m1.m33 != m2.m33) return true;
  return false;
}

XRotMatrix::XRotMatrix(double angle)
{
   m11 = 1; m12 = 0;           m13 = 0;
   m21 = 0; m22 = cos (angle); m23 = -sin(angle);
   m31 = 0; m32 = sin (angle); m33 =  cos(angle);
}

YRotMatrix::YRotMatrix(double angle)
{
   m11 = cos (angle); m12 = 0; m13 = -sin(angle);
   m21 = 0;           m22 = 1; m23 = 0;
   m31 = sin (angle); m32 = 0; m33 =  cos(angle);
}

ZRotMatrix::ZRotMatrix(double angle)
{
   m11 = cos (angle); m12 = -sin(angle); m13 = 0;
   m21 = sin (angle); m22 =  cos(angle); m23 = 0;
   m31 = 0;           m32 = 0;           m33 = 1;
}

BITMAP *video=NULL;
bool DEBUG=false;
long StartTime,EndTime,FrameCount=0;

void CleanUp()
{
#if 0
   destroy_bitmap(video);
   // Set SCreen to Text
   set_gfx_mode(GFX_TEXT,80,25,0,0);
   // FPS
   cout << "Frames per Second: " << (double)FrameCount*CLK_TCK/(double)(EndTime-StartTime) << endl;
#endif
}


