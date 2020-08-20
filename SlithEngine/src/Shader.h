#pragma once
#include  "SE_Core.h"
#include<glad/glad.h>
#include<glm/glm.hpp>
namespace SlithEngine {
	class SLITH_API Shader
	{
	private:
		GLuint m_ID;
		std::unordered_map<std::string, GLint> m_PropertyStringMap;
		GLuint getCachedPropertyHash(const std::string &name);

		
	public:
		Shader(const std::string &vertShaderPath, const std::string &fragShaderPath);
		~Shader();
		void bind();
		void unbind();
		void setBool(const std::string &name, GLboolean value);
		void setInt(const std::string &name, GLint value);
		void setFloat(const std::string &name, GLfloat value);
		void setMat4(const std::string &name, const glm::mat4 &mat);
		void setVec3(const std::string & name, const glm::vec3 & vec3);
	};

}

