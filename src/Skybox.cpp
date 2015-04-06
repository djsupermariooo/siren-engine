#include <Skybox.h>
#include <Texture.h>

#include <glm/glm.hpp>

using namespace glm;
using namespace Siren;

Skybox::Skybox(GLfloat size, const char* rightFileName, const char* leftFileName, const char* topFileName, const char* bottomFileName, const char* backFileName, const char* frontFileName)
{
	m_Size = size;

	const GLfloat skyboxVerts[] = { -m_Size, m_Size, -m_Size,
		-m_Size, -m_Size, -m_Size,
		m_Size, -m_Size, -m_Size,
		m_Size, -m_Size, -m_Size,
		m_Size, m_Size, -m_Size,
		-m_Size, m_Size, -m_Size,

		-m_Size, -m_Size, m_Size,
		-m_Size, -m_Size, -m_Size,
		-m_Size, m_Size, -m_Size,
		-m_Size, m_Size, -m_Size,
		-m_Size, m_Size, m_Size,
		-m_Size, -m_Size, m_Size,

		m_Size, -m_Size, -m_Size,
		m_Size, -m_Size, m_Size,
		m_Size, m_Size, m_Size,
		m_Size, m_Size, m_Size,
		m_Size, m_Size, -m_Size,
		m_Size, -m_Size, -m_Size,

		-m_Size, -m_Size, m_Size,
		-m_Size, m_Size, m_Size,
		m_Size, m_Size, m_Size,
		m_Size, m_Size, m_Size,
		m_Size, -m_Size, m_Size,
		-m_Size, -m_Size, m_Size,

		-m_Size, m_Size, -m_Size,
		m_Size, m_Size, -m_Size,
		m_Size, m_Size, m_Size,
		m_Size, m_Size, m_Size,
		-m_Size, m_Size, m_Size,
		-m_Size, m_Size, -m_Size,

		-m_Size, -m_Size, -m_Size,
		-m_Size, -m_Size, m_Size,
		m_Size, -m_Size, -m_Size,
		m_Size, -m_Size, -m_Size,
		-m_Size, -m_Size, m_Size,
		m_Size, -m_Size, m_Size };

	std::cout << "Generating Skybox Vertex Array" << std::endl;
	glGenVertexArrays(1, &VertexArrayId);
	std::cout << "Binding Skybox Vertex Array" << std::endl;
	glBindVertexArray(VertexArrayId);
	std::cout << "Generating Skybox Vertex Buffer" << std::endl;
	glGenBuffers(1, &VertexBufferId);
	std::cout << "Binding Skybox Vertex Buffer" << std::endl;
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferId);
	std::cout << "Sending Skybox data to Vertex Buffer" << std::endl;
	glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof(GLint), &skyboxVerts, GL_STATIC_DRAW);
	std::cout << "Setting Skybox Vertex Attributes" << std::endl;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	std::cout << "Generating Skybox Texture Buffer" << std::endl;
	glGenTextures(1, &TextureBufferId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, TextureBufferId);
	AddTexture(backFileName, SRN_SKYBOX_RIGHT);
	AddTexture(frontFileName, SRN_SKYBOX_LEFT);
	AddTexture(topFileName, SRN_SKYBOX_TOP);
	AddTexture(bottomFileName, SRN_SKYBOX_BOTTOM);
	AddTexture(rightFileName, SRN_SKYBOX_BACK);
	AddTexture(leftFileName, SRN_SKYBOX_FRONT);
	glBindTexture(GL_TEXTURE_CUBE_MAP, TextureBufferId);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
}

Skybox::~Skybox()
{

}

void Skybox::AddTexture(const char* fileName, GLuint side)
{
	std::cout << "Loading skybox texture: " << fileName << std::endl;

	glBindTexture(GL_TEXTURE_CUBE_MAP, TextureBufferId);

	int x, y, n;
	unsigned char* data = stbi_load(fileName, &x, &y, &n, 4);

	if (data == NULL)
	{
		const char* error = stbi_failure_reason();
		std::cout << "Unable to load skybox texture: " << error << std::endl;
	}
	else
	{

		if (n == 3)
			std::cout << "Not enough channels in skybox texture!" << std::endl;
		else if (n == 4)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + side, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			std::cout << "Loaded skybox texture: " << fileName << std::endl;
		}
	}

	stbi_image_free(data);
}

void Skybox::SetShaders(const char* strVertexFile, const char* strFragmentFile)
{
	shader.Initialize(strVertexFile, strFragmentFile);
}

void Siren::Skybox::Draw()
{
	glDepthMask(GL_FALSE);

	shader.TurnOn();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, TextureBufferId);
	glUniform1i(glGetUniformLocation(shader.getShaderProgramId(), "skybox"), 0);

	mat4 projectionMatrix = camera->GetProjectionMatrix();
	mat4 viewM = camera->GetViewMatrix();
	mat4 viewMatrix = mat4(mat3(viewM));

	GLint viewMatrixId = shader.GetVariable("viewMatrix");
	GLint projectionMatrixId = shader.GetVariable("projectionMatrix");

	shader.SetMatrix4(viewMatrixId, 1, GL_FALSE, &viewMatrix[0][0]);
	shader.SetMatrix4(projectionMatrixId, 1, GL_FALSE, &projectionMatrix[0][0]);

	glBindVertexArray(VertexArrayId);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDisableVertexAttribArray(0);
	glBindVertexArray(0);

	shader.TurnOff();

	glDepthMask(GL_TRUE);
}
