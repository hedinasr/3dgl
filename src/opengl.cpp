#include "opengl.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "Animation.h"
#include "Camera.h"
#include "Image.h"
#include "Scene.h"
#include "Vec3f.h"

// Code ASCII
#define ESCAPE 27
#define PAGE_UP 73
#define PAGE_DOWN 81
#define UP_ARROW 72
#define DOWN_ARROW 80
#define LEFT_ARROW 75
#define RIGHT_ARROW 77

Camera cam;
Scene scene;

int window;          // glut windows
bool bLight = true;  // lighting on/off
bool bBlend = false; // blending on/off
bool bAxe = true;    // draw Axe X,Y,Z
bool bGrid = true;   // draw Grid
bool bAnim = false;  // draw Animation curve
bool bWireframe = false;

const float ambiant = 0.02;
const float diffus = 0.98;
const float spec = 0.5;
GLfloat LightAmbient[] = {ambiant, ambiant, ambiant, 1.f};
GLfloat LightDiffuse[] = {diffus, diffus, diffus, 1.0f};
GLfloat LightSpecular[] = {spec, spec, spec, 1.0f};
GLfloat LightPosition[] = {5.0f, 15.0f, 5.0f, 1.0f};

void help() {
  printf("\n\nCamera :\n");
  printf("   Fleches+pageup+pagedown : bouge la camera\n");
  printf("   Ctrl+Fleches ou souris+bouton gauche: tourne la camera\n");
  printf("   Souris+bouton droit : avance/recule la camera\n");
  printf("   i : reinitialise la camera\n");
  printf("\n");
  printf("Lumiere : \n");
  printf("    alt+fleches ou pageup,pagedown : bouge la mumière\n");
  printf("\n");
  printf("Touches : \n");
  printf("   a : dessine les axes\n");
  printf("   b : active blend(transparence)\n");
  printf("   f : camera Fly or Trackball\n");
  printf("   l : active la lumiere\n");
  printf("   z : dessine le point qui devrait s'animer\n");
  printf("   w : wireframe\n");
}

// Load Bitmaps And Convert To Textures
unsigned int LoadGLTexture(const char *nomfichier, bool isTransparency,
                           const unsigned char R, const unsigned char G,
                           const unsigned char B, int option) {
  Image image1;
  imInitPPM(image1, nomfichier);

  if (isTransparency)
    imCreateTransparency(image1, R, G, B, option);

  unsigned int texture;

  // Create Textures
  glGenTextures(1, &texture);

  // mipmapped texture
  glBindTexture(GL_TEXTURE_2D, texture); // 2d texture (x and y size)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                  GL_LINEAR); // scale linearly when image bigger than texture
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR); // GL_LINEAR_MIPMAP_NEAREST); //
                                            // scale mipmap when image smalled
                                            // than texture
  switch (imGetDimC(image1)) {
  case 1:
    gluBuild2DMipmaps(GL_TEXTURE_2D, 1, imGetDimX(image1), imGetDimY(image1),
                      GL_LUMINANCE, GL_UNSIGNED_BYTE, imGetData(image1));
    break;
  case 3:
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, imGetDimX(image1), imGetDimY(image1),
                      GL_RGB, GL_UNSIGNED_BYTE, imGetData(image1));
    break;
  case 4:
    gluBuild2DMipmaps(GL_TEXTURE_2D, 4, imGetDimX(image1), imGetDimY(image1),
                      GL_RGBA, GL_UNSIGNED_BYTE, imGetData(image1));
    break;
  default:
    printf(
        "LoadGLTexture: can not load GL texture! dimColor is not managed!\n");
    break;
  }

  imFree(image1);

  return texture;
}

void GLColor(const float r, const float g, const float b) {
  // when the light is enable
  Vec3f v, V;
  vecInit(V, r, g, b);
  vecMul(v, 0.05f, V);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ((float *)(&v)));
  vecMul(v, 1.f, V);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ((float *)(&v)));
  vecMul(v, 0.1f, V);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ((float *)(&v)));

  // when the light is disable
  glColor3f(r, g, b);
}

