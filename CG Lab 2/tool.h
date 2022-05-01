#ifndef TOOL_H
#define TOOL_H

#include"GLAD/glad.h"


const float twoPi;

typedef struct
{
	union {
		float c[2];

		struct {
			float x, y;
		};
		struct {
			float u, v;
		};
	};
}VEC2;

typedef struct
{
	union {
		float c[3];

		struct {
			float x, y,z;
		};
		struct {
			float r,g,b;
		};
	};
}VEC3;

typedef struct
{
	float m[16];
}MAT4;

extern const MAT4 idenMat4;
extern const MAT4 zeroMat4;


MAT4 MulMat4(MAT4* a, MAT4* b);

MAT4 RotZ(float rad);
MAT4 RotX(float rad);
MAT4 RotY(float rad);

MAT4 Projection(float n, float f, float r, float t);
MAT4 Translate(float x, float y, float z);

typedef struct
{
	GLuint vboPoints;
	GLuint vboNormals;
	GLuint vboTexCoords;
	GLuint ebo;

	int indicesCount;

	GLuint vao;
}MODEL;

MODEL GetRingModel(void);

GLuint GetRingProgram(void);

GLuint GetRingTexture(void);

#endif