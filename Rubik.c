#include <stdio.h>
#include <stdlib.h>            
#include <math.h>
#include <GLUT/glut.h>

#include "Rubik.h"

#define EPSILON 0.1

#define BACK 0
#define FRONT 1
#define RIGHT 2
#define LEFT 3
#define BOTTOM 4
#define TOP 5

#define SPIN_UP 0
#define SPIN_DOWN 1
#define SPIN_LEFT 2
#define SPIN_RIGHT 3

GLfloat Blue[] = {0.0, 0.0, 0.9, 1.0};
GLfloat Yellow[] = {1.0, 1.0, 0.0, 1.0};
GLfloat Green[] = {0.0, 0.9, 0.0, 1.0};
GLfloat Red[]  = {0.9, 0.0, 0.0, 1.0};
GLfloat Orange[] = {1.0, 0.5, 0.0, 1.0};
GLfloat Gray[] = {0.0, 0.0, 0.0, 1.0};
GLfloat Black[] = {0.0, 0.0, 0.0, 1.0};
GLfloat White[] = {1.0, 1.0, 1.0, 1.0};

typedef GLfloat VERTICES[8][3];
typedef GLfloat FACES[6][4];

struct rubiktype {
  VERTICES vertex;
  FACES facecolor;
};
typedef struct rubiktype RUBIK;

GLint DIM;
GLfloat CUBESIZE;
GLfloat SPEED = 2.0;

RUBIK rubik[7][7][7];

void setColor(GLfloat c1[4], GLfloat c2[]){
	c1[0] = c2[0];	c1[1] = c2[1];	c1[2] = c2[2];	c1[3] = c2[3];
}

void setCord(GLfloat vertex[3], GLfloat x, GLfloat y, GLfloat z)
{
	vertex[0] = x; 	vertex[1] = y;	vertex[2] = z;
}

void direction(GLfloat clickx, GLfloat clicky, GLfloat clickz, GLfloat x, GLfloat y, GLfloat z, GLint face, GLint *dir) {
  GLfloat deltaX = x - clickx;
  GLfloat deltaY = y - clicky;
  GLfloat deltaZ = z - clickz;

  switch (face) {
  case BACK: ;
  case FRONT:
    if (fabsf(deltaX) > fabsf(deltaY)) {
      if (deltaX < 0.0 && face == BACK && (*dir == -1 || *dir == SPIN_LEFT))     *dir = SPIN_RIGHT;
      else if (deltaX >= 0.0 && face == BACK && (*dir==-1 || *dir==SPIN_RIGHT))   *dir = SPIN_LEFT;
      else if (deltaX < 0.0 && face == FRONT && (*dir==-1 || *dir==SPIN_RIGHT))    *dir = SPIN_LEFT;
      else if (deltaX >= 0.0 && face == FRONT && (*dir==-1 || *dir==SPIN_LEFT))     *dir = SPIN_RIGHT;
    }
    else {
      if (deltaY < 0.0 && (*dir==-1 || *dir==SPIN_UP))  *dir = SPIN_DOWN;
      else if (deltaY >= 0.0 && (*dir==-1 || *dir==SPIN_DOWN))  *dir = SPIN_UP;
    }
    break;
  case RIGHT: ;
  case LEFT:
    if (fabsf(deltaY) > fabsf(deltaZ)) {
      if (deltaY < 0.0 && (*dir==-1 || *dir==SPIN_UP))   *dir = SPIN_DOWN;
      else if (deltaY >= 0.0 && (*dir==-1 || *dir==SPIN_DOWN))  *dir = SPIN_UP;
    }
    else {
      if (deltaZ < 0.0 && face == RIGHT && (*dir==-1 || *dir==SPIN_LEFT))  *dir = SPIN_RIGHT;
      else if (deltaZ >= 0.0 && face == RIGHT && (*dir==-1 || *dir==SPIN_RIGHT)) *dir = SPIN_LEFT;
      else if (deltaZ < 0.0 && face == LEFT && (*dir==-1 || *dir==SPIN_RIGHT)) *dir = SPIN_LEFT;
      else if (deltaZ >= 0.0 && face == LEFT && (*dir==-1 || *dir==SPIN_LEFT)) *dir = SPIN_RIGHT;
    }
    break;
  case BOTTOM: ;
  case TOP:
    if (fabsf(deltaX) > fabsf(deltaZ)) {
      if (deltaX < 0.0 && (*dir==-1 || *dir==SPIN_RIGHT))  *dir = SPIN_LEFT;
      else if (deltaX >= 0.0 && (*dir==-1 || *dir==SPIN_LEFT))  *dir = SPIN_RIGHT;
    }
    else {
      if (deltaZ < 0.0 && face == BOTTOM && (*dir==-1 || *dir==SPIN_UP))  *dir = SPIN_DOWN;
      else if (deltaZ >= 0.0 && face == BOTTOM && (*dir==-1 || *dir==SPIN_DOWN))  *dir = SPIN_UP;
      else if (deltaZ < 0.0 && face == TOP && (*dir==-1 || *dir==SPIN_DOWN))   *dir = SPIN_UP;
      else if (deltaZ >= 0.0 && face == TOP && (*dir==-1 || *dir==SPIN_UP))    *dir = SPIN_DOWN;
    }
    break;
  default:    break;
  }
}


