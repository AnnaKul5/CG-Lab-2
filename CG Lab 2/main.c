#include<stdio.h>
#include<math.h>

#include"GLAD/glad.h"
#include"GLFW/glfw3.h"

#include"tool.h"

#define WIN_WIDTH  800
#define WIN_HEIGHT 600

int main(void)
{
	//---WINDOW & OPENGL INITIALIZATION
	/**********************************************************/


	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);// Выбрали версию 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//Выбрали версию, где функции только для версии 3.3

	GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Demo", (void*)0, (void*)0); // создали указатель на окно

	glfwMakeContextCurrent(window);//Для того чтобы окно и программа подружились

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);//Для адесов графических функций

	//---PREPARING RENDER DATA
	/**********************************************************/

	//---MODEL

	MODEL modelRing = GetRingModel();

	glBindVertexArray(modelRing.vao);

	//---SHADERS

	GLint progRing = GetRingProgram();


	GLint progRingLocModel = glGetUniformLocation(progRing, "model"); // получили индентификатор на эту переменную модел
	GLint progRingLocProj = glGetUniformLocation(progRing, "proj");
	GLint progRingLocTex = glGetUniformLocation(progRing, "tex");

	glUseProgram(progRing); // сделать программу активной

	//---Texture

	GLuint texRing = GetRingTexture();

	glUniform1i(progRingLocTex, 2);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texRing);

	//---Variables 

	int i, ringNums = 6;
	float time;
	MAT4 rotY, rotZ, tran, res;



	//glClearColor(0.0f,0.0f,0.0f,1.0f);

	//---RENDER LOOP
	/**********************************************************/
	
	glEnable(GL_CULL_FACE);// по умолчанию выключена 
	//glFrontFace(GL_CCW);// против часовой стрелки

	glEnable(GL_DEPTH_TEST);// Тест глубины 
	//glDepthFunc(GL_LESS);
	

	MAT4 proj = Projection(0.1f, 100.0f, 1.0f, 1.0f * WIN_HEIGHT/ WIN_WIDTH);//верхняя и нижняя граница, ответственная за угол
	
	glUniformMatrix4fv(progRingLocProj, 1, GL_TRUE, proj.m);


	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while(!glfwWindowShouldClose(window))
	{

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float time = glfwGetTime();

		for (i = 0; i < ringNums; ++i)
		{
			{
				res = idenMat4;

				//---stage 0
				rotZ = RotZ(-time);
				res = MulMat4(&rotZ, &res);

				// ---stage 1
				tran = Translate(0.0f, 0.0f, 4.0f);
				res = MulMat4(&tran, &res);

				//---stage 2
				rotY = RotY(i*(twoPi/ringNums) + time*0.5);
				res = MulMat4(&rotY, &res);

				//---stage 3 
				tran = Translate(0.0f, 0.0f, -7.0f);
				res = MulMat4(&tran, &res);

				glUniformMatrix4fv(progRingLocModel, 1, GL_TRUE, res.m);// сейчас будем загружать данные для матрикс

				glDrawElements(GL_TRIANGLES, modelRing.indicesCount, GL_UNSIGNED_INT, (const void*)0);
			}
		}
		glfwSwapBuffers(window);//Выводит картинку на экран 

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)// Сделали так, чтобы нажимая кнопку esc закрывалось окно
			glfwSetWindowShouldClose(window, 1);

		glfwPollEvents();
	}

	//---APP TERMINATION
	/**********************************************************/
	glDeleteTextures(1,&texRing);
	glDeleteProgram(progRing);
	glDeleteVertexArrays(1, &modelRing.vao);

	glDeleteBuffers(1, &modelRing.vboPoints);
	glDeleteBuffers(1, &modelRing.vboNormals);
	glDeleteBuffers(1, &modelRing.vboTexCoords);

	glDeleteBuffers(1, &modelRing.ebo);


	glfwTerminate();

	return 0;
}