#ifndef _VEC3F_H
#define _VEC3F_H

//! A vector is defined by 3 coord (X,Y,Z)
struct Vec3f {
  float x, y, z;
};

//! degree -> radian
float rad(const float a);

//! print Vec3f
void vecPrint(const Vec3f &v);

//! v=(x,y,z)
void vecInit(Vec3f &v, const float x, const float y, const float z);

//! spherical coord -> (x,y,z)
void vecInit(Vec3f &v, const float a, const float b);

//! R=A+B
void vecAdd(Vec3f &R, const Vec3f &A, const Vec3f &B);

//! R=A-B
void vecSub(Vec3f &R, const Vec3f &A, const Vec3f &B);

//! R = a*B
void vecMul(Vec3f &R, const float a, const Vec3f &B);

//! Scalar product
float vecDot(const Vec3f &A, const Vec3f &B);

//! Cross product (produit vectoriel)
void vecCross(Vec3f &R, const Vec3f &A, const Vec3f &B);

//! normalize the vector
void vecNormalize(Vec3f &v);

//! return the length of the vector
float vecLength(const Vec3f &v);

#endif
