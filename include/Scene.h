
#ifndef _SCENE_H
#define _SCENE_H

#include "Animation.h"
#include "Image.h"

//! The structure Scene stores all texture Id or images used by the sceneDraw fonction
struct Scene
{
    Animation anim;
	unsigned int tex_papillon;

	Image terrain;
	unsigned int tex_terrain;
    
    unsigned int tex_arbre;
    
    unsigned int tex_eau;
 
    unsigned int tex_cube;
};

void draw_arbre();
//dessine les arbres

//! Dessine un terrain
void terraindraw(const Image & im, const Scene & sc);

//! Dessine les normales en chaque points du terrain
void terrainNormal(const Image & im, const int i, const int j);

//! Init a Scene
void sceneInit(Scene& sc);

//! Draw the scene
void sceneDraw(const Scene& sc);


//! Draw a cube
void draw_cube();

//! Draw a cylinder
void draw_cylinder();

//! Draw a cone
void draw_cone();

//! Draw a sphere
void draw_sphere();

//! Draw the X,Y,Z axes
void draw_axes();

//! Draw a grid in the XZ plane
void draw_grid();

//! Dessine le dessin que je veux =)
void dessine_();


#endif
