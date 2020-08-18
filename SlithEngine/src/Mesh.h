#pragma once
#include  "SE_Core.h"
#include <vector>
#include <glad/glad.h> 
#include "vertex.h"
namespace SlithEngine {
	class SLITH_API Mesh
	{
	private:
		GLuint m_VAO, m_VBO, m_EBO;
		


		int BindMesh();
	public:
		Mesh(const std::vector<SlithEngine::Vertex>& vertexBuffer, const std::vector<GLuint>& triangleBuffer);
		GLuint getMeshID() const;
		int getNumberOfTriangles() const;
		std::vector<SlithEngine::Vertex> m_vertexBuffer;
		std::vector<GLuint> m_triangleBuffer;
		~Mesh();


	};

}
