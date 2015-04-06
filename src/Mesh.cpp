#include <Mesh.h>
#include <Texture.h>
#include <Time.h>

#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
using namespace Siren;

Mesh::Mesh(std::string fileName)
{
	std::cout << "Loading mesh: " << fileName << std::endl;
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName, aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
	if (scene == NULL)
	{
		std::cout << "The file failed to open. @" << fileName << std::endl;
		const char* errorString = importer.GetErrorString();
		std::cout << errorString << std::endl;
		return;
	}
	RecursiveProcess(scene->mRootNode, scene);
}

Mesh::~Mesh()
{
	camera = nullptr;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VertexBufferId);
	VertexBufferId = 0;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &IndexBufferId);
	IndexBufferId = 0;

	glBindVertexArray(0);
	glDeleteBuffers(1, &VertexArrayId);
	VertexArrayId = 0;
}

void Mesh::InitMesh()
{
	std::cout << "Initializing mesh" << std::endl;
	Scale = vec3(1.0f, 1.0f, 1.0f); // TODO: FIGURE OUT WHY NOT BEING USED
	Position = vec3(0.0f, 0.0f, 0.0f);
	Rotation = vec3(0.0f, 0.0f, 0.0f);

	std::cout << "Generating VAO" << std::endl;
	glGenVertexArrays(1, &VertexArrayId);
	std::cout << "Binding VAO " << VertexArrayId << std::endl;
	glBindVertexArray(VertexArrayId);
	std::cout << "VAO " << VertexArrayId << " successfully bound." << std::endl;

	std::cout << "Generating VBO" << std::endl;
	glGenBuffers(1, &VertexBufferId);
	std::cout << "Binding VBO " << VertexBufferId << std::endl;
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferId);
	std::cout << "Sending VBO data" << std::endl;
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * vertexData.size(), &vertexData[0], GL_STATIC_DRAW);

	std::cout << "Generating IBO" << std::endl;
	glGenBuffers(1, &IndexBufferId);
	std::cout << "Binding IBO " << IndexBufferId << std::endl;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferId);
	std::cout << "Sending IBO data" << std::endl;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

	std::cout << "Setting Vertex Attribute data" << std::endl;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), 0); //vertex
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(3 * sizeof(float))); //normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(6 * sizeof(float))); //tangent
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(9 * sizeof(float))); //color
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(12 * sizeof(float))); //UV
}

void Mesh::SetShaders(std::string strVertexFile, std::string strFragmentFile)
{
	shader.Initialize(strVertexFile, strFragmentFile);
}

void Mesh::Draw()
{
	shader.TurnOn();

	mat4 projectionMatrix = camera->GetProjectionMatrix();
	mat4 viewMatrix = camera->GetViewMatrix();

	mat4 modelMatrix = translate(mat4(1.0f), GetPosition());
	modelMatrix = scale(modelMatrix, GetScale());
	modelMatrix = rotate(modelMatrix, GetRotation().x, vec3(1, 0, 0));
	modelMatrix = rotate(modelMatrix, GetRotation().y, vec3(0, 1, 0));
	modelMatrix = rotate(modelMatrix, GetRotation().z, vec3(0, 0, 1));

	GLint modelMatrixId = shader.GetVariable("modelMatrix");
	GLint viewMatrixId = shader.GetVariable("viewMatrix");
	GLint projectionMatrixId = shader.GetVariable("projectionMatrix");

	shader.SetMatrix4(modelMatrixId, 1, GL_FALSE, &modelMatrix[0][0]);
	shader.SetMatrix4(viewMatrixId, 1, GL_FALSE, &viewMatrix[0][0]);
	shader.SetMatrix4(projectionMatrixId, 1, GL_FALSE, &projectionMatrix[0][0]);

	std::string str = "texture";
	for (int i = 0; i < textureData.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textureData[i].GetTextureId());
		glUniform1i(glGetUniformLocation(shader.getShaderProgramId(), (str + (char)(i + '0')).c_str()), i);
	}
	vec3 m_lightDirection = vec3(0, 1, 1);
	shader.SetFloat3(glGetUniformLocation(shader.getShaderProgramId(), "lightDirection"), m_lightDirection.x, m_lightDirection.y, m_lightDirection.z);

	glBindVertexArray(VertexArrayId);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);

	glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindVertexArray(0);

	shader.TurnOff();
}

void Mesh::RecursiveProcess(aiNode* node, const aiScene* scene)
{
	// process
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMeshData(mesh, scene);
	}

	// recursion
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		RecursiveProcess(node->mChildren[i], scene);
	}
}

void Mesh::ProcessMeshData(aiMesh* mesh, const aiScene* scene)
{
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		VertexData tmp;
		vec3 tmpVec;

		//position
		tmpVec.x = mesh->mVertices[i].x;
		tmpVec.y = mesh->mVertices[i].y;
		tmpVec.z = mesh->mVertices[i].z;
		tmp.position = tmpVec;

		//normals
		tmpVec.x = mesh->mNormals[i].x;
		tmpVec.y = mesh->mNormals[i].y;
		tmpVec.z = mesh->mNormals[i].z;
		tmp.normal = tmpVec;

		//tangents
		if (mesh->mTangents)
		{
			tmpVec.x = mesh->mTangents[i].x;
			tmpVec.y = mesh->mTangents[i].y;
			tmpVec.z = mesh->mTangents[i].z;
		}
		else
		{
			tmpVec.x = 1.0;
			tmpVec.y = tmpVec.z = 0;
		}
		tmp.tangent = tmpVec;

		//colors
		if (mesh->mColors[0])
		{
			tmpVec.x = mesh->mColors[0][i].r;
			tmpVec.y = mesh->mColors[0][i].g;
			tmpVec.z = mesh->mColors[0][i].b;
		}
		else
			tmpVec.x = tmpVec.y = tmpVec.z = 0.7f;
		tmp.color = tmpVec;

		//texture coordinates
		if (mesh->mTextureCoords[0])
		{
			tmpVec.x = mesh->mTextureCoords[0][i].x;
			tmpVec.y = mesh->mTextureCoords[0][i].y;
		}
		else
			tmpVec.x = tmpVec.y = 0.0;
		tmp.U = tmpVec.x;
		tmp.V = tmpVec.y;
		vertexData.push_back(tmp);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	InitMesh();
}

void Mesh::AddTexture(const char* fileName, GLuint type)
{
	Texture* tmp = new Texture(fileName, type);
	textureData.push_back(*tmp);
	delete tmp;
}
