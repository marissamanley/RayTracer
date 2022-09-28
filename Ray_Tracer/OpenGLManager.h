#pragma once
#include "LightSource.h"
#include "Camera.h"
#include "Ray.h"
#include "Shape.h"
#include "Sphere.h"
#include "Tetrahedron.h"
#include "Plane.h"
#include "RayTracer.h"
/*
* The purpose of this class is to execute all OpenGL-related functions 
* to create the output window and correcly bind and swap buffers every frame.
* The render loop and all user input is processed within this class as well. 
*/
class OpenGLManager {
private:
	RayTracer rayTracer;
	GLFWwindow* window;

	// Declaration of output screen size
	const unsigned int SCR_WIDTH = 1080;
	const unsigned int SCR_HEIGHT = 1080;

	//OpenGL HLSL code for the Vertex and Fragment shaders
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aColor;\n"
		"layout (location = 2) in vec2 aTexCoord;\n"
		"out vec3 ourColor;\n"
		"out vec2 TexCoord;\n"
		"void main()\n"
		"{\n"
		"gl_Position = vec4(aPos, 1.0);\n"
		"ourColor = aColor;\n"
		"TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
		"}\0";

	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec3 ourColor;\n"
		"in vec2 TexCoord;\n"
		"uniform sampler2D texture1;\n"
		"void main()\n"
		"{\n"
		"   FragColor = texture(texture1, TexCoord);\n"
		"}\n\0";

	//values to instantiate the viewing screen
	float vertices[32] = {
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, 
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f };
	unsigned int indices[6] = {
		0, 1, 3,
		1, 2, 3 };

public:
	void ProcessInput();
	void Start();
	OpenGLManager();
};