//! A appeller juste avant la boucle principale (Main Loop) et après la creation
//! de a fenetre
GLvoid GLInit(GLsizei Width, GLsizei Height) {
  printf("repertoire courant=");
  system("pwd");

  glEnable(GL_TEXTURE_2D);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // The Background Color = Black
  glClearDepth(1.0);
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);

  // Indique à GL de calculer la couleur suivant la formule
  // couleur_du_buffer = alpha * couleur_polygone +
  // (1-alpha)*couleur_du_buffer_de_fond
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_BLEND);

  // Indique à GL d'eliminer les fragments du polygones qui sont transparents
  // (On ne garde que les pixel dont alpha>0.01, )
  glAlphaFunc(GL_GREATER, 0.01f);
  glDisable(GL_ALPHA_TEST);

  glEnable(GL_NORMALIZE);

  // set up lights.
  glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpecular);
  glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
  glEnable(GL_LIGHT1);

  if (bLight)
    glEnable(GL_LIGHTING);
  else
    glDisable(GL_LIGHTING);

  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
  // glDisable( GL_COLOR_MATERIAL );

  camInit(cam);
  sceneInit(scene);
}

//! LA fonction d'affichage
void GLDraw(void) {
  // Efface la couleur et le ZBuffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);

  // Place la scène comme on veut
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  camSetGL(cam);

  glPushAttrib(GL_ENABLE_BIT);
  glPointSize(18);
  glColor3f(1, 1, 1);
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);
  glBegin(GL_POINTS);
  glVertex3f(LightPosition[0], LightPosition[1], LightPosition[2]);
  glEnd();

  if (bAxe)
    draw_axes();
  if (bGrid)
    draw_grid();
  if (bAnim)
    animDraw(scene.anim);

  glPopAttrib();
  sceneDraw(scene);

  // since this is double buffered, swap the buffers to display what just got
  // drawn.
  glutSwapBuffers();
}

//! Quand la fenetre est 'resizée'
GLvoid GLResize(GLsizei Width, GLsizei Height) {
  if (Height == 0)
    Height = 1;

  glViewport(
      0, 0, Width,
      Height); // Reset The Current Viewport And Perspective Transformation

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(40.0f, (GLfloat)Width / (GLfloat)Height, 0.1f, 10000.0f);

  glMatrixMode(GL_MODELVIEW);
}

//! Quand une touche est pressée
void keyPressed(unsigned char key, int x, int y) {
  // usleep(100);

  switch (key) {
  case ESCAPE: // termine
    exit(1);
    break;

  case 'h':
  case 'H':
    help();
    break;

  case 'z':
  case 'Z':
    bAnim = !bAnim;
    break;

  case 'g':
  case 'G':
    bGrid = !bGrid;
    break;

  case 'a':
  case 'A':
    bAxe = !bAxe;
    break;

  case 'f':
  case 'F':
    if (cam.flyOrTrackball == CAMERA_TRACKBALL)
      camSetFlyTrackballMode(cam, CAMERA_FLY);
    else
      camSetFlyTrackballMode(cam, CAMERA_TRACKBALL);
    printf("Camera fly=0 or trackball=1 : %d\n", cam.flyOrTrackball);
    break;

  case 'I':
    LightPosition[0] = 5.0f;
    LightPosition[1] = 15.0f;
    LightPosition[2] = 5.0f;
    break;
  case 'i':
    camInit(cam);
    break;

  case 'w':
  case 'W':
    bWireframe = !bWireframe;
    if (bWireframe)
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    printf("wireframe=%d\n", bWireframe);
    break;

  case 'b':
  case 'B': // switch the blending
    //~ printf("B/b pressed; blending is: %d\n", bBlend);
    bBlend =
        bBlend ? 0 : 1; // switch the current value of blend, between 0 and 1.
    if (bBlend) {
      glEnable(GL_BLEND);
      glEnable(GL_ALPHA_TEST);
    } else {
      glDisable(GL_BLEND);
      glDisable(GL_ALPHA_TEST);
    }
    printf("Blending is now: %d\n", bBlend);
    break;

  case 'l':
  case 'L': // switch the lighting
    //~ printf("L/l pressed; lighting is: %d\n", bLight);
    bLight =
        bLight ? 0 : 1; // switch the current value of light, between 0 and 1.
    if (bLight) {
      glEnable(GL_LIGHTING);
    } else {
      glDisable(GL_LIGHTING);
    }
    printf("Lighting is now: %d\n", bLight);
    break;

  default:
    printf("Key %d pressed. No action there yet.\n", key);
    break;
  }
}

