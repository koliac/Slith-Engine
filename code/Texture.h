#pragma once
#include <string>
namespace WSYEngine {
	class Texture
	{
		unsigned int m_textureID;
		void setupTexture(const std::string &texturePath);
	public:
		Texture(std::string &texturePath);
		~Texture();
		int getTextureID() const;
	};


}
