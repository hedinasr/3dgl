#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include "Vec3f.h"

enum Camera_FlyTrackball { CAMERA_FLY=0, CAMERA_TRACKBALL=1 };

//! A camera is defined by a position and 2 angles of rotations (defining the orientation)
struct Camera
{
    Vec3f pos;
    float rotLeft;
    float rotUp;
    int mode;       // 0=neutral, 1=rotation mode, 2=translation mode
    Camera_FlyTrackball flyOrTrackball;
    int mx,my;      // mouse position
};


inline void camInit(Camera& cam, const Camera_FlyTrackball ft=CAMERA_TRACKBALL)
{
    cam.pos.x = 0;
    cam.pos.y = 2;
    cam.pos.z = 10;
    cam.rotUp = 0;
    cam.rotLeft = 0;
    cam.mode = 0;
    cam.flyOrTrackball = ft;

}


inline void camComputeDirFront( const Camera& cam, Vec3f& dir)
{
    if (cam.flyOrTrackball==CAMERA_FLY)
    {
        float a = rad(cam.rotUp);
        float b = rad(cam.rotLeft);
        int signz=(cam.pos.z>0)?1:-1;
        int signy=(cam.pos.y>0)?1:-1;
        vecInit( dir, sin(b), -signz*signy*sin(a)*cos(b), -cos(a)*cos(b));
    }
    else
    {
        vecInit( dir, -cam.pos.x, -cam.pos.y, -cam.pos.z);
        vecNormalize( dir );
    }
//    glMatrixMode(GL_MODELVIEW);
//    glPushMatrix();
//    camSetGL( cam );
//    float m[16];
//    glGetFloatv( GL_MODELVIEW_MATRIX, m);
//    vecInit( dir, m[2], m[6], m[10]);
//    glPopMatrix();

}

inline void camSetFlyTrackballMode(Camera& cam, const Camera_FlyTrackball m)
{
//    cam.flyOrTrackball = m;
    camInit( cam, m);
}

inline void camSetGL(const Camera& cam)
{

//Vec3f dir;
//camComputeDirFront(cam, dir);
//vecPrint(dir);
//printf("%f %f\n\n", cam.rotUp, cam.rotLeft);

    glMatrixMode( GL_MODELVIEW);

    if (cam.flyOrTrackball==CAMERA_FLY)
    {
        glRotatef( cam.rotUp, 1.0f, 0, 0);
        glRotatef( cam.rotLeft, 0, 1.0f, 0);
        glTranslatef( -cam.pos.x, -cam.pos.y, -cam.pos.z);
    }
    else
    {
        glTranslatef( -cam.pos.x, -cam.pos.y, -cam.pos.z);
        glRotatef( cam.rotUp, 1.0f, 0, 0);
        glRotatef( cam.rotLeft, 0, 1.0f, 0);
    }
}

inline void camTurnUpDown(Camera& cam, const float stepR=2.f)
{
    cam.rotUp += stepR;
}
inline void camTurnLeftRight(Camera& cam, const float stepR=2.f)
{
    cam.rotLeft += stepR;
}


enum DIR { FRONT, BACK, LEFT, RIGHT, UP, DOWN };
inline void camTranslate(Camera& cam, const DIR where, const float c=1.f)
{
    Vec3f dir, up, right;

    camComputeDirFront( cam, dir);

    //vecInit( dir, a, b);
    if (dir.y>=1.f)
        vecInit( up, 1, 0, 0);
    else
        vecInit( up, 0, 1, 0);
    vecCross( right, dir, up );

    Vec3f tr, res;
    vecInit( tr, 0,0,0);
    switch (where)
    {
    case UP:
        vecMul( tr, c, up);
        break;

    case DOWN:
        vecMul( tr, -c, up);
        break;

    case FRONT:
        vecMul( tr, c, dir);
        break;

    case BACK:
        vecMul( tr, -c, dir);
        break;

    case LEFT:
        vecMul( tr, -c, right);
        break;

    case RIGHT:
        vecMul( tr, c, right);
        break;
    }
    //printf("%f %f %f\n", cam.pos.x, cam.pos.y, cam.pos.z);
    vecAdd( res, cam.pos, tr);
    if (vecLength(res)>1.f) cam.pos=res;
}

enum CAMERA_MODE { CAMERA_STOP, CAMERA_ROTATION, CAMERA_TRANSLATION };
inline void camStartMove( Camera& cam, const CAMERA_MODE mode, const int x, const int y)
{
    cam.mode = mode;
    cam.mx = x;
    cam.my = y;
}

inline void camMove( Camera& cam, const int x, const int y)
{
    if (cam.mode==CAMERA_TRANSLATION)
    {
        if (y>cam.my) camTranslate( cam, BACK);
        else camTranslate( cam, FRONT);
    }
    else if (cam.mode==CAMERA_ROTATION)
    {
//        if (abs(y-cam.my)>3)
        camTurnUpDown( cam, 0.5f*(y-cam.my));
//        if (abs(x-cam.mx)>3)
        camTurnLeftRight( cam, 0.5f*(x-cam.mx));
    }
    cam.mx = x;
    cam.my = y;
}


#endif // CAMERA_H_INCLUDED
