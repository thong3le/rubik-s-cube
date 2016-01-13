/*

  Trackball style user interface. Converts mouse motion to quaternions, accumulates quaternions 
  and produces transformation matrix for use in drawin. 

*/

#include <stdio.h>
#include <math.h>
#include <GLUT/glut.h>
//#include "mathConstants.h"
#include "Vec.h"
#include "TrackballUI.h"

#define TRACKBALL_SIZE 0.8
#define CLAMP(V,Min,Max) ( (V < Min) ? (Min) : ( (V > Max) ? (Max) : (V) ) )

int WinWidth, WinHeight;
int ButState, LastX, LastY, StartX, StartY;

vec4 Q;

float
ProjToSphere(float R, float X, float Y)
{ float d, t, z;
  
  d = sqrt(X*X + Y*Y);
  if (d < R*M_SQRT1_2) {
    z = sqrt(R*R - d*d);
  }
  else {
    t = R / M_SQRT2;
    z = t*t / d;
  }
  return z;
}

void
AxisToQuat(vec3 Axis, float Phi, vec4 Q)
{
  v3Normalize(Axis);
  v3Copy(Axis,Q);
  v3Scale(Q,sin(Phi/2.0));
  Q[3] = cos(Phi/2.0);
}

void
ComputeQuat(vec4 RetQ, int SX, int SY, int EX, int EY)
{ vec3 p1, p2, d, rotAxis;
  float t, rotAngle;
  float p1x, p1y, p2x, p2y;
  
  if (SX == EX && SY == EY) {
    v4Set(RetQ,0,0,0,1);
    return;
  }
  
  p1x = 2 * ((SX / (float) WinWidth) - 0.5);
  p1y = -2 * ((SY / (float) WinHeight) - 0.5);
  p2x = 2 * ((EX / (float) WinWidth) - 0.5);
  p2y = -2 * ((EY / (float) WinHeight) - 0.5);
  
  v3Set(p1,p1x,p1y,ProjToSphere(TRACKBALL_SIZE,p1x,p1y));
  v3Set(p2,p2x,p2y,ProjToSphere(TRACKBALL_SIZE,p2x,p2y));
  v3Cross(p2,p1,rotAxis);
  
  v3Sub(p1,p2,d);
  t = CLAMP(v3Length(d) / (2.0 * TRACKBALL_SIZE),-1,1);
  rotAngle = 2.0 * asin(t);
  
  AxisToQuat(rotAxis,rotAngle,RetQ);
}

void
CombineQuats(vec4 Q1, vec4 Q2, vec4 Dest)
{ vec3 t1, t2, t3;
  
  v3Copy(Q1,t1); v3Scale(t1,Q2[3]);
  v3Copy(Q2,t2); v3Scale(t2,Q1[3]);
  v3Cross(Q2,Q1,t3);
  v3Add(t1,t2,Dest);
  v3Add(t3,Dest,Dest);
  Dest[3] = Q1[3] * Q2[3] - v3Dot(Q1,Q2);
  v4Normalize(Dest);
}

//========================

void
TB_Init()
{
  v4Set(Q,0,0,0,1);
}

void
TB_WinResize(int AWidth, int AHeight)
{
  WinWidth = AWidth;
  WinHeight = AHeight;
}

void
TB_MouseButtonEvent(int But, int State, int X, int Y)
{
  ButState = State;
  LastX = StartX = X;
  LastY = StartY = Y;
}

void
TB_MouseMotionEvent(int X, int Y)
{ vec4 q;
  
  ComputeQuat(q,LastX,LastY,X,Y);
  CombineQuats(q,Q,Q);
  LastX = X; LastY = Y;
}

void
TB_Reset()
{
  v4Set(Q,0,0,0,1);
}

void
TB_SetTransform(mat4 M)
{
  M[0][0] = 1.0 - 2.0 * (Q[1] * Q[1] + Q[2] * Q[2]);
	M[0][1] = 2.0 * (Q[0] * Q[1] - Q[2] * Q[3]);
	M[0][2] = 2.0 * (Q[2] * Q[0] + Q[1] * Q[3]);
	M[0][3] = 0.0;
  
	M[1][0] = 2.0 * (Q[0] * Q[1] + Q[2] * Q[3]);
	M[1][1] = 1.0 - 2.0 * (Q[2] * Q[2] + Q[0] * Q[0]);
	M[1][2] = 2.0 * (Q[1] * Q[2] - Q[0] * Q[3]);
	M[1][3] = 0.0;
  
	M[2][0] = 2.0 * (Q[2] * Q[0] - Q[1] * Q[3]);
	M[2][1] = 2.0 * (Q[1] * Q[2] + Q[0] * Q[3]);
	M[2][2] = 1.0 - 2.0 * (Q[1] * Q[1] + Q[0] * Q[0]);
	M[2][3] = 0.0;
  
	M[3][0] = 0.0;
	M[3][1] = 0.0;
	M[3][2] = 0.0;
	M[3][3] = 1.0;

}
