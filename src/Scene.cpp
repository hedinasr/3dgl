#include "Scene.h"
#include "opengl.h"
#include <GL/glut.h>
#include <time.h>

void draw_axes() {
  glLineWidth(5);
  glBegin(GL_LINES);

  glColor3f(1.f, 0.f, 0.f);
  glVertex3f(0.f, 0.f, 0.f);
  glVertex3f(1.f, 0.f, 0.f);

  glColor3f(0.f, 1.f, 0.f);
  glVertex3f(0.f, 0.f, 0.f);
  glVertex3f(0.f, 1.f, 0.f);

  glColor3f(0.f, 0.f, 1.f);
  glVertex3f(0.f, 0.f, 0.f);
  glVertex3f(0.f, 0.f, 1.f);

  glEnd();
}

void draw_grid() {
  int i;
  glLineWidth(1);
  glColor3f(1.f, 1.f, 1.f);

  glPushMatrix();
  glTranslatef(-5, 0, -5);

  glBegin(GL_LINES);

  for (i = 0; i <= 10; ++i) {
    glVertex3f(i, 0, 0);
    glVertex3f(i, 0, 10);
  }

  for (i = 0; i <= 10; ++i) {
    glVertex3f(0, 0, i);
    glVertex3f(10, 0, i);
  }

  glEnd();
  glPopMatrix();
}

void draw_cube() {
  static float pt[8][3] = {{0, 0, 0}, {1, 0, 0}, {1, 0, 1}, {0, 0, 1},
                           {0, 1, 0}, {1, 1, 0}, {1, 1, 1}, {0, 1, 1}};
  static int f[6][4] = {{0, 1, 2, 3}, {5, 4, 7, 6}, {1, 5, 6, 2},
                        {0, 3, 7, 4}, {3, 2, 6, 7}, {0, 4, 5, 1}};
  static float n[6][3] = {{0, -1, 0}, {0, 1, 0}, {1, 0, 0},
                          {-1, 0, 0}, {0, 0, 1}, {0, 0, -1}};
  static float uv[6][4][2] = {
      {{0, 0}, {1, 0}, {1, 1}, {0, 1}}, {{0, 0}, {1, 0}, {1, 1}, {0, 1}},
      {{0, 0}, {1, 0}, {1, 1}, {0, 1}}, {{0, 0}, {1, 0}, {1, 1}, {0, 1}},
      {{0, 0}, {1, 0}, {1, 1}, {0, 1}}, {{0, 0}, {1, 0}, {1, 1}, {0, 1}}};
  int i, j;

  glTranslatef(-0.5, -0.5, -0.5);
  glBegin(GL_QUADS);
  for (i = 0; i < 6; i++) {
    glNormal3f(n[i][0], n[i][1], n[i][2]);
    for (j = 0; j < 4; j++) {
      glTexCoord2f(uv[i][j][0], uv[i][j][1]);
      glVertex3f(pt[f[i][j]][0], pt[f[i][j]][1], pt[f[i][j]][2]);
    }
  }
  glEnd();
}

void draw_sphere() {
  int divBeta = 80;
  int divAlpha = divBeta / 2;
  float beta, alpha, alpha2;
  glBegin(GL_TRIANGLE_STRIP);
  for (int i = 0; i <= divAlpha; i++) {
    alpha = -0.5 * M_PI + float(i) * M_PI / divAlpha;
    alpha2 = -0.5 * M_PI + float(i + 1) * M_PI / divAlpha;
    for (int j = 0; j < divBeta; j++) {
      beta = float(j) * 2 * M_PI / (divAlpha - 1);

      glNormal3f(cos(alpha) * cos(beta), sin(alpha), cos(alpha) * sin(beta));
      // glTexCoord2f(j/alpha,0); A faire
      glVertex3f(cos(alpha) * cos(beta), sin(alpha), cos(alpha) * sin(beta));

      glNormal3f(cos(alpha2) * cos(beta), sin(alpha2), cos(alpha) * sin(beta));
      // glTexCoord2f(j/alpha2,1); A faire
      glVertex3f(cos(alpha2) * cos(beta), sin(alpha2), cos(alpha2) * sin(beta));
    }
  }
  glEnd();
}

void draw_cone() {
  int div = 50;
  float beta;
  float step = 2 * M_PI / div;
  glBegin(GL_TRIANGLE_STRIP);
  for (int i = 0; i <= div; i++) {
    beta = i * step;
    glVertex3f(0, 2, 0);
    glNormal3f(sin(beta), 0, cos(beta));
    glVertex3f(sin(beta), 0, cos(beta));
  }
  // Dessous du cercle
  glNormal3f(0, 0, 0);
  for (int i = 0; i <= div; i++) {
    beta = i * step;
    glVertex3f(0, 0, 0);
    glVertex3f(sin(beta), 0, cos(beta));
  }
  glEnd();
}

