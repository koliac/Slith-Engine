#include "Mesh.h"
namespace SlithEngine {
	Mesh::Mesh(const std::vector<SlithEngine::Vertex>& vertexBuffer, const std::vector<GLuint>& triangleBuffer) :
		m_vertexBuffer(vertexBuffer), m_triangleBuffer(triangleBuffer)
	{
		BindMesh();
	}

	GLuint Mesh::getMeshID() const
	{
		return this->m_VAO;
	}

	int Mesh::getNumberOfTriangles() const {
		return this->m_triangleBuffer.size();
	}

	Mesh::~Mesh()
	{
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
		glDeleteBuffers(1, &m_EBO);
	}

	int Mesh::BindMesh()
	{
		using namespace SlithEngine;
		m_VAO = 0, m_VBO = 0, m_EBO = 0;
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_EBO);

		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, m_vertexBuffer.size() * sizeof(Vertex), &m_vertexBuffer[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_triangleBuffer.size() * sizeof(GLuint),
			&m_triangleBuffer[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
		// vertex tangent
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, tangent));
		// vertex color
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));

		glBindVertexArray(0);



		return 0;
	}

}

