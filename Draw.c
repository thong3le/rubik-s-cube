#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GLUT/glut.h>

//#include "mathConstants.h"
#include "TrackballUI.h"
#include "Draw.h"
#include "Rubik.h"

#define REDRAW_MSECS 10
#define FRAME_SIZE 10

int WinWidth, WinHeight;

/* define lighing model data */
GLfloat LightPos[] = { 10.0, 10.0, 10.0, 0.0};
GLfloat LightClr[] = { 0.9, 0.9, 0.9, 1.0};
GLfloat LightAmb[] = { 0.25, 0.25, 0.25};

GLint dim;
//GLint SPEED;

GLfloat SliceSpin = 0.0;
GLfloat PrevX, PrevY;
char RotationAxis;
GLint CubeI, CubeJ, CubeK;

GLint face;
GLint Direction = -1;
GLint RotateMode = 0;

void
DrawCone(float Radius, float Height, int FaceCnt)
{
  glBegin(GL_TRIANGLES);
  for (int i = 0; i < FaceCnt; i++) {
    float angle = 2 * M_PI * i / (float) FaceCnt;
    float nangle = 2 * M_PI * (i+1) / (float) FaceCnt;
    glNormal3f(Height*cos(angle),Height*sin(angle),Radius);
    glVertex3f(Radius*cos(angle),Radius*sin(angle),0.0);
    glNormal3f(Height*cos(nangle),Height*sin(nangle),Radius);
    glVertex3f(Radius*cos(nangle),Radius*sin(nangle),0.0);
    glVertex3f(0.0,0.0,Height);
  }
  glEnd();
}

void
DrawFrame(float Size)
{
  glPushMatrix();
  glRotatef(90,0,1,0);
  glColor3f(1,0,0);
  DrawCone(0.5,Size,10);
  glPopMatrix();
  
  glPushMatrix();
  glRotatef(-90,1,0,0);
  glColor3f(0,1,0);
  DrawCone(0.5,Size,10);
  glPopMatrix();
  
  glPushMatrix();
  glColor3f(0,0,1);
  DrawCone(0.5,Size,10);
  glPopMatrix();
}

void
DrawScene()
{ mat4 M;
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glPushMatrix();
  
   
  TB_SetTransform(M);
  glMultMatrixf((GLfloat *)M);
  
  glPushMatrix();
  glTranslatef(-15.0,-15.0,-15.0);
  DrawFrame(FRAME_SIZE);
  glPopMatrix();  
  
  if (RotateMode) {
    axis(face, Direction, &RotationAxis);
    DrawSlices(SliceSpin, RotationAxis, CubeI, CubeJ, CubeK);
  }
  else {
    DrawRubik();
  }
  glutSwapBuffers();
}

void
ResizeView(int AWidth, int AHeight)
{
  GLfloat aspect;
  WinWidth = AWidth;
  WinHeight = AHeight;
  
  aspect = WinWidth / (float) WinHeight;
  glViewport(0,0,WinWidth,WinHeight);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1.0*aspect,1.0*aspect,-1.0,1.0,10.0,200.0);
  glTranslatef(0.0,0.0,-150.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  TB_WinResize(AWidth, AHeight);
}

void
ProcessKey(unsigned char Key, int X, int Y)
{
  switch (Key) {
    case '\027':
      exit(0);
      break;
  }
}

void
ProcessMouseButton(int But, int State, int X, int Y)
{
  GLint view[4];
  GLdouble model[16], proj[16];
  GLfloat Z;
  GLdouble wx,wy,wz;

  if (But == GLUT_LEFT_BUTTON && State == GLUT_DOWN){
    glGetIntegerv(GL_VIEWPORT, view);    
    glGetDoublev(GL_MODELVIEW_MATRIX, model);
    glGetDoublev(GL_PROJECTION_MATRIX, proj);

    PrevX = X;
    PrevY = Y;
    
    glReadPixels(X, view[3] - Y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &Z);
    gluUnProject((GLfloat) X, (GLfloat) view[3]-Y, Z, model, proj, view, &wx,&wy,&wz);
    
    get_position(wx,wy,wz, &face, &CubeI, &CubeJ, &CubeK);
    //printf("wx :%f and wy:%f and wz: %f\n", wx, wy, wz);
    //printf("CubeI :%d and CubeJ:%d and CubeK: %d\n", CubeI, CubeJ, CubeK);
    if (CubeI == -1 && CubeJ == -1 && CubeK == -1) RotateMode = 0;
    else  RotateMode = 1;   
  }
  else if (But == GLUT_LEFT_BUTTON && State == GLUT_UP){
    if (RotateMode)  update(face, RotationAxis, CubeI, CubeJ, CubeK, SliceSpin);
    RotateMode = 0;
    Direction = -1;
    SliceSpin = 0.0;
  }
  TB_MouseButtonEvent(But, State, X, Y);
  glutPostRedisplay();
}

