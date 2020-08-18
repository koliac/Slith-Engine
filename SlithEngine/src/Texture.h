#pragma once
#include  "SE_Core.h"
#include <string>
namespace SlithEngine {
	class SLITH_API Texture
	{
		unsigned int m_textureID;
		void setupTexture(const std::string &texturePath, bool isHDR);
	public:
		Texture(std::string &texturePath, bool isHDR = false);
		~Texture();
		int getTextureID() const;
	};


}
