#pragma once
#include "GLFunctions.h"
#include "assets/Assets.h"


class Texture {
public:
	Texture() {}
	~Texture()
	{
		if (m_textureID != 0) {
			glDeleteTextures(1, &m_textureID);
		}
	}

	void Init(const TextureData& textureData)
	{
		m_width = textureData.width;
		m_height = textureData.height;
		glCreateTextures(GL_TEXTURE_2D, 1, &m_textureID);
		glTextureStorage2D(m_textureID, m_mipLevels, GL_RGBA8, m_width, m_height);
		glTextureSubImage2D(m_textureID, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, textureData.buffer.data());

		glTextureParameteri(m_textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTextureParameteri(m_textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glGenerateTextureMipmap(m_textureID);
	}

	GLuint GetTextureID() const { return m_textureID; }

protected:
	GLuint m_textureID = 0;
	GLsizei m_mipLevels = 4;
	unsigned int m_width = 0;
	unsigned int m_height = 0;

};