void
ProcessMouseMotion(int X, int Y)
{
    GLint view[4];
    GLdouble model[16], proj[16];
    GLfloat Z;
    GLdouble wx, wy, wz;
    GLdouble ClickWx, ClickWy, ClickWz;

    if (RotateMode){
      glGetIntegerv(GL_VIEWPORT, view);
      glGetDoublev(GL_MODELVIEW_MATRIX, model);
      glGetDoublev(GL_PROJECTION_MATRIX, proj);

      glReadPixels(PrevX, view[3]-PrevY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &Z);
      gluUnProject((GLfloat)PrevX, (GLfloat) view[3]-PrevY, Z, model, proj, view, &ClickWx, &ClickWy, &ClickWz);
      glReadPixels(X, view[3]-Y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &Z);
      gluUnProject((GLfloat)X, (GLfloat) view[3]-Y, Z, model, proj, view, &wx, &wy, &wz);

      direction(ClickWx, ClickWy, ClickWz, wx, wy, wz, face, &Direction); 
      angle(face, Direction, &SliceSpin);
      if (SliceSpin > 360.0)
        SliceSpin -= 360.0;
      if (SliceSpin < 0.0)
        SliceSpin += 360.0;
      PrevX = X;
      PrevY = Y;
    }
    else {
      TB_MouseMotionEvent(X, Y);
      glutPostRedisplay();
    }
}

void
TriggerRedraw(int FrameCnt)
{
  glutPostRedisplay();
  glutTimerFunc(REDRAW_MSECS,TriggerRedraw,FrameCnt+1);
}

void Lighting()
{
  glLightfv(GL_LIGHT0, GL_POSITION, LightPos);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, LightClr);
  glEnable(GL_LIGHT0);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightAmb);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
  glEnable(GL_LIGHTING);
  glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, White);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10.0);
  glEnable(GL_NORMALIZE);
}
static void selectMainMenu(int selection)
{
  switch (selection) {
  case 0:
    exit(0);
  case 1:
    SPEED = 1.5;
    initRubik(dim);
    break;
  case 2:
    randomRubik();
    break;
  case 3:
    Lighting();
    break;
  case 4:
    glDisable(GL_LIGHTING);
    break;
  case 5:
    SPEED = 4.0;
    break;
  case 6:
    SPEED = 2.0;
    break;
  case 7:
    dim = 2;
    initRubik(dim);
    break;
  case 8:  
    dim = 3;
    initRubik(dim);
    break;
  case 9: 
    dim = 4; 
    initRubik(dim);
    break;
  case 10:  
    dim = 5;
    initRubik(dim);
    break;
  case 11:  
    dim = 6;
    initRubik(dim);
    break;
  case 12:  
    dim = 7;
    initRubik(dim);
    break;
  
  }
  glutPostRedisplay();
}

void
InitScene()
{
  TB_Init();

  WinWidth = 1000;
  WinHeight = 600;
  dim = 3;

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - WinWidth) / 2.0, (glutGet(GLUT_SCREEN_HEIGHT) - WinHeight) / 2.0);
  glutInitWindowSize(WinWidth,WinHeight);
  glutCreateWindow("Rubik's Cube Game");

  initRubik(dim);

  glutDisplayFunc(DrawScene);
  glutReshapeFunc(ResizeView);
  glutKeyboardFunc(ProcessKey);
  glutMouseFunc(ProcessMouseButton);
  glutMotionFunc(ProcessMouseMotion);
  glutTimerFunc(REDRAW_MSECS,TriggerRedraw,0);
  
  glClearDepth(1.0);
  glEnable(GL_DEPTH_TEST);
  glClearColor(.4,.4,.4,1.0);
  glShadeModel(GL_SMOOTH);
  

  glutCreateMenu(selectMainMenu);
  glutAddMenuEntry("Reset", 1);
  glutAddMenuEntry("Random", 2);
  glutAddMenuEntry("Lighting On", 3);
  glutAddMenuEntry("Lighting Off", 4);
  glutAddMenuEntry("Fast Mode", 5);
  glutAddMenuEntry("Slow Mode", 6);
  glutAddMenuEntry("2x2x2", 7);
  glutAddMenuEntry("3x3x3", 8);
  glutAddMenuEntry("4x4x4", 9);
  glutAddMenuEntry("5x5x5", 10);
  glutAddMenuEntry("6x6x6", 11);
  glutAddMenuEntry("7x7x7", 12);
  glutAddMenuEntry("Quit", 0);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
  

  ResizeView(WinWidth,WinHeight);
}
