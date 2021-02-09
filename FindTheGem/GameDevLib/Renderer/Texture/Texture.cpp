#include "Texture.h"

namespace Rendering
{
	Texture::Texture()
		: m_textureID(0), m_textureWidth(1), m_textureHeight(1)
	{
		m_topLeftPosition = { 0.f, 1.f };
		m_bottomRightPosition = { 1.f, 0.f };
	}

	void Texture::UseSection(float x, float y, float width, float height)
	{
		float left = x / m_textureWidth;
		float right = (x + width) / m_textureWidth;
		float top = 1 - (y / m_textureHeight);
		float bottom = 1 - ((y + height) / m_textureHeight);

		m_topLeftPosition = { left, top };
		m_bottomRightPosition = { right, bottom };
	}

	void Texture::LoadTexture(const char* path, bool smooth)
	{
        unsigned char* image;
        int BPP;
        image = stbi_load(path, &m_textureWidth, &m_textureHeight, &BPP, 4);
        if (!image)
            std::cout << "ERROR loading image" << std::endl;
        else
            std::cout << "Success loading image" << std::endl;

        glGenTextures(1, &m_textureID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (smooth) ? GL_LINEAR : GL_NEAREST);//GL_NEAREST::GL_LINEAR
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (smooth) ? GL_LINEAR : GL_NEAREST);//GL_NEAREST::GL_LINEAR
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindTexture(GL_TEXTURE_2D, m_textureID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_textureWidth, m_textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

		delete image;
	}

	void Texture::LoadTexture()
	{
		uint32_t pixel = 0xffffffff;

		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &pixel);
	}

	Texture::~Texture()
	{
		//
	}
}