void angle(GLint face, GLint spin_dir, GLfloat *spin) {
  switch (face) {
  case BACK:
    if (spin_dir == SPIN_RIGHT || spin_dir == SPIN_UP)        *spin += SPEED;
    else if (spin_dir == SPIN_LEFT || spin_dir == SPIN_DOWN)  *spin -= SPEED;
    break;
  case FRONT:
    if (spin_dir == SPIN_RIGHT || spin_dir == SPIN_DOWN)      *spin += SPEED; 
    else if (spin_dir == SPIN_LEFT || spin_dir == SPIN_UP)    *spin -= SPEED;
    break;
  case RIGHT:
    if (spin_dir == SPIN_RIGHT || spin_dir == SPIN_UP)        *spin += SPEED;
    else if (spin_dir == SPIN_LEFT || spin_dir == SPIN_DOWN)  *spin -= SPEED;
    break;
  case LEFT:
    if (spin_dir == SPIN_RIGHT || spin_dir == SPIN_DOWN)      *spin += SPEED;
    else if (spin_dir == SPIN_LEFT || spin_dir == SPIN_UP)    *spin -= SPEED;
    break;
  case BOTTOM:
    if (spin_dir == SPIN_RIGHT || spin_dir == SPIN_DOWN)      *spin += SPEED;
    else if (spin_dir == SPIN_LEFT || spin_dir == SPIN_UP)    *spin -= SPEED;
    break;
  case TOP:
    if (spin_dir == SPIN_LEFT || spin_dir == SPIN_DOWN)       *spin += SPEED;
    else if (spin_dir == SPIN_RIGHT || spin_dir == SPIN_UP)   *spin -= SPEED;
    break;
  default:      break;
  }
}

void axis(GLint face, GLint spin_dir, char *axis) {
  switch (face) {
  case BACK: ;
  case FRONT:
    if (spin_dir == SPIN_LEFT || spin_dir == SPIN_RIGHT)       *axis = 'Y';
    else if (spin_dir == SPIN_UP || spin_dir == SPIN_DOWN)     *axis = 'X';
    break;
  case RIGHT: ;
  case LEFT:
    if (spin_dir == SPIN_LEFT || spin_dir == SPIN_RIGHT)      *axis = 'Y';
    else if (spin_dir == SPIN_UP || spin_dir == SPIN_DOWN)    *axis = 'Z';
    break;
  case BOTTOM: ;
  case TOP:
    if (spin_dir == SPIN_LEFT || spin_dir == SPIN_RIGHT)      *axis = 'Z';
    else if (spin_dir == SPIN_UP || spin_dir == SPIN_DOWN)    *axis = 'X';
    break;
  default:    break;
  }
}

int is_inside(GLfloat x, GLfloat y, GLfloat z, VERTICES vertices){
  GLfloat left, right, bottom, top, front, back;

  left = vertices[0][0];  right = vertices[1][0];
  bottom = vertices[0][1];  top = vertices[3][1];
  front = vertices[4][2];  back = vertices[0][2];

  if (((x>left && x<right) || (fabs(x-left)<=0.05 || fabs(x-right)<=0.05)) && 
      ((y>bottom && y<top) || (fabs(y-bottom)<=0.05 || fabs(y-top)<=0.05)) &&
      ((z>back && z<front) || (fabs(z-front)<=0.05 || fabs(z-back)<=0.05)))
    return 1;
  else
    return 0;
}

