#include <GLFW/glfw3.h>

#include "math.h"

int steps = 30;
const float angle = 3.1415926 * 2 / steps;

void DrawCircle(float red, float green, float blue) {
	float xPos = 0;
	float yPos = 0;
	float radius = 1.0f;
	float prevX = xPos;
	float prevY = radius - yPos;

	for (int i = 0; i <= steps; i++)
	{
		float newX = radius * sin(angle * i);
		float newY = radius * cos(angle * i);

		glBegin(GL_TRIANGLES);

		glColor3f(red, green, blue);
		glVertex3f(-0.0f, 0.0f, 0.f);
		glVertex3f(prevX, prevY, 0.f);
		glVertex3f(newX, newY, 0.f);

		glEnd();

		prevX = newX;
		prevY = newY;
	}
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(800, 800, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glScalef(0.1, 0.1, 1);    //[-1,1] -->[-10,10]  ÆÁÄ»×ø±ê·¶Î§

	float angle = 0;
	float angleMoon = 0;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		angle += 1;

		glClearColor(0.0, 0.0, 0.0, 0);

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		//! Í¼Ôª»æÖÆ
		//!»æÖÆÌ«Ñôsun
		DrawCircle(1.0, 1.0, 0);
		
		//! µØÇò
		{
			glPushMatrix();
			
			glRotatef(angle, 0, 0, 1);   //ÈÆzÖáÐý×ª
			glTranslatef(0, 5, 0);
			glScalef(0.6, 0.6, 0.6);
			DrawCircle(0, 0.3, 1);

			//! »æÖÆÔÂÁÁ
			{
				glPushMatrix();
				glRotatef(angleMoon, 0, 0, 1);
				glTranslatef(0, 3, 0);
				glScalef(0.5, 0.5, 1);
				DrawCircle(0.5, 0.5, 0.5);
				glPopMatrix();
				angleMoon += 3;
			}

			glPopMatrix();
		}
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}