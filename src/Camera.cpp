#include <Camera.h>
#include <Input.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace Siren;
using namespace glm;

Camera::Camera()
{
	Position = vec3(0.0f, 0.0f, 0.0f);
}

Camera::~Camera()
{

}

mat4 Camera::SetProjection(float fov, float aspectRatio, float nearPlane, float farPlane)
{
	ProjectionMatrix = perspective(fov, aspectRatio, nearPlane, farPlane);
	return ProjectionMatrix;
}

mat4 Camera::GetViewMatrix()
{
	return GetRotationMatrix() * inverse(translate(mat4(), Position));
}

mat4 Camera::GetRotationMatrix()
{
	mat4 rotationMatrix(1.0f);
	rotationMatrix = rotate(rotationMatrix, Pitch, vec3(1.0f, 0.0f, 0.0f));
	rotationMatrix = rotate(rotationMatrix, Yaw, vec3(0.0f, 1.0f, 0.0f));
	return rotationMatrix;
}

void Camera::PositionCamera(float positionX, float positionY, float positionZ, float yaw, float pitch)
{
	Position = vec3(positionX, positionY, positionZ);
	Yaw = yaw;
	Pitch = pitch;
}

vec3 Camera::GetView()
{
	vec4 viewVector = inverse(GetRotationMatrix()) * vec4(0.0f, 0.0f, -1.0f, 1.0f);
	return vec3(viewVector);
}

vec3 Camera::GetUp()
{
	vec4 upVector = inverse(GetRotationMatrix()) * vec4(0.0f, 1.0f, 0.0f, 1.0f);
	return vec3(upVector);
}

vec3 Camera::GetRight()
{
	vec4 rightVector = inverse(GetRotationMatrix()) * vec4(1.0f, 0.0f, 0.0f, 1.0f);
	return vec3(rightVector);
}

void Camera::MoveCamera(float speed)
{
	vec3 viewVector = GetView();
	Position.x += viewVector.x * speed;
	Position.z += viewVector.z * speed;
}

void Camera::Strafe(float speed)
{
	vec3 rightVector = GetRight();
	Position.x += rightVector.x * speed;
	Position.z += rightVector.z * speed;
}

void Camera::SetViewByMouse(float offsetX, float offsetY)
{
	Yaw += offsetX * MouseSpeed;
	Pitch += offsetY * MouseSpeed;

	//if (Yaw > 2 * PI)
		//Yaw = 0;

	if (Yaw < 0)
		Yaw = 2 * PI;

	if (Pitch > radians(75.0f))
		Pitch = radians(75.0f);

	if (Pitch < radians(-75.0f))
		Pitch = radians(-75.0f);

	Input::mouseX = 0;
	Input::mouseY = 0;
}