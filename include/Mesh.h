#ifndef SRN_MESH_H
#define SRN_MESH_H

#include <vector>
#include <string>

#include <Camera.h>
#include <MeshData.h>
#include <Shader.h>
#include <SRN_Platform.h>
#include <Texture.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <gl/glew.h>
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

namespace Siren {

	class SRN_API Mesh
	{
	protected:
		vector<VertexData> vertexData;
		vector<Texture> textureData;
		vector<GLuint> indices;

		GLuint VertexArrayId;
		GLuint VertexBufferId;
		GLuint IndexBufferId;

		vec3 Scale;
		vec3 Position;
		vec3 Rotation;
		Camera* camera;
		Shader shader;

	public:
		Mesh(string fileName);
		~Mesh();
		void InitMesh();
		void SetShaders(string strVertexFile, string strFragmentFile);
		void AddTexture(const char* fileName, GLuint type);
		void Draw();
		void RecursiveProcess(aiNode* node, const aiScene* scene);
		void ProcessMeshData(aiMesh* mesh, const aiScene* scene);

		vec3 GetPosition() { return Position; }
		void SetPosition(vec3 position) { Position = position; }
		vec3 GetScale() { return Scale; }
		void SetScale(vec3 scale) { Scale = scale; }
		vec3 GetRotation() { return Rotation; }
		void SetRotation(vec3 rotation) { Rotation = rotation; }
		Camera* GetCamera() { return camera; }
		void SetCamera(Camera *pCamera) { camera = pCamera; }

	};

}

#endif // SRN_MESH_H