void get_position(GLfloat x, GLfloat y, GLfloat z, GLint *face, GLint *i, GLint *j, GLint *k){
	GLfloat size = CUBESIZE + EPSILON;
	GLfloat radius = 0.5*(DIM*size - EPSILON);
	GLfloat upper = radius+EPSILON;
	GLfloat lower =	radius-EPSILON;
	GLint i1, j1, k1;
	
	if (x >= lower && x <= upper)   *face = RIGHT;
  else if (x >= -upper && x <= -lower)   *face = LEFT;
  else if (y >= lower && y <= upper)   *face = TOP;
  else if (y >= -upper && y <= -lower)  *face = BOTTOM;
  else if (z >= lower && z <= upper)   *face = FRONT;
  else if (z >= -upper && z <= -lower)  *face = BACK;

  for (i1=0; i1<DIM; ++i1) {
   	for (j1=0; j1<DIM; ++j1) {
     		for (k1=0; k1<DIM; ++k1) {
	     		if (is_inside(x, y, z, rubik[i1][j1][k1].vertex)) {
	 		  		*i = i1;
	  		 		*j = j1;
	  				*k = k1;
	  				return;
	  			}
	    	}
     }
  }  	
  *i = *j = *k = -1;
	return;
}


void change_color(char axis, GLint slice_no) {
  GLint c, f, j;
  GLfloat color[DIM][6][3];

  switch (axis) {
  case 'X': 
  for (j = 0; j < DIM/2; j++){
    for (c=j; c<DIM-j; c++)
      for (f=0; f<6; f++)
        setColor(color[c][f], rubik[slice_no][c][DIM-j-1].facecolor[f]);
    for (c=j; c<DIM-j; c++) {
      setColor(rubik[slice_no][c][DIM-j-1].facecolor[FRONT], rubik[slice_no][DIM-j-1][DIM-1-c].facecolor[FRONT]);
      setColor(rubik[slice_no][c][DIM-j-1].facecolor[TOP], rubik[slice_no][DIM-j-1][DIM-1-c].facecolor[RIGHT]);
      setColor(rubik[slice_no][c][DIM-j-1].facecolor[BACK], rubik[slice_no][DIM-j-1][DIM-1-c].facecolor[BACK]);
      setColor(rubik[slice_no][c][DIM-j-1].facecolor[BOTTOM], rubik[slice_no][DIM-j-1][DIM-1-c].facecolor[LEFT]);
      setColor(rubik[slice_no][c][DIM-j-1].facecolor[LEFT], rubik[slice_no][DIM-j-1][DIM-1-c].facecolor[TOP]);
      setColor(rubik[slice_no][c][DIM-j-1].facecolor[RIGHT],  rubik[slice_no][DIM-j-1][DIM-1-c].facecolor[BOTTOM]);
    }
    for (c=j; c<DIM-j; c++)  {
      setColor(rubik[slice_no][DIM-j-1][DIM-1-c].facecolor[TOP], rubik[slice_no][DIM-1-c][j].facecolor[RIGHT]);
      setColor(rubik[slice_no][DIM-j-1][DIM-1-c].facecolor[BACK], rubik[slice_no][DIM-1-c][j].facecolor[BACK]);
      setColor(rubik[slice_no][DIM-j-1][DIM-1-c].facecolor[BOTTOM], rubik[slice_no][DIM-1-c][j].facecolor[LEFT]);
      setColor(rubik[slice_no][DIM-j-1][DIM-1-c].facecolor[FRONT], rubik[slice_no][DIM-1-c][j].facecolor[FRONT]);
      setColor(rubik[slice_no][DIM-j-1][DIM-1-c].facecolor[LEFT], rubik[slice_no][DIM-1-c][j].facecolor[TOP]);
      setColor(rubik[slice_no][DIM-j-1][DIM-1-c].facecolor[RIGHT], rubik[slice_no][DIM-1-c][j].facecolor[BOTTOM]);
    }
    for (c=j; c<DIM-j; c++) {
      setColor(rubik[slice_no][DIM-1-c][j].facecolor[FRONT], rubik[slice_no][j][c].facecolor[FRONT]);
      setColor(rubik[slice_no][DIM-1-c][j].facecolor[TOP],  rubik[slice_no][j][c].facecolor[RIGHT]);
      setColor(rubik[slice_no][DIM-1-c][j].facecolor[BACK],  rubik[slice_no][j][c].facecolor[BACK]);
      setColor(rubik[slice_no][DIM-1-c][j].facecolor[BOTTOM], rubik[slice_no][j][c].facecolor[LEFT]);
      setColor(rubik[slice_no][DIM-1-c][j].facecolor[LEFT], rubik[slice_no][j][c].facecolor[TOP]);
      setColor(rubik[slice_no][DIM-1-c][j].facecolor[RIGHT],  rubik[slice_no][j][c].facecolor[BOTTOM]);
    }
    for (c=j; c<DIM-j; c++) {
      setColor(rubik[slice_no][j][c].facecolor[FRONT], color[c][FRONT]);
      setColor(rubik[slice_no][j][c].facecolor[TOP], color[c][RIGHT]);
      setColor(rubik[slice_no][j][c].facecolor[BACK],color[c][BACK]);
      setColor(rubik[slice_no][j][c].facecolor[BOTTOM],color[c][LEFT]);
      setColor(rubik[slice_no][j][c].facecolor[LEFT],color[c][TOP]);
      setColor(rubik[slice_no][j][c].facecolor[RIGHT],color[c][BOTTOM]);
    }
  }
    break;
  case 'Y': 
  for (j=0; j<DIM/2; j++){
    for (c=j; c<DIM-j; c++)
      for (f=0; f<6; f++)
        setColor(color[c][f],
                        rubik[DIM-1-c][slice_no][DIM-j-1].facecolor[f]);
    for (c=j; c<DIM-j; c++) {
      setColor(rubik[DIM-1-c][slice_no][DIM-j-1].facecolor[FRONT], rubik[j][slice_no][DIM-1-c].facecolor[TOP]);
      setColor(rubik[DIM-1-c][slice_no][DIM-j-1].facecolor[LEFT], rubik[j][slice_no][DIM-1-c].facecolor[LEFT]);
      setColor(rubik[DIM-1-c][slice_no][DIM-j-1].facecolor[BACK], rubik[j][slice_no][DIM-1-c].facecolor[BOTTOM]);
      setColor(rubik[DIM-1-c][slice_no][DIM-j-1].facecolor[RIGHT], rubik[j][slice_no][DIM-1-c].facecolor[RIGHT]);
      setColor(rubik[DIM-1-c][slice_no][DIM-j-1].facecolor[TOP], rubik[j][slice_no][DIM-1-c].facecolor[BACK]);
      setColor(rubik[DIM-1-c][slice_no][DIM-j-1].facecolor[BOTTOM], rubik[j][slice_no][DIM-1-c].facecolor[FRONT]);
    }
    for (c=j; c<DIM-j; c++) {
      setColor(rubik[j][slice_no][DIM-1-c].facecolor[FRONT], rubik[c][slice_no][j].facecolor[TOP]);
      setColor(rubik[j][slice_no][DIM-1-c].facecolor[LEFT], rubik[c][slice_no][j].facecolor[LEFT]);
      setColor(rubik[j][slice_no][DIM-1-c].facecolor[BACK], rubik[c][slice_no][j].facecolor[BOTTOM]);
      setColor(rubik[j][slice_no][DIM-1-c].facecolor[RIGHT], rubik[c][slice_no][j].facecolor[RIGHT]);
      setColor(rubik[j][slice_no][DIM-1-c].facecolor[TOP], rubik[c][slice_no][j].facecolor[BACK]);
      setColor(rubik[j][slice_no][DIM-1-c].facecolor[BOTTOM], rubik[c][slice_no][j].facecolor[FRONT]);
    }
    for (c=j; c<DIM-j; c++) {
      setColor(rubik[c][slice_no][j].facecolor[FRONT], rubik[DIM-j-1][slice_no][c].facecolor[TOP]);
      setColor(rubik[c][slice_no][j].facecolor[LEFT], rubik[DIM-j-1][slice_no][c].facecolor[LEFT]);
      setColor(rubik[c][slice_no][j].facecolor[BACK], rubik[DIM-j-1][slice_no][c].facecolor[BOTTOM]);
      setColor(rubik[c][slice_no][j].facecolor[RIGHT], rubik[DIM-j-1][slice_no][c].facecolor[RIGHT]);
      setColor(rubik[c][slice_no][j].facecolor[TOP], rubik[DIM-j-1][slice_no][c].facecolor[BACK]);
      setColor(rubik[c][slice_no][j].facecolor[BOTTOM], rubik[DIM-j-1][slice_no][c].facecolor[FRONT]);
    }
    for (c=j; c<DIM-j; c++) {
      setColor(rubik[DIM-j-1][slice_no][c].facecolor[FRONT], color[c][TOP]);
      setColor(rubik[DIM-j-1][slice_no][c].facecolor[LEFT], color[c][LEFT]);
      setColor(rubik[DIM-j-1][slice_no][c].facecolor[BACK], color[c][BOTTOM]);
      setColor(rubik[DIM-j-1][slice_no][c].facecolor[RIGHT], color[c][RIGHT]);
      setColor(rubik[DIM-j-1][slice_no][c].facecolor[TOP], color[c][BACK]);
      setColor(rubik[DIM-j-1][slice_no][c].facecolor[BOTTOM], color[c][FRONT]);
    }
  }
    break;
  case 'Z': 
    for (j=0; j<DIM/2; j++){
      for (c=j; c<DIM-j; c++)
        for (f=0; f<6; f++)     setColor(color[c][f], rubik[c][DIM-j-1][slice_no].facecolor[f]);
      for (c=j; c<DIM-j; c++) {
        setColor(rubik[c][DIM-j-1][slice_no].facecolor[TOP], rubik[DIM-j-1][DIM-1-c][slice_no].facecolor[TOP]);
        setColor(rubik[c][DIM-j-1][slice_no].facecolor[RIGHT], rubik[DIM-j-1][DIM-1-c][slice_no].facecolor[FRONT]);
        setColor(rubik[c][DIM-j-1][slice_no].facecolor[BOTTOM], rubik[DIM-j-1][DIM-1-c][slice_no].facecolor[BOTTOM]);
        setColor(rubik[c][DIM-j-1][slice_no].facecolor[LEFT], rubik[DIM-j-1][DIM-1-c][slice_no].facecolor[BACK]);
        setColor(rubik[c][DIM-j-1][slice_no].facecolor[FRONT], rubik[DIM-j-1][DIM-1-c][slice_no].facecolor[LEFT]);
        setColor(rubik[c][DIM-j-1][slice_no].facecolor[BACK], rubik[DIM-j-1][DIM-1-c][slice_no].facecolor[RIGHT]);
      }
      for (c=j; c<DIM-j; c++) {
        setColor(rubik[DIM-j-1][DIM-1-c][slice_no].facecolor[TOP], rubik[DIM-1-c][j][slice_no].facecolor[TOP]);
        setColor(rubik[DIM-j-1][DIM-1-c][slice_no].facecolor[RIGHT], rubik[DIM-1-c][j][slice_no].facecolor[FRONT]);
        setColor(rubik[DIM-j-1][DIM-1-c][slice_no].facecolor[BOTTOM], rubik[DIM-1-c][j][slice_no].facecolor[BOTTOM]);
        setColor(rubik[DIM-j-1][DIM-1-c][slice_no].facecolor[LEFT], rubik[DIM-1-c][j][slice_no].facecolor[BACK]);
        setColor(rubik[DIM-j-1][DIM-1-c][slice_no].facecolor[FRONT], rubik[DIM-1-c][j][slice_no].facecolor[LEFT]);
        setColor(rubik[DIM-j-1][DIM-1-c][slice_no].facecolor[BACK], rubik[DIM-1-c][j][slice_no].facecolor[RIGHT]);
      }
      for (c=j; c<DIM-j; c++) {
        setColor(rubik[DIM-1-c][j][slice_no].facecolor[TOP], rubik[j][c][slice_no].facecolor[TOP]);
        setColor(rubik[DIM-1-c][j][slice_no].facecolor[RIGHT], rubik[j][c][slice_no].facecolor[FRONT]);
        setColor(rubik[DIM-1-c][j][slice_no].facecolor[BOTTOM], rubik[j][c][slice_no].facecolor[BOTTOM]);
        setColor(rubik[DIM-1-c][j][slice_no].facecolor[LEFT], rubik[j][c][slice_no].facecolor[BACK]);
        setColor(rubik[DIM-1-c][j][slice_no].facecolor[FRONT], rubik[j][c][slice_no].facecolor[LEFT]);
        setColor(rubik[DIM-1-c][j][slice_no].facecolor[BACK], rubik[j][c][slice_no].facecolor[RIGHT]);
      }
      for (c=j; c<DIM-j; c++) {
        setColor(rubik[j][c][slice_no].facecolor[TOP], color[c][TOP]);
        setColor(rubik[j][c][slice_no].facecolor[RIGHT], color[c][FRONT]);
        setColor(rubik[j][c][slice_no].facecolor[BOTTOM], color[c][BOTTOM]);
        setColor(rubik[j][c][slice_no].facecolor[LEFT], color[c][BACK]);
        setColor(rubik[j][c][slice_no].facecolor[FRONT], color[c][LEFT]);
        setColor(rubik[j][c][slice_no].facecolor[BACK], color[c][RIGHT]);
      }
    }
  }
}

