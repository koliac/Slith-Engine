#pragma once
#include<glad/glad.h>
#include<string>
#include<unordered_map>
#include <fstream>
#include <sstream>
#include<iostream>
#include<glm/glm.hpp>
namespace WSYEngine {
	class Shader
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
	};

}

