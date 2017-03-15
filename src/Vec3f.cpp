

#include "Vec3f.h"
#include <math.h>
#include <cstdio>
using namespace std;

float rad(const float a)
{
    return a*2.0f*M_PI/360.f;
}


void vecPrint(const Vec3f& v)
{
    printf("(%f,%f,%f)\n", v.x, v.y, v.z);
}


void vecInit(Vec3f& v, const float x, const float y, const float z)
{
    v.x = x;
    v.y = y;
    v.z = z;
}


void vecInit(Vec3f& v, const float a, const float b)
{
    v.x = sin(a)*cos(b);
    v.y = sin(a)*sin(b);
    v.z = cos(a);
}


void vecAdd(Vec3f& R, const Vec3f& A, const Vec3f& B)
{
    R.x = A.x+B.x;
    R.y = A.y+B.y;
    R.z = A.z+B.z;
}


void vecSub(Vec3f& R, const Vec3f& A, const Vec3f& B)
{
    R.x = A.x-B.x;
    R.y = A.y-B.y;
    R.z = A.z-B.z;
}


void vecMul(Vec3f& R, const float a, const Vec3f& B)
{
    R.x = a*B.x;
    R.y = a*B.y;
    R.z = a*B.z;
}


float vecDot(const Vec3f& A, const Vec3f& B)
{
    return A.x*B.x + A.y*B.y + A.z*B.z;
}


void vecCross(Vec3f& R, const Vec3f& A, const Vec3f& B)
{
    R.x = A.y*B.z-A.z*B.y;
    R.y = A.z*B.x-A.x*B.z;
    R.z = A.x*B.y-A.y*B.x;
}


void vecNormalize(Vec3f& v)
{
    float n=sqrt( v.x*v.x + v.y*v.y + v.z*v.z);
    v.x /= n;
    v.y /= n;
    v.z /= n;
}

float vecLength(const Vec3f& v)
{
    return sqrt( v.x*v.x + v.y*v.y + v.z*v.z);
}
