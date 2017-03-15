#include "opengl.h"
#include "Vec3f.h"
#include <time.h>
#include "Animation.h"
#include <cassert>


const int NBPB=10;              // NomBre de Points de Bezier (entre 2 points de Contr il y aura NPB points)


float temps()
{
    return float(clock())/CLOCKS_PER_SEC;
}


/** Fonction de calcul
 pc[0..3] sont les 4 points de controles
 t  le  parametre
 p le  point resultat de la courbe
*/
Vec3f Bezier(const Vec3f& p0, const Vec3f& p1, const Vec3f& p2, const Vec3f& p3, const float t)
{
    float t1,t13,t3;
    Vec3f p, r0, r1, r2, r3;
    t1 = 1 - t;
    t13 = t1 * t1 * t1;
    t3 = t * t * t;
    //p = t13*p0 + 3*t*t1*t1*p1 + 3*t*t*t1*p2 + t3*p3;

    vecMul( r0, t13, p0);
    vecMul( r1, 3*t*t1*t1, p1);
    vecMul( r2, 3*t*t*t1, p2);
    vecMul( r3, t3, p3);
    vecAdd( p, r0, r1);
    vecAdd( p, p, r2);
    vecAdd( p, p, r3);
    return p;
}


void animInit(Animation& a, const char* nom_fichier)
{
	const int taille=512;
	char txt[taille];
	FILE* f;

    f = fopen( nom_fichier, "r");
    assert( f );

    // donnée temporaire (=point de controle PC) servant pour calculer les points finaux (P)
    int NBPC;
    Vec3f* PC;

   	do { fgets( txt,taille,f); } while( txt[0]=='#' );
   	assert( sscanf( txt, "%d\n", &NBPC)==1 );
    a.nbp = (NBPC/4)*NBPB;	// Nombre de points en tout

    PC = new Vec3f[NBPC];
    a.P = new Vec3f[a.nbp];
    int i;
    for(i=0;i<NBPC;++i)
    {
        do { fgets( txt,taille,f); } while( txt[0]=='#' );
        assert( sscanf( txt, "%f %f %f\n", &PC[i].x, &PC[i].y, &PC[i].z)==3 );
    }

    printf("Animation: %d PdC et %d P en tout\n", NBPC, a.nbp);
    //for(i=0;i<NBPC;++i) vecPrint( a.P[i] );


    // Courbe de Bezier
    int c=0, j;
    for (i=0;i<NBPC;i+=4)                   // Pour calculer une courbe de Bezier, on regroupe les points de controle par 4
    {
        for (j=0;j<NBPB;j++)
        {
            a.P[c] = Bezier( PC[i], PC[(i+1)%NBPC], PC[(i+2)%NBPC], PC[(i+3)%NBPC],  float(j)/(NBPB) );
            c++;
        }
    }
    delete[] PC;
}


void animDraw(const Animation& a)
{
    int i;
    glPushAttrib( GL_ENABLE_BIT );
    glLineWidth(5);
    glDisable(GL_LIGHTING );
    glDisable(GL_TEXTURE_2D);
    glColor4f(1,0,0,1);
    glBegin(GL_LINE_LOOP);
    for (i=0;i<a.nbp;i++)
    {
        //printf("%f %f %f\n", P[i].x, P[i].y, P[i].z );
        glVertex3f( a.P[i].x, a.P[i].y, a.P[i].z );
    }
    glEnd();
    glPopAttrib();
}
