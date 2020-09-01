#include <GLFW/glfw3.h>

#include "math.h"

int steps = 30;
const float angle = 3.1415926 * 2 / steps;

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	float xPos = 0;
	float yPos = 0;
	float radius = 1.0f;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(1.0, 1.0, 1.0, 0);

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		//! Í¼Ôª»æÖÆ
		/*glBegin(GL_TRIANGLES);

		glColor3f(1.0, 0, 0);
		glVertex3f(-0.6f, -0.4f, 0.f);
		glColor3f(0.0, 1.0, 0);
		glVertex3f(0.6f, -0.4f, 0.f);
		glColor3f(0.0, 0, 1.0);
		glVertex3f(0.f, 0.6f, 0.f);

		glEnd();*/

		float prevX = xPos;
		float prevY = radius - yPos;

		for (int i = 0; i <= steps; i++)
		{
			float newX = radius * sin(angle * i);
			float newY = radius * cos(angle * i);

			glBegin(GL_TRIANGLES);

			glColor3f(1.0, 0.5, 0);
			glVertex3f(-0.0f, 0.0f, 0.f);
			glVertex3f(prevX, prevY, 0.f);
			glVertex3f(newX, newY, 0.f);

			glEnd();

			prevX = newX;
			prevY = newY;
		}

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}