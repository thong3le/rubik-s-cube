#include <stdio.h>
#include <math.h>

#include "Vec.h"

void
v3Set(vec3 V, float A, float B, float C)
{
  V[0] = A; V[1] = B; V[2] = C;
}

void
v3Copy(vec3 Src, vec3 Dst)
{
  for (int i = 0; i < 3; i++) Dst[i] = Src[i];
}

void
v3Scale(vec3 V, float S)
{
  for (int i = 0; i < 3; i++) V[i] *= S;
}

void
v3Add(vec3 A, vec3 B, vec3 C)
{
  for (int i = 0; i < 3; i++) C[i] = A[i] + B[i];
}

void
v3Sub(vec3 A, vec3 B, vec3 C)
{
  for (int i = 0; i < 3; i++) C[i] = A[i] - B[i];
}

float
v3Dot(vec3 A, vec3 B)
{ float dot;
  
  dot = 0;
  for (int i = 0; i < 3; i++) dot += A[i] * B[i];
  
  return dot;
}

void
v3Cross(vec3 A, vec3 B, vec3 C)
{
	C[0] = (A[1] * B[2]) - (A[2] * B[1]);
	C[1] = (A[2] * B[0]) - (A[0] * B[2]);
	C[2] = (A[0] * B[1]) - (A[1] * B[0]);
}

float
v3Length(vec3 V)
{
  return sqrt(v3Dot(V,V));
}

void
v3Normalize(vec3 V)
{
  v3Scale(V,1/v3Length(V));
}

//=====================

void
v4Set(vec4 V, float A, float B, float C, float D)
{
  V[0] = A; V[1] = B; V[2] = C; V[3] = D;
}

void
v4Scale(vec4 V, float S)
{
  for (int i = 0; i < 4; i++) V[i] *= S;
}

float
v4Dot(vec4 A, vec4 B)
{ float dot;
  
  dot = 0;
  for (int i = 0; i < 4; i++) dot += A[i] * B[i];
  
  return dot;
}

float
v4Length(vec4 V)
{
  return sqrt(v4Dot(V,V));
}

void
v4Normalize(vec4 V)
{
  v4Scale(V,1/v4Length(V));
}