void update(GLint face, char axis, GLint i, GLint j, GLint k, GLfloat spin) {
  GLint slice;
  switch (axis) {
  case 'X':    slice = i;    break;
  case 'Y':    slice = j;    break;
  case 'Z':    slice = k;    break;
  default :    break;
  }
  while (spin > 45.0 && spin <= 315.0) {
    change_color(axis, slice);
    spin -= 90.0;
  }
}

void randomRubik() {
  GLint i, count;
  char axis;
  GLint axis_no, slice_no;

  for (i = 0; i < 50; i++) {
    axis_no = rand() % 3;
    switch (axis_no) {
    case 0:  
      axis = 'X';
      break;
    case 1:  
      axis = 'Y';
      break;
    case 2:  
      axis = 'Z';
      break;
    }
    slice_no = rand() % DIM;
    count = rand() % 4;
    for (; count > 0; count--)
      change_color(axis, slice_no); 
  }
}



void initRubik(GLint dim)
{
  DIM = dim;

  if (dim < 4) CUBESIZE = 5.0;
  else if (dim < 6) CUBESIZE = 4.0;
  else CUBESIZE = 3.0;

	GLfloat size = CUBESIZE + EPSILON;
	GLfloat radius = 0.5*(DIM*size - EPSILON);
	GLint i, j, k;
	GLfloat x, y, z;
	GLfloat back_col[4], front_col[4], right_col[4], left_col[4], bottom_col[4], top_col[4];
	
	for (i = 0; i < DIM; i++){
		x = i*size - radius;

		if (i == 0) {
			setColor(back_col, Yellow);
			setColor(front_col, Gray);
		}
		else if (i == DIM - 1){
			setColor(back_col, Gray);
			setColor(front_col, White);
		}
		else{
			setColor(back_col, Gray);
			setColor(front_col, Gray);
		}

		for (j = 0; j < DIM; j++){
			y = j*size - radius;

			if (j == 0) {
				setColor(left_col, Green);
				setColor(right_col, Gray);
			}
			else if (j == DIM - 1){
				setColor(left_col, Gray);
				setColor(right_col, Blue);
			}
			else{
				setColor(left_col, Gray);
				setColor(right_col, Gray);
			}

			for (k = 0; k < DIM; k++){
				z = k*size - radius;

				if (k == 0) {
          			 setColor(bottom_col, Red);
					       setColor(top_col, Gray);
        		}
        		else if (k == DIM-1) {
          			 setColor(bottom_col, Gray);
					       setColor(top_col, Orange);
          		}
        		else {
        			   setColor(bottom_col, Gray);
					       setColor(top_col, Gray);
		        }

		        setCord( (rubik[i][j][k]).vertex[0], x, y, z);
		        setCord( (rubik[i][j][k]).vertex[1], x+CUBESIZE, y, z);
		        setCord( (rubik[i][j][k]).vertex[2], x+CUBESIZE, y+CUBESIZE, z);
		        setCord( (rubik[i][j][k]).vertex[3], x, y+CUBESIZE, z);
		        setCord( (rubik[i][j][k]).vertex[4], x, y, z+CUBESIZE);
		        setCord( (rubik[i][j][k]).vertex[5], x+CUBESIZE, y, z+CUBESIZE);
		        setCord( (rubik[i][j][k]).vertex[6], x+CUBESIZE, y+CUBESIZE, z+CUBESIZE);
		        setCord( (rubik[i][j][k]).vertex[7], x, y+CUBESIZE, z+CUBESIZE);

		        setColor( (rubik[i][j][k]).facecolor[BACK], back_col);
		        setColor( (rubik[i][j][k]).facecolor[FRONT], front_col);
		        setColor( (rubik[i][j][k]).facecolor[RIGHT], right_col);
		        setColor( (rubik[i][j][k]).facecolor[LEFT], left_col);
		        setColor( (rubik[i][j][k]).facecolor[BOTTOM], bottom_col);
		        setColor( (rubik[i][j][k]).facecolor[TOP], top_col);
			}
		}
	}

}

