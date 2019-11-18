#pragma once
#include <vector>
#include <glad/glad.h> 
#include "vertex.h"
namespace WSYEngine {
	class Mesh
	{
	private:
		GLuint m_VAO, m_VBO, m_EBO;
		


		int BindMesh();
	public:
		Mesh(const std::vector<WSYEngine::Vertex>& vertexBuffer, const std::vector<GLuint>& triangleBuffer);
		GLuint getMeshID() const;
		int getNumberOfTriangles() const;
		std::vector<WSYEngine::Vertex> m_vertexBuffer;
		std::vector<GLuint> m_triangleBuffer;
		~Mesh();


	};

}