void draw_cone_(float h, float R1, float R2) {
  int div = 40;
  float beta, step = 2 * M_PI / div;
  glBegin(GL_TRIANGLE_STRIP);
  glNormal3f(0, h, 0);
  for (int i = 0; i <= div; i++) {
    beta = i * step;
    glVertex3f(0, h, 0);
    glVertex3f(sin(beta) * R2, h, cos(beta) * R2);
  }
  for (int i = 0; i <= div; i++) {
    beta = i * step;
    glNormal3f(sin(beta) * R2, h, cos(beta) * R2);
    glVertex3f(sin(beta) * R2, h, cos(beta) * R2);
    glNormal3f(sin(beta) * R1, 0, cos(beta) * R1);
    glVertex3f(sin(beta) * R1, 0, cos(beta) * R1);
  }
  glNormal3f(0, 0, 0);
  for (int i = 0; i <= div; i++) {
    beta = i * step;
    glVertex3f(sin(beta) * R1, 0, cos(beta) * R1);
    glVertex3f(0, 0, 0);
  }
  glEnd();
}

void draw_cylinder() {
  int div = 40;
  float x = 1;
  float beta;
  float step = 2 * M_PI / div;
  glBegin(GL_QUAD_STRIP);
  // glNormal3f(0, 2, 0);
  // Disque du haut
  /*for(int i=0;i<=div;i++)
  {
      beta=i*step;
      glVertex3f(0, 2, 0);
      glVertex3f(sin(beta)*x ,2, cos(beta)*x);

  }*/
  for (int i = 0; i <= div; i++) {
    beta = i * step;
    glNormal3f(sin(beta) * x, 0, cos(beta) * x);
    // glTexCoord2f(float(beta/2*M_PI),1);
    // glPushMatrix();
    // glTranslatef(0.5, 1, 0);
    glVertex3f(sin(beta) * x + 1, 2, cos(beta) * x + 3);
    // glTexCoord2f(float(beta/2*M_PI),0);
    glVertex3f(sin(beta) * x, 0, cos(beta) * x);
  }

  /*glNormal3f(0, 0, 0);
  //Disque du bas
  for(int i=0;i<=div;i++)
  {
      beta=i*step;
      glVertex3f(sin(beta)*x, 0, cos(beta)*x);
      glVertex3f(0, 0, 0);
  }*/
  glEnd();
}

void draw_quad() {
  glBegin(GL_TRIANGLE_STRIP);

  glNormal3f(0, 0, 1);
  glTexCoord2f(0, 0);
  glVertex3f(-0.5, 0, 0);

  glNormal3f(0, 0, 0);
  glTexCoord2f(1, 0);
  glVertex3f(0.5, 0, 0);

  glNormal3f(0, 1, 1);
  glTexCoord2f(0, 1);
  glVertex3f(-0.5, 1, 0);

  glNormal3f(0, 1, 0);
  glTexCoord2f(1, 1);
  glVertex3f(0.5, 1, 0);

  glEnd();
}

void draw_arbre() {
  for (int i = 0; i < 250; i = i + 50) {
    glPushMatrix();
    glColor3f(1, 1, 1);
    draw_quad();
    glRotatef(90 + i, 0, 1, 0);
    draw_quad();
    glPopMatrix();
  }
}

void dessine_() {
  glPushMatrix();
  glTranslatef(0, 2, 8);
  glColor3f(1, 0, 0);
  glScalef(2, 2, 2);
  draw_cone();
  glPopMatrix();
  glPushMatrix();
  glTranslatef(5, 2, 0);
  // glRotatef(45, 1, 0, 0);
  glColor3f(1, 0, 0);
  draw_cylinder();
  glPopMatrix();
  glPushMatrix();
  glTranslatef(0, 4, 0);
  glColor3f(1, 0, 1);
  draw_sphere();
  glPopMatrix();
  glPushMatrix();
  glTranslatef(0, 5.5, 0);
  glScalef(0.8, 0.8, 0.8);
  glColor3f(1, 0, 1);
  draw_sphere();
  glPopMatrix();
  glPushMatrix();
  glTranslatef(0, 5, 0);
  draw_cone_(3, 1, 0.5);
  glPopMatrix();
  /*
  glPushMatrix();
  glColor3f(1, 1, 1);
  glTranslatef(0, 10, 0);
  draw_arbre(sc.terrain);
  glRotatef(90, 0, 1, 0);
  draw_arbre();
  glPopMatrix();
   */

  /*
  glPushMatrix();
  draw_arbre();
  glPopMatrix();
  */
}