void DrawCube(GLint i, GLint j, GLint k) {

  glBegin(GL_QUADS);

    /* back side   */
  glColor3fv(rubik[i][j][k].facecolor[BACK]);
  glNormal3f(-1.0, 0.0, 0.0);
  glVertex3fv(rubik[i][j][k].vertex[0]);
  glVertex3fv(rubik[i][j][k].vertex[4]);
  glVertex3fv(rubik[i][j][k].vertex[7]);
  glVertex3fv(rubik[i][j][k].vertex[3]);

    /* front side */
  glColor3fv(rubik[i][j][k].facecolor[FRONT]);
  glNormal3f(1.0, 0, 0.0);
  glVertex3fv(rubik[i][j][k].vertex[1]);
  glVertex3fv(rubik[i][j][k].vertex[2]);
  glVertex3fv(rubik[i][j][k].vertex[6]);
  glVertex3fv(rubik[i][j][k].vertex[5]);
  
    /* right side */
  glColor3fv(rubik[i][j][k].facecolor[RIGHT]);
  glNormal3f(0.0, 1.0, 0.0);
  glVertex3fv(rubik[i][j][k].vertex[2]);
  glVertex3fv(rubik[i][j][k].vertex[3]);
  glVertex3fv(rubik[i][j][k].vertex[7]);
  glVertex3fv(rubik[i][j][k].vertex[6]);

    /* left side */
  glColor3fv(rubik[i][j][k].facecolor[LEFT]);
  glNormal3f(0.0, -1.0, 0.0);
  glVertex3fv(rubik[i][j][k].vertex[0]);
  glVertex3fv(rubik[i][j][k].vertex[1]);
  glVertex3fv(rubik[i][j][k].vertex[5]);
  glVertex3fv(rubik[i][j][k].vertex[4]);

    /* bottom side */
  glColor3fv(rubik[i][j][k].facecolor[BOTTOM]);
  glNormal3f(0.0, 0.0, -1.0);
  glVertex3fv(rubik[i][j][k].vertex[0]);
  glVertex3fv(rubik[i][j][k].vertex[3]);
  glVertex3fv(rubik[i][j][k].vertex[2]);
  glVertex3fv(rubik[i][j][k].vertex[1]);

    /* top side */
  glColor3fv(rubik[i][j][k].facecolor[TOP]);
  glNormal3f(0.0, 0.0, 1.0);
  glVertex3fv(rubik[i][j][k].vertex[4]);
  glVertex3fv(rubik[i][j][k].vertex[5]);
  glVertex3fv(rubik[i][j][k].vertex[6]);
  glVertex3fv(rubik[i][j][k].vertex[7]);

  glEnd();

  /* line around cubes*/
  glLineWidth(3.0);
  glBegin(GL_LINES);
  glColor3f(0.0,0.0,0.0);  

  glVertex3fv(rubik[i][j][k].vertex[0]); glVertex3fv(rubik[i][j][k].vertex[1]);
  glVertex3fv(rubik[i][j][k].vertex[6]); glVertex3fv(rubik[i][j][k].vertex[7]);
  glVertex3fv(rubik[i][j][k].vertex[4]); glVertex3fv(rubik[i][j][k].vertex[5]);
  glVertex3fv(rubik[i][j][k].vertex[2]); glVertex3fv(rubik[i][j][k].vertex[3]);  
  glVertex3fv(rubik[i][j][k].vertex[1]); glVertex3fv(rubik[i][j][k].vertex[5]);
  glVertex3fv(rubik[i][j][k].vertex[3]); glVertex3fv(rubik[i][j][k].vertex[7]);  
  glVertex3fv(rubik[i][j][k].vertex[0]); glVertex3fv(rubik[i][j][k].vertex[4]);
  glVertex3fv(rubik[i][j][k].vertex[2]); glVertex3fv(rubik[i][j][k].vertex[6]);  
  glVertex3fv(rubik[i][j][k].vertex[1]); glVertex3fv(rubik[i][j][k].vertex[2]);
  glVertex3fv(rubik[i][j][k].vertex[4]); glVertex3fv(rubik[i][j][k].vertex[7]);  
  glVertex3fv(rubik[i][j][k].vertex[0]); glVertex3fv(rubik[i][j][k].vertex[3]);
  glVertex3fv(rubik[i][j][k].vertex[5]); glVertex3fv(rubik[i][j][k].vertex[6]);
  
  glEnd();

}


