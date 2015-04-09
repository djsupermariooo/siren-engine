#ifndef SRN_CAMERA_H
#define SRN_CAMERA_H

#define PI 3.14159265358979323846264338327950288

#include <SRN_Platform.h>

#include <glm/glm.hpp>

using namespace glm;

namespace Siren {

	class SRN_API Camera
	{
	public:
		Camera();
		~Camera();
		mat4 SetProjection(float fov, float aspectRatio, float nearPlane, float farPlane);
		mat4 GetProjectionMatrix() { return ProjectionMatrix; }
		mat4 GetViewMatrix();
		mat4 GetRotationMatrix();
		void SetPosition(vec3 position) { Position = position; }
		vec3 GetPosition() { return Position; }
		vec3 GetView();
		vec3 GetUp();
		vec3 GetRight();
		float GetYaw() { return Yaw; }
		void SetYaw(float yaw) { Yaw = yaw; }
		float GetPitch() { return Pitch; }
		void SetPitch(float pitch) { Pitch = pitch; }
		float GetSpeed() { return Speed; }
		void SetSpeed(float speed) { Speed = speed; }
		float GetRotationSpeed() { return RotationSpeed; }
		void SetRotationSpeed(float speed) { RotationSpeed = speed; }
		void SetViewByController(int controllerX, int controllerY);
		void SetViewByMouse(float mouseX, float mouseY);
		void PositionCamera(float positionX, float positionY, float positionZ, float yaw, float pitch);
		void MoveCamera(float speed);
		void Strafe(float speed);

	protected:
		mat4 ProjectionMatrix;
		vec3 Position;
		float RotationSpeed = 2.0f;
		float MouseSpeed = 0.001f;
		float Speed = 5.0f;
		float Yaw = 0.0f;
		float Pitch = 0.0f;
	};

}

#endif // SRN_CAMERA_H