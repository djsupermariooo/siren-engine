#include <Model.h>
#include <iostream>
#include <Time.h>

using namespace Siren;

Shader shader;

void Model::Initialize(vector<glm::vec3> verts, vector<glm::vec2> uvs, vector<glm::vec3> normals, std::string strVertex, std::string strFragment)
{
	shader.Initialize(strVertex, strFragment);

	VerticesLength = verts.size();

	GLenum ErrorCheckValue = glGetError();

	glGenVertexArrays(1, &VertexArrayObjectId);
	glBindVertexArray(VertexArrayObjectId);

	glGenBuffers(1, &VertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferId);

	glBufferData(GL_ARRAY_BUFFER, sizeof(&verts[0]) * verts.size(), &verts[0], GL_STATIC_DRAW);

	glVertexAttribPointer(kVertexIndex, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	ErrorCheckValue = glGetError();

	if (ErrorCheckValue != GL_NO_ERROR)
	{
		fprintf(stderr, "ERROR: Could not create a VAO and VBO with error Id: %d\n", ErrorCheckValue);
		exit(-1);
	}
}

void Model::Render()
{
	shader.TurnOn();

	mat4 modelMatrix = translate(mat4(1.0f), Position);
	modelMatrix = scale(modelMatrix, Scale);
	modelMatrix = rotate(modelMatrix, Rotation.x, vec3(1, 0, 0));
	modelMatrix = rotate(modelMatrix, Rotation.y, vec3(0, 1, 0));
	modelMatrix = rotate(modelMatrix, Rotation.z, vec3(0, 0, 1));

	GLint modelMatrixId = shader.GetVariable("modelMatrix");
	GLint viewMatrixId = shader.GetVariable("viewMatrix");
	GLint projectionMatrixId = shader.GetVariable("projectionMatrix");

	shader.SetMatrix4(modelMatrixId, 1, GL_FALSE, &modelMatrix[0][0]);
	shader.SetMatrix4(viewMatrixId, 1, GL_FALSE, &ViewMatrix[0][0]);
	shader.SetMatrix4(projectionMatrixId, 1, GL_FALSE, &ProjectionMatrix[0][0]);

	glBindVertexArray(VertexArrayObjectId);

	glEnableVertexAttribArray(kVertexIndex);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glBindBuffer(GL_ARRAY_BUFFER, VertexBufferId);

	glDrawArrays(GL_TRIANGLES, 0, VerticesLength);

	glDisableVertexAttribArray(kVertexIndex);

	glBindVertexArray(0);
	
	shader.TurnOff();
}

void Model::Destroy()
{
	if (VertexBufferId)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &VertexBufferId);
		VertexBufferId = 0;
	}

	if (VertexArrayObjectId)
	{
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &VertexArrayObjectId);
		VertexArrayObjectId = 0;
	}

	shader.Destroy();
}

void Model::Display(mat4 viewMatrix, vec3 position)
{
	SetViewMatrix(viewMatrix);
	SetPosition(position);

	Render();
}
