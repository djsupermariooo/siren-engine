#ifndef SRN_MODEL_H
#define SRN_MODEL_H

#define GLM_FORCE_RADIANS

#include <SRN_Platform.h>
#include <gl/glew.h>
#include <Shader.h>
#include <stdio.h>
#include <stdlib.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
using namespace glm;

namespace Siren {

	class SRN_API Vertex3
	{
	public:
		float xyz[3];
		float rgba[4];
	};

	class SRN_API Model
	{
	public:
		static const GLuint kVertexIndex = 0;
		static const GLuint kColorIndex = 1;
		static const GLuint kTextureIndex = 2;
		static const GLuint kNormalIndex = 3;

		Model() { Scale = vec3(1.0f, 1.0f, 1.0f); }
		~Model() { Destroy(); }

		void Initialize(Vertex3 vertices[], int length, std::string strVertex, std::string strFragment);
		void Render();

		vec3 GetPosition() { return Position; }
		void SetPosition(vec3 position) { Position = position; }

		vec3 GetRotation() { return Rotation; }
		void SetRotation(vec3 rotation) { Rotation = rotation; }

		vec3 GetScale() { return Scale; }
		void SetScale(vec3 scale) { Scale = scale; }

		void SetViewMatrix(mat4 viewMatrix) { ViewMatrix = viewMatrix; }

		void SetProjectionMatrix(mat4 projectionMatrix) { ProjectionMatrix = projectionMatrix; }

		void Destroy();

	private:
		Vertex3 *Vertices;
		GLint VerticesLength;
		GLuint VertexBufferId;
		GLuint VertexArrayObjectId;

		vec3 Position;
		vec3 Rotation;
		vec3 Scale;
		mat4 ViewMatrix;
		mat4 ProjectionMatrix;

		Shader shader;

	};

}

#endif