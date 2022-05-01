#include"tool.h"

#include<math.h>

const float twoPi = 6.283185307179586f;

const MAT4 zeroMat4 = {
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
};

const MAT4 idenMat4 = {
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
};

MAT4 MulMat4(MAT4* a, MAT4* b)
{
	MAT4 m = zeroMat4;

	int i, j, k;

	for (i = 4; i--; )
		for (j = 4; j--; )
			for (k = 4; k--; )
				m.m[4 * i + j] += a->m[4 * i + k] * b->m[4 * k + j];

	return m;
};

MAT4 Translate(float x, float y, float z)
{
	MAT4 m = {
		1.0f, 0.0f, 0.0f, x   ,
		0.0f, 1.0f, 0.0f, y   ,
		0.0f, 0.0f, 1.0f, z   ,
		0.0f, 0.0f, 0.0f, 1.0f,
	};

	return m;
};

MAT4 RotZ(float rad)
{
	float c = cosf(rad);
	float s = sinf(rad);

	MAT4 m = {
		   c,   -s, 0.0f, 0.0f,
		   s,    c, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	return m;
}

MAT4 RotX(float rad)
{
	float c = cosf(rad);
	float s = sinf(rad);

	MAT4 m = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f,    c,   -s, 0.0f,
		0.0f,    s,    c, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	return m;
}


MAT4 RotY(float rad)
{
	float c = cosf(rad);
	float s = sinf(rad);

	MAT4 m = {
		   c, 0.0f,    s,   0.0f,
		0.0f, 1.0f,   0.0f, 0.0f,
		  -s, 0.0f,    c,   0.0f,
		0.0f, 0.0f, 0.0f,   1.0f,
	};
	return m;
}

MAT4 Projection(float n, float f, float r, float t)
{
	MAT4 m = {
		 1.0f / r,  0.0f  ,  0.0f           ,  0.0f                ,
		 0.0f  ,  1.0f / t,  0.0f           ,  0.0f                ,
		 0.0f  ,  0.0f  , -(f + n) / (f - n), -2.0f * n * f / (f - n),
		 0.0f  ,  0.0f  , -1.0f           ,  0.0f                ,
	};

	return m;
};

#define SMALL 32
#define LARGE 64

MODEL GetRingModel(void)
{
	int   i, j, k;
	float angle, cosine, sine, length;

	VEC3 p[(LARGE + 1) * (SMALL + 1)];
	VEC3 n[(LARGE + 1) * (SMALL + 1)];
	VEC2 t[(LARGE + 1) * (SMALL + 1)];

	for (j = 0; j < SMALL + 1; ++j)
	{
		angle = j < SMALL ? twoPi * j / SMALL : 0.0f;

		p[j].x = 0.5f * cosf(angle);
		p[j].y = 0.0f;
		p[j].z = 0.5f * -sinf(angle);

		length = sqrtf(p[j].x * p[j].x + p[j].y * p[j].y + p[j].z * p[j].z);

		n[j].x = p[j].x / length;
		n[j].y = p[j].y / length;
		n[j].z = p[j].z / length;

		p[j].x += 1.5f;

		t[j].u = 0.0f;
		t[j].v = 4.0f * j / SMALL;
	}

	for (i = 1; i < LARGE + 1; ++i)
	{
		angle = i < LARGE ? twoPi * i / LARGE : 0.0f;
		cosine = cosf(angle);
		sine = sinf(angle);

		for (j = 0; j < SMALL + 1; ++j)
		{
			k = i * (SMALL + 1) + j;

			p[k].x = p[j].x * cosine;
			p[k].y = p[j].x * sine;
			p[k].z = p[j].z;

			n[k].x = n[j].x * cosine;
			n[k].y = n[j].x * sine;
			n[k].z = n[j].z;

			t[k].u = 8.0f * i / LARGE;
			t[k].v = t[j].v;
		}
	}

	MODEL model;

	model.indicesCount = 0;

	unsigned indices[6 * SMALL * LARGE];

	for (i = 0; i < LARGE; ++i)
	{
		for (j = 0; j < SMALL; ++j)
		{
			indices[model.indicesCount++] = (i + 1) * (SMALL + 1) + (j + 0);
			indices[model.indicesCount++] = (i + 0) * (SMALL + 1) + (j + 0);
			indices[model.indicesCount++] = (i + 0) * (SMALL + 1) + (j + 1);

			indices[model.indicesCount++] = (i + 1) * (SMALL + 1) + (j + 0);
			indices[model.indicesCount++] = (i + 0) * (SMALL + 1) + (j + 1);
			indices[model.indicesCount++] = (i + 1) * (SMALL + 1) + (j + 1);
		}
	}

	glGenVertexArrays(1, &model.vao);
	glBindVertexArray(model.vao);

	glGenBuffers(1, &model.vboPoints);
	glBindBuffer(GL_ARRAY_BUFFER, model.vboPoints);
	glBufferData(GL_ARRAY_BUFFER, sizeof p, (void*)p, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &model.vboNormals);
	glBindBuffer(GL_ARRAY_BUFFER, model.vboNormals);
	glBufferData(GL_ARRAY_BUFFER, sizeof n, (void*)n, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &model.vboTexCoords);
	glBindBuffer(GL_ARRAY_BUFFER, model.vboTexCoords);
	glBufferData(GL_ARRAY_BUFFER, sizeof t, (void*)t, GL_STATIC_DRAW);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &model.ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, (void*)indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return model;
};


GLuint GetRingProgram(void)
{
	const char* stringVertexShader =
		"   #version 330 core\n"
		"   uniform mat4 model;"
		"   uniform mat4 proj;\n"
		"	layout(location = 0) in vec3 pos;"
		"   layout(location = 1) in vec3 norm;"
		"   layout(location = 2) in vec2 uv;"
		"   out vec3 ipos;"
		"   out vec3 inorm;"
		"   out vec2 iuv;"
		"	void main(void)"
		"	{"
		"       ipos = (model * vec4(pos, 1.0)).xyz;\n"
		"       inorm = (model * vec4(norm, 0.0)).xyz;\n"
		"       iuv = uv;\n"
		"       gl_Position = proj * model * vec4(pos, 1.0);\n"
		"	}\n";

	const char* stringFragmentShader =
		"#version 330 core\n"
		"   uniform sampler2D tex;\n"
		"in vec3 inorm;\n"
		"in vec2 iuv;\n"
		"in vec3 ipos;\n"
		"out vec4 fragColor;\n"
		"void main(void)\n"
		"{\n"
		" vec3 normal = normalize(inorm);"
		" vec3 dirView = normalize(ipos);"
		" vec3 dirLight = dirView;" // Глаз и источник света из начала координат 

		" vec3 diffProp = texture(tex, iuv).rgb * vec3(1.0, 0.0, 0.0);"
		" vec3 specProp = vec3(0.3);"

		" float diffCoef = max(dot(normal, -dirLight),0.0);"
		" float specCoef = pow(max(dot(reflect(dirLight, normal), -dirView), 0.0),32);"
		" float distance = length(ipos);"

		" vec3 result = (diffProp * diffCoef + specProp * specCoef) / pow((0.4 * distance),3.0);"

		" fragColor = vec4(result, 1.0);"
		" "
		"}\n";

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &stringVertexShader, (void*)0);
	glShaderSource(fragmentShader, 1, &stringFragmentShader, (void*)0);

	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);


	GLuint program = glCreateProgram();

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);

	return program;
};


GLuint GetRingTexture(void)
{
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);// Текстура аналогично буферу 

	// Включается по умолчанию 
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// Второй параметр определяет, чем будут закрашиваться пустые области
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Тоже самое, только верхний правый угол

	//Указываем обязательно!!
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);// Смазывание краёв текстуры. Мин фильтр маленькую текстуру натягивает на большой объект
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);// Маг фильтр натягивает большую текстуру на маленький объект 

	{
		unsigned buffer[2 * 2] = { 0, ~0, ~0, 0 };

		glTexImage2D(
			GL_TEXTURE_2D,    //---current texture target
			0,                //---tells which mipmap level is loaded
			GL_RGB,           //---format we want OpenGL to store our texture
			2,                //---resulting width  of texture
			2,                //---resulting height of texture
			0,                //---must be zero
			GL_RGBA,          //---source image format
			GL_UNSIGNED_BYTE, //---color component type
			buffer);          //---pointer to source image data
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	return tex;
};