void DrawRubik() {
  GLint i, j, k;

  for (i=0; i<DIM; i++) {
    for (j=0; j<DIM; j++) {
      for (k=0; k<DIM; k++) {
        DrawCube(i, j, k);
      }
    }
  }
}

void DrawSlice(char rotateAxis, GLint slice_no)
{
  GLint i, j, k;
 
  switch (rotateAxis) {
    case 'X': i = slice_no;
              for (j=0; j<DIM; ++j) {
                for (k=0; k<DIM; ++k) 
                  DrawCube(i, j, k);
              }
              break;
    case 'Y': j = slice_no;
              for (i=0; i<DIM; ++i) {
                for (k=0; k<DIM; ++k) 
                  DrawCube(i, j, k);
              }
              break;  
    case 'Z': k = slice_no;
              for (i=0; i<DIM; ++i) {
                for (j=0; j<DIM; ++j) 
                  DrawCube(i, j, k);
              }
  }
}

void DrawSlices(GLfloat spin, char axis, GLint i, GLint j, GLint k) {
  GLfloat axis_x = 0.0, axis_y = 0.0, axis_z = 0.0;
  GLint slice_no, index;

  switch (axis) {
  case 'X':
    axis_x = 1.0;
    slice_no = i;
    break;
  case 'Y':
    axis_y = 1.0;
    slice_no = j;
    break;
  case 'Z':
    axis_z = 1.0;
    slice_no = k;
    break;
  default:
    break;
  }

  for (index = 0; index < DIM; index++) 
    if (index != slice_no)
      DrawSlice(axis, index);

  glRotatef(spin, axis_x, axis_y, axis_z);
  DrawSlice(axis, slice_no);
}