void terrainNormal(const Image &im, const int i, const int j) {
  Vec3f n;
  Vec3f a, b, c, d, ab, cd; // Les quattres points autour du point (i,j)
  vecInit(a, i - 1, getPixConst(im, i - 1, j), j);
  vecInit(b, i + 1, getPixConst(im, i + 1, j), j);
  vecInit(c, i, getPixConst(im, i, j - 1), j - 1);
  vecInit(d, i, getPixConst(im, i, j + 1), j + 1);
  vecSub(ab, b, a);
  vecSub(cd, d, c);

  // Attention au bord du terrain
  vecCross(n, cd, ab); // Pour le produit vectoriel
  glNormal3f(n.x, n.y, n.z);
}

void terraindraw(const Image &im, const Scene &sc) {

  srand(12); // La fonction srand permet d'initialiser le générateur de nombres
             // pseudo-alétoires avec une graine différente;
  int x = 1;
  glColor3f(1, 1, 1);
  for (int i = 1; i < imGetDimX(im) - 2; i++) {
    glBegin(GL_TRIANGLE_STRIP);
    for (int j = 1; j < imGetDimY(im) - 1; j++) {
      terrainNormal(im, i,
                    j); // Calcul de la normal en i,j et fait le glNormal3f(..)
      glTexCoord2f(float(i) / imGetDimX(im), float(j) / imGetDimY(im));
      glVertex3f(i / x, getPixConst(im, i, j), j / x);
      terrainNormal(im, i + 1, j);
      glTexCoord2f(float(i + 1) / imGetDimX(im), float(j) / imGetDimY(im));
      glVertex3f(i + 1 / x, getPixConst(im, i + 1, j), j / x);
    }
    glEnd();
  }
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, sc.tex_arbre);
  int x1 = 0, z = 0;
  for (int i = 0; i < 100; i++) {
    x1 = (rand() % imGetDimX(im));
    z = (rand() % imGetDimY(im));
    if (getPixConst(im, x1, z) > 50) {
      glPushMatrix();
      glTranslatef(x1, getPixConst(im, x1, z), z);
      glScalef(10, 100, 10);
      draw_arbre();
    }
  }
  glDisable(GL_TEXTURE_2D);
}

void draw_cube_map() {
  static float pt[8][3] = {{0, 0, 0}, {1, 0, 0}, {1, 0, 1}, {0, 0, 1},
                           {0, 1, 0}, {1, 1, 0}, {1, 1, 1}, {0, 1, 1}};
  static int f[6][4] = {{0, 1, 2, 3}, {5, 4, 7, 6}, {1, 5, 6, 2},
                        {0, 3, 7, 4}, {3, 2, 6, 7}, {0, 4, 5, 1}};
  static float n[6][3] = {{0, -1, 0}, {0, 1, 0}, {1, 0, 0},
                          {-1, 0, 0}, {0, 0, 1}, {0, 0, -1}};
  static float uv[6][4][2] = {
      {{0, 0}, {1, 0}, {1, 1}, {0, 1}}, {{0, 0}, {1, 0}, {1, 1}, {0, 1}},
      {{0, 0}, {1, 0}, {1, 1}, {0, 1}}, {{0, 0}, {1, 0}, {1, 1}, {0, 1}},
      {{0, 0}, {1, 0}, {1, 1}, {0, 1}}, {{0, 0}, {1, 0}, {1, 1}, {0, 1}}};
  int i, j;

  glTranslatef(-0.5, -0.5, -0.5);
  glBegin(GL_QUADS);
  for (i = 0; i < 6; i++) {
    glNormal3f(n[i][0], n[i][1], n[i][2]);
    for (j = 0; j < 4; j++) {
      glTexCoord2f(uv[i][j][0], uv[i][j][1]);
      glVertex3f(pt[f[i][j]][0], pt[f[i][j]][1], pt[f[i][j]][2]);
    }
  }
  glEnd();
}

