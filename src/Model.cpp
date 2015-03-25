#include <Model.h>

using namespace Siren;

Shader shader;

void Model::Initialize(Vertex3 vertices[], int length, std::string strVertex, std::string strFragment)
{
	shader.Initialize(strVertex, strFragment);

	Vertices = vertices;
	VerticesLength = length;

	GLenum ErrorCheckValue = glGetError();

	glGenVertexArrays(1, &VertexArrayObjectId);
	glBindVertexArray(VertexArrayObjectId);

	glGenBuffers(1, &VertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferId);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices[0]) * length, Vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(kVertexIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertices[0]), 0);

	glVertexAttribPointer(kColorIndex, 4, GL_FLOAT, GL_FALSE, sizeof(Vertices[0]), (GLvoid*)sizeof(Vertices[0].xyz));

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

	shader.SetMatrix4(modelMatrixId, 1, false, &modelMatrix[0][0]);
	shader.SetMatrix4(viewMatrixId, 1, false, &ViewMatrix[0][0]);
	shader.SetMatrix4(projectionMatrixId, 1, false, &ProjectionMatrix[0][0]);

	glBindVertexArray(VertexArrayObjectId);

	glEnableVertexAttribArray(kVertexIndex);

	glEnableVertexAttribArray(kColorIndex);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glDrawArrays(GL_TRIANGLES, 0, VerticesLength);

	glDisableVertexAttribArray(kColorIndex);

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
