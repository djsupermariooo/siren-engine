#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG

#include <iostream>

#include <Texture.h>

using namespace Siren;

Texture::Texture(const char* fileName, GLuint type)
{
	std::cout << "Loading texture: " << fileName << std::endl;

	GLuint num;
	glGenTextures(1, &num);

	int x, y, n;
	unsigned char* data = stbi_load(fileName, &x, &y, &n, 0);

	if (data == NULL)
	{
		const char* error = stbi_failure_reason();
		std::cout << "Unable to load texture: " << error << std::endl;
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, num);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		if (n == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else if (n == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		std::cout << "Loaded texture: " << fileName << std::endl;
	}

	stbi_image_free(data);

	m_Id = num;
	m_Type = type;
}

Texture::~Texture()
{

}