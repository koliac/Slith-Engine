#pragma once
#include <string>
namespace SlithEngine {
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
