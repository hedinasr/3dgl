
#ifndef _ANIMATION_H_LIF041
#define _ANIMATION_H_LIF041

#include "Vec3f.h"


//! un tableau dynamique contenant les points d'une courbe d'animation
struct Animation
{
    //! Les points de la trajectoire/courbe d'animation que va suivre la fusée
    Vec3f* P;
    //! Nombre de point
    int nbp;
};


//! renvoie le temps passé depuis le demarrage
float temps();

//! initialise la courbe d'animation avec une liste de points de C du fichier "nom_fichier"
void animInit(Animation& a, const char* nom_fichier);

//! dessine la coure d'animation
void animDraw(const Animation& a);


	// VOICI un exemple de comment animer un objet : ceci est à mettre dans la fonction sceneDraw(...)
	//
//    glPushMatrix();
//    Vec3f pos ;
//    float t = temps();
//    int temps_entier = int(t);
//    int temps_entier_ok = temps_entier % sc.anim.nbp ;
//    pos = sc.anim.P[temps_entier_ok] ;
//    glTranslatef( pos.x, pos.y, pos.z ) ;
//    draw_plane();
//    glPopMatrix();


#endif
