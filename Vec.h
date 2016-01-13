#ifndef VEC_H
#define VEC_H

typedef float vec3[3];
typedef float vec4[4];
typedef float mat3[3][3];
typedef float mat4[4][4];

void  v3Set(vec3 V, float A, float B, float C);
void  v3Copy(vec3 Src, vec3 Dst);
void  v3Scale(vec3 V, float S);
void  v3Add(vec3 A, vec3 B, vec3 C);
void  v3Sub(vec3 A, vec3 B, vec3 C);
float v3Dot(vec3 A, vec3 B);
void  v3Cross(vec3 A, vec3 B, vec3 C);
float v3Length(vec3 V);
void  v3Normalize(vec3 V);

void  v4Set(vec4 V, float A, float B, float C, float D);
void  v4Scale(vec4 V, float S);
float v4Dot(vec4 A, vec4 B);
void  v4Normalize(vec4 V);

#endif
