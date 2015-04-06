#ifndef SRN_SKYBOX_H
#define SRN_SKYBOX_H

#include <Camera.h>
#include <Shader.h>
#include <SRN_Platform.h>

#include <gl/glew.h>

namespace Siren {

	enum SRN_API SkyboxSide
	{
		SRN_SKYBOX_FRONT,
		SRN_SKYBOX_BACK,
		SRN_SKYBOX_TOP,
		SRN_SKYBOX_BOTTOM,
		SRN_SKYBOX_LEFT,
		SRN_SKYBOX_RIGHT
	};

	class SRN_API Skybox
	{
	public:
		Skybox(GLfloat size, const char* rightFileName, const char* leftFileName, const char* topFileName, const char* bottomFileName, const char* backFileName, const char* frontFileName);
		~Skybox();

		void AddTexture(const char* fileName, GLuint side);
		void SetShaders(const char* strVertexFile, const char* strFragmentFile);
		void Draw();

		GLuint GetSize() { return m_Size; }
		void SetSize(GLfloat size) { m_Size = size; }
		Camera* GetCamera() { return camera; }
		void SetCamera(Camera* pCamera) { camera = pCamera; }

	private:
		GLfloat m_Size = 1;
		GLuint TextureBufferId;
		GLuint VertexBufferId;
		GLuint VertexArrayId;
		Shader shader;
		Camera* camera;

		static const GLfloat skyboxVerts[];

	};

}

#endif // SRN_SKYBOX_H