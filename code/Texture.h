#pragma once
#include <string>
namespace WSYEngine {
	class Texture
	{
		unsigned int m_textureID;
		void setupTexture(const std::string &texturePath, bool isHDR);
	public:
		Texture(std::string &texturePath, bool isHDR = false);
		~Texture();
		int getTextureID() const;
	};


}