/* The function called whenever a normal key is pressed. */
void specialKeyPressed(int key, int x, int y) {
  /* avoid thrashing this procedure */
  usleep(100);

  const float stepR = 2;
  const int stepL = 1;

  if (glutGetModifiers() == GLUT_ACTIVE_CTRL) {
    switch (key) {
    case GLUT_KEY_UP:
      camTurnUpDown(cam, -stepR);
      break;

    case GLUT_KEY_DOWN:
      camTurnUpDown(cam, stepR);
      break;

    case GLUT_KEY_LEFT:
      camTurnLeftRight(cam, -stepR);
      break;

    case GLUT_KEY_RIGHT:
      camTurnLeftRight(cam, stepR);
      break;
    }
  } else if (glutGetModifiers() == GLUT_ACTIVE_ALT) {
    switch (key) {
    case GLUT_KEY_UP:
      LightPosition[0] += stepL;
      break;

    case GLUT_KEY_DOWN:
      LightPosition[0] -= stepL;
      break;

    case GLUT_KEY_LEFT:
      LightPosition[2] -= stepL;
      break;

    case GLUT_KEY_RIGHT:
      LightPosition[2] += stepL;
      break;

    case GLUT_KEY_PAGE_UP:
      LightPosition[1] += stepL;
      break;

    case GLUT_KEY_PAGE_DOWN:
      LightPosition[1] -= stepL;
      break;
    }
    printf("Light position= %f %f %f\n", LightPosition[0], LightPosition[1],
           LightPosition[2]);
  } else {
    switch (key) {
    case GLUT_KEY_PAGE_UP:
      camTranslate(cam, UP);
      break;

    case GLUT_KEY_PAGE_DOWN:
      camTranslate(cam, DOWN);
      break;
    case GLUT_KEY_UP:
      camTranslate(cam, FRONT);
      break;

    case GLUT_KEY_DOWN:
      camTranslate(cam, BACK);
      break;

    case GLUT_KEY_LEFT:
      camTranslate(cam, LEFT);
      break;

    case GLUT_KEY_RIGHT:
      camTranslate(cam, RIGHT);
      break;
    default:
      //~ printf ("Special key %d pressed. No action there yet.\n", key);
      break;
    }
  }
}

void processMouse(int button, int state, int x, int y) {
  //    printf("processMouse\n");
  //    int specialKey=0i
  //    specialKey = glutGetModifiers();

  camStartMove(cam, CAMERA_STOP, x, y);

  if ((state == GLUT_DOWN)) // && (specialKey == GLUT_ACTIVE_ALT))
  {
    if (button == GLUT_LEFT_BUTTON) {
      camStartMove(cam, CAMERA_ROTATION, x, y);
    } else if (button == GLUT_RIGHT_BUTTON) {
      camStartMove(cam, CAMERA_TRANSLATION, x, y);
    }
  }
}

void processMouseActiveMotion(int x, int y) {
  //    printf("processMouseActiveMotion\n");
  camMove(cam, x, y);
}

void processMousePassiveMotion(int x, int y) {
  // printf("processMousePassiveMotion\n");
}

int main(int argc, char **argv) {
  // Initialize GLUT
  glutInit(&argc, argv);

  /* Select type of Display mode:	 Double buffer	 RGBA color	 Depth
   * buffer	 Alpha blending */
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA);

  // demande une fenetre de 640 x 480 à la position 0,0
  const float W = 1024, H = 768;
  glutInitWindowSize(W, H);
  glutInitWindowPosition(0, 0);

  // Creer une fenetre
  window = glutCreateWindow("3dgl");

  // La fonction d'affichage est GLDraw
  glutDisplayFunc(&GLDraw);

  // Plein écran
  // glutFullScreen();

  // Quand il n'y a aucun évenement, on affiche quand même la scène
  glutIdleFunc(&GLDraw);

  // Quand la fenetre change de taille
  glutReshapeFunc(&GLResize);

  // quand une touche est pressee
  glutKeyboardFunc(&keyPressed);

  // quand une touche speciale est pressee
  glutSpecialFunc(&specialKeyPressed);

  // mouse
  glutMouseFunc(processMouse);
  glutMotionFunc(processMouseActiveMotion);
  glutPassiveMotionFunc(processMousePassiveMotion);
  //    glutEntryFunc(processMouseEntry);

  // Notre init:
  GLInit(W, H);

  // La boucle principale
  glutMainLoop();

  return 0;
}