// void DrawSample()
// {
//   glBegin(GL_QUADS);
//     //front
//     glColor3fv(Green);
//     glNormal3f(1, 0.0, 0);
//     glVertex3f(2, 2, 2);
//     //glNormal3f(1, 0.0, 0);
//     glVertex3f(2, -2, 2);
//     //glNormal3f(1, 0.0, 0);
//     glVertex3f(2, -2, -2);
//     //glNormal3f(1, 0.0, 0);
//     glVertex3f(2, 2, -2);
//     //back
//     glColor3fv(Cyan);
//     glNormal3f(-1, 0.0, 0);
//     glVertex3f(-2, -2, -2);
//     //glNormal3f(-1, 0.0, 0);
//     glVertex3f(-2, -2, 2);
//     //glNormal3f(-1, 0.0, 0);
//     glVertex3f(-2, 2, 2);
//     //glNormal3f(-1, 0.0, 0);
//     glVertex3f(-2, 2, -2);

//     //left
//     glColor3fv(Red);
//     glNormal3f(0, -1.0, 0);
//     glVertex3f(2, -2, 2);
//     //glNormal3f(0, -1.0, 0);
//     glVertex3f(-2, -2, 2);
//     //glNormal3f(0, -1.0, 0);
//     glVertex3f(-2, -2, -2);
//     //glNormal3f(0, -1.0, 0);
//     glVertex3f(2, -2, -2);
//     //right
//     glColor3fv(Blue);
//     glNormal3f(0, 1.0, 0);
//     glVertex3f(2, 2, 2);
//     //glNormal3f(0, 1.0, 0);
//     glVertex3f(2, 2, -2);
//     //glNormal3f(0, 1.0, 0);
//     glVertex3f(-2, 2, -2);
//     //glNormal3f(0, 1.0, 0);
//     glVertex3f(-2, 2, 2);

//     //top
//     glColor3fv(Magenta);
//     glNormal3f(0, 0.0, 1);
//     glVertex3f(2, 2, 2);
//     //glNormal3f(0, 0.0, 1);
//     glVertex3f(-2, 2, 2);
//     //glNormal3f(0, 0.0, 1);
//     glVertex3f(-2, -2, 2);
//     //glNormal3f(0, 0.0, 1);
//     glVertex3f(2, -2, 2);

//     //bottom
//     glColor3fv(Yellow);
//     glNormal3f(0, 0.0, -1.0);
//     glVertex3f(2, 2, -2);
//     //glNormal3f(0, 0.0, -1.0);
//     glVertex3f(2, -2, -2);
//     //glNormal3f(0, 0.0, -1.0);
//     glVertex3f(-2, -2, -2);
//     //glNormal3f(0, 0.0, -1.0);
//     glVertex3f(-2, 2, -2);
//   glEnd();
// }