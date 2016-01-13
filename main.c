#include <stdlib.h>
#include <GLUT/glut.h>

#include "Draw.h"

int
main(int argc, char * argv[])
{
  glutInit(&argc, argv);
  
  InitScene();
  
  glutMainLoop();
  
  return 0;
}

