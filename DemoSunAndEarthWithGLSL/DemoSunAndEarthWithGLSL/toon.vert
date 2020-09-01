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