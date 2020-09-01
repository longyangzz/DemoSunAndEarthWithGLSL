#include "gl/glew.h"
#include <GLFW/glfw3.h>
#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include <iostream>

void DrawCircle(float red, float green, float blue) {
	float xPos = 0;
	float yPos = 0;
	float radius = 1.0f;
	float prevX = xPos;
	float prevY = radius - yPos;
	const int steps = 10;
	const float angle = 3.1415936 * 2 / 360;
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

const GLfloat vertices[] = {
	-1.0f, -1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f,
	 1.0f,  1.0f, 0.0f,
	-1.0f, -1.0f, 0.0f,
	 1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f
};

const GLfloat colors[] = {
	0.0f, 0.0f, 1.0f,
	0.0f, 1.0f,0.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f
};

const GLchar* vertex120 = R"END(
#version 120
attribute vec4 inColor;
attribute vec4 inPosition;
uniform mat4 matrix;
varying vec4 outColor;
void main()
{
	outColor = inColor;
	gl_Position = inPosition;
}
)END";

const GLchar* raster120 = R"END(
#version 120
varying vec4 outColor;
void main()
{
	gl_FragColor = outColor;
}
)END";

char* readShaderSource(const char *fileName)
{
	FILE *fp;
	char *content = NULL;
	int count = 0;

	if (fileName != NULL)
	{
		fp = fopen(fileName, "rt");

		if (fp != NULL)
		{
			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);
			if (count > 0)
			{
				content = (char *)malloc(sizeof(char) * (count + 1));
				count = fread(content, sizeof(char), count, fp);
				content[count] = NULL;
			}
			fclose(fp);
		}
	}
	return content;
}

int main(int argc, char *argv[])
{
	GLFWwindow* window;
	//初始化freeglut
	//glutInit(&argc, argv);
	

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

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to init GLEW" << std::endl;
		return -1;
	}


	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	//!1、 顶点着色器
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	char* vertStr = readShaderSource("toon.vert");
	const char* vertex120 = vertStr;
	glShaderSource(vertexShader, 1, &vertex120, 0);
	glCompileShader(vertexShader);

	GLint compilationStatus;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compilationStatus);
	if(compilationStatus == GL_FALSE){
		GLchar message[256];
		glGetShaderInfoLog(vertexShader, sizeof(message),0,&message[0]);
		std::cout << message;
		exit(1);
	}

	//!2、 片元着色器
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	char* fragStr = readShaderSource("toon.frag");
	const char* raster120 = fragStr;
	glShaderSource(fragmentShader, 1, &raster120, 0);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compilationStatus);
	if (compilationStatus == GL_FALSE) {
		GLchar message[256];
		glGetShaderInfoLog(fragmentShader, sizeof(message), 0, &message[0]);
		std::cout << message;
		exit(1);
	}

	//！3、 shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &compilationStatus);
	if (compilationStatus == GL_FALSE) {
		GLchar message[256];
		glGetShaderInfoLog(shaderProgram, sizeof(message), 0, &message[0]);
		std::cout << message;
		exit(1);
	}

	glUseProgram(shaderProgram);

	//！4、vbo setup
	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	GLuint colorsBuffer;
	glGenBuffers(1, &colorsBuffer);

	//!5、send data to gpu
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, colorsBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	//5、locattion
	GLint attribPosition = glGetAttribLocation(shaderProgram, "inPosition");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

	GLint attribColor = glGetAttribLocation(shaderProgram, "inColor");
	glEnableVertexAttribArray(attribColor);
	glBindBuffer(GL_ARRAY_BUFFER, colorsBuffer);
	glVertexAttribPointer(attribColor, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//!6\ uniforms
	GLuint attributeMatrix = glGetUniformLocation(shaderProgram, "matrix");

	float alpha = 0;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0, 0.0, 0.0, 0);

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		//! 图元绘制
		float sa = 0.5 * sin(alpha);
		float ca = 0.5 * cos(alpha);
		alpha += 0.1;

		const GLfloat matrix[] = {
			sa, -ca, 0, 0,
			ca,  sa, 0, 0,
			0,    0, 1, 0,
			0,    0, 0, 1
		};
		glUniformMatrix4fv(attributeMatrix, 1, GL_FALSE, matrix);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}