#ifndef SRN_DISPLAY_H
#define SRN_DISPLAY_H

#include <SRN_Platform.h>

#include <glm/glm.hpp>
#include <gl/glew.h>

using namespace glm;

class SRN_API Display
{
public:
	// Constructor & Destructor
	Display();
	~Display();

	// Client called methods
	void clearScreen();
	void createViewPort(GLsizei width, GLsizei height);
	mat4 createProjectionMatrix(float fieldOfView, float aspectRatio, float nearPlane, float farPlane);
	mat4 createViewMatrix(vec3 eye, vec3 center, vec3 up);
};

#endif // SRN_DISPLAY_H