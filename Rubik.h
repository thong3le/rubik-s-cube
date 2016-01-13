#ifndef RUBIK_H
#define RUBIK_H

GLfloat SPEED;
GLfloat Blue[4];
GLfloat Yellow[4];
GLfloat Green[4];
GLfloat Red[4];
GLfloat Cyan[4];
GLfloat Magenta[4];
GLfloat Gray[4];
GLfloat Black[4];
GLfloat White[4];

void direction(GLfloat clickx, GLfloat clicky, GLfloat clickz,
            GLfloat x, GLfloat y, GLfloat z, GLint face,
            GLint *dir);
void angle(GLint face, GLint spin_dir, GLfloat *spin);
void axis(GLint face, GLint spin_dir, char *axis);
void update(GLint face, char axis, GLint i, GLint j, GLint k, GLfloat spin);
void get_position(GLfloat x, GLfloat y, GLfloat z, GLint *face, GLint *i, GLint *j, GLint *k);
void randomRubik();
void initRubik(GLint dim);
void DrawRubik();
void DrawSlices(GLfloat spin, char axis, GLint i, GLint j, GLint k);

#endif