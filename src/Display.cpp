#include <Display.h>
#include <glm/gtc/matrix_transform.hpp>
#include <gl/glew.h>

using namespace glm;

Display::Display()
{
}

Display::~Display()
{
}

void Display::clearScreen()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Display::createViewPort(GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);
}

mat4 Display::createProjectionMatrix(float fieldOfView, float aspectRatio, float nearPlane, float farPlane)
{
	return perspective(fieldOfView, aspectRatio, nearPlane, farPlane);
}

mat4 Display::createViewMatrix(vec3 eye, vec3 center, vec3 up)
{
	return lookAt(eye, center, up);
}
