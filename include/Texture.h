#ifndef SRN_TEXTURE_H
#define SRN_TEXTURE_H

#include <stb_image.h>
#include <SRN_Platform.h>

#include <gl/glew.h>

namespace Siren {

	enum SRN_API TextureType
	{
		SRN_TEXTURETYPE_NONE,
		SRN_TEXTURETYPE_DIFFUSE,
		SRN_TEXTURETYPE_SPECULAR,
		SRN_TEXTURETYPE_AMBIENT,
		SRN_TEXTURETYPE_EMISSIVE,
		SRN_TEXTURETYPE_HEIGHT,
		SRN_TEXTURETYPE_NORMALS,
		SRN_TEXTURETYPE_SHININESS,
		SRN_TEXTURETYPE_OPACITY,
		SRN_TEXTURETYPE_DISPLACEMENT,
		SRN_TEXTURETYPE_LIGHTMAP,
		SRN_TEXTURETYPE_REFLECTION,
		SRN_TEXTURETYPE_UNKNOWN
	};
	
	class SRN_API Texture
	{
	public:
		Texture(const char* fileName, GLuint type);
		~Texture();

		GLuint GetTextureId() { return m_Id; }
		void SetTextureId(GLuint id) { m_Id = id; }
		GLuint GetTextureType() { return m_Type; }
		void SetTextureType(GLuint type) { m_Type = type; }

	private:
		GLuint m_Id;
		GLuint m_Type;
	};

}

#endif // SRN_TEXTURE_H