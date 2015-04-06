#include <Model.h>
#include <iostream>
#include <Time.h>

using namespace Siren;

Shader shader;

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