void sceneInit(Scene &sc) {
  animInit(sc.anim, "data/anim1.ani");
  sc.tex_terrain = LoadGLTexture("data/terrain/terrain_texture.ppm", false);
  sc.tex_papillon = LoadGLTexture("data/papillon.ppm", true, 230, 230, 230,
                                  LOADGLTEX_GREATER);
  sc.tex_arbre = LoadGLTexture("data/billboard/arbre.ppm", true);
  sc.tex_eau = LoadGLTexture("data/eau.ppm", false);
  sc.tex_cube = LoadGLTexture("data/cubemap/cubemap_BlueSkyRainbow.ppm", false);
  imInitPPM(sc.terrain, "data/terrain/terrain.ppm");
}

void sceneDraw(const Scene &sc) {
  // A MODIFIER : CETTE FONCTION AFFICHE LA SCENE PRINCIPALE

  // Mon 1er cube
  glColor3f(1, 1, 1);
  glPushMatrix(); // sauvegarde la transformation courante
  glTranslatef(-2, 2, 0);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, sc.tex_cube);
  glTranslatef(3, 5, 5);
  // draw_cube();
  draw_cube_map();
  glDisable(GL_TEXTURE_2D);
  glPopMatrix(); // récupère la transformation sauvegardée avant

  glPushMatrix();
  glColor3f(1, 1, 1);
  glEnable(GL_TEXTURE_2D);
  glScalef(9.9, 10, 9);
  glBindTexture(GL_TEXTURE_2D, sc.tex_eau);
  glBegin(GL_QUADS);
  glNormal3f(0, 1, 0);
  glTexCoord2f(0, 0);
  glVertex3f(-1, 0, -1);
  // glVertex3f(-imGetDimX(sc.terrain), 0, -imGetDimY(sc.terrain));
  glTexCoord2f(1, 0);
  glVertex3f(1, 0, -1);
  // glVertex3f(imGetDimX(sc.terrain), 0, -imGetDimY(sc.terrain));
  glTexCoord2f(1, 1);
  glVertex3f(1, 0, 1);
  // glVertex3f(imGetDimX(sc.terrain), 0, imGetDimY(sc.terrain));
  glTexCoord2f(0, 1);
  glVertex3f(-1, 0, 1);
  // glVertex3f(-imGetDimX(sc.terrain), 0, imGetDimY(sc.terrain));
  glEnd();
  glPopMatrix();

  glPushMatrix();
  glColor3f(255, 255, 255);
  glBindTexture(GL_TEXTURE_2D, sc.tex_arbre);
  dessine_();
  glPopMatrix();

  glPushMatrix();
  glBindTexture(GL_TEXTURE_2D, sc.tex_terrain);
  glTranslated(-9, -0.2, -10);
  glScalef(0.1, 0.01, 0.1);
  terraindraw(sc.terrain, sc);
  glPopMatrix();

  // **************** FORME DE BASE ***********************/
}

void _draw_cylinder_() {
  int div = 80;
  float beta;
  float step = 2.0 * M_PI / div;
  glBegin(GL_TRIANGLE_STRIP);
  for (int i = 0; i <= div; i++) {
    beta = i * step;
    glNormal3f(cos(beta), 0, sin(beta));
    glVertex3f(cos(beta), 0, sin(beta));
    glNormal3f(cos(beta), 2, sin(beta));
    glVertex3f(cos(beta), 2, sin(beta));
  }
  glEnd();
  glBegin(GL_TRIANGLE_FAN);
  glNormal3f(0, 0, 0);
  glVertex3f(0, 0, 0);
  for (int i = 0; i <= div; i++) {
    beta = i * step;
    glVertex3f(cos(beta), 0, sin(beta));
  }
  glEnd();
  glBegin(GL_TRIANGLE_FAN);
  glNormal3f(0, 2, 0);
  glVertex3f(0, 2, 0);
  for (int i = 0; i <= div; i++) {
    beta = i * step;
    glVertex3f(cos(beta), 2, sin(beta));
  }
  glEnd();
}

void _draw_cone_() {
  int div = 100;
  float beta;
  float step = 2.0 * M_PI / div;
  glBegin(GL_TRIANGLE_STRIP);
  for (int i = 0; i <= div; i++) {
    beta = i * step;
    glNormal3f(cos(beta), 0, sin(beta));
    glVertex3f(cos(beta), 0, sin(beta));
    glNormal3f(0, 3, 0);
    glVertex3f(0, 3, 0);
  }
  glEnd();
  glBegin(GL_TRIANGLE_FAN);
  glNormal3f(0, 0, 0);
  glVertex3f(0, 0, 0);
  for (int i = 0; i <= div; i++) {
    beta = i * step;
    glVertex3f(cos(beta), 0, sin(beta));
  }
  glEnd();
}
