#ifndef TRACKBALLUI_H
#define TRACKBALLUI_H

#include "Vec.h"

void TB_Init();
void TB_WinResize(int AWidth, int AHeight);
void TB_MouseButtonEvent(int But, int State, int X, int Y);
void TB_MouseMotionEvent(int X, int Y);
void TB_Reset();
void TB_SetTransform(mat4 M);

#endif
