#include "Shader.h"

namespace WSYEngine {
	Shader::Shader(const std::string &vertShaderPath, const std::string &fragShaderPath) :
		m_PropertyStringMap(std::unordered_map<std::string, GLint>())
	{
		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::string geometryCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		std::ifstream gShaderFile;
		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			vShaderFile.open(vertShaderPath);
			fShaderFile.open(fragShaderPath);
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();


		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char * fShaderCode = fragmentCode.c_str();
		// 2. compile shaders
		unsigned int vertex, fragment;
		GLint success;
		GLchar infoLog[1024];

		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << "VERTEX SHADER" << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}

		// fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << "FRAGMENT SHADER" << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}

		// shader Program
		m_ID = glCreateProgram();
		glAttachShader(m_ID, vertex);
		glAttachShader(m_ID, fragment);
		glLinkProgram(m_ID);
		glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(m_ID, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << "SHADER PROGRAM LINKING" << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
		// delete the shaders as they're linked into our program now and no longer necessery
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}


	Shader::~Shader()
	{
		glDeleteProgram(m_ID);
	}

	void Shader::bind()
	{
		glUseProgram(m_ID);
	}

	void Shader::unbind()
	{
		glUseProgram(0);
	}

	GLuint Shader::getCachedPropertyHash(const std::string &name)
	{
		if (m_PropertyStringMap.find(name) == m_PropertyStringMap.end()) {
			m_PropertyStringMap[name] = glGetUniformLocation(m_ID, name.c_str());
		}
		return m_PropertyStringMap[name];
	}

	void Shader::setBool(const std::string &name, GLboolean value)
	{
		glUniform1i(getCachedPropertyHash(name), (GLint)value);
	}



	void Shader::setInt(const std::string & name, GLint value)
	{
		glUniform1i(getCachedPropertyHash(name), value);
	}

	void Shader::setFloat(const std::string & name, GLfloat value)
	{
		glUniform1f(getCachedPropertyHash(name), value);
	}
	void Shader::setMat4(const std::string & name, const glm::mat4 & mat)
	{
		glUniformMatrix4fv(getCachedPropertyHash(name), 1, GL_FALSE, &(mat[0][0]));
	}
}

