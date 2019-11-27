#include "Texture.h"
#include <stb_image.h>
#include <glad/glad.h>
#include<iostream>
namespace WSYEngine {
	
	Texture::Texture(std::string &texturePath)
	{
		setupTexture(texturePath);
	}
	Texture::~Texture()
	{
		if (m_textureID != -1) {
			glDeleteTextures(1, &m_textureID);
		}
	}
	int Texture::getTextureID() const
	{
		return m_textureID;
	}
	void Texture::setupTexture(const std::string &texturePath )
	{
		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, nrChannels;
		unsigned char *data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			std::cout << "Load texture " << m_textureID << " succeeded." << std::endl;
		}
		else
		{
			std::cout << "Failed to load texture "<<m_textureID << std::endl;
			glDeleteTextures(1, &m_textureID);
			m_textureID = -1;
		}
		stbi_image_free(data);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}