#pragma once
#include <vector>
#include <glad/glad.h> 
#include <glm/glm.hpp>
struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texcoord;
	glm::vec3 tangent;
	glm::vec4 color;
};
class Mesh
{
private:
	GLuint m_VAO, m_VBO, m_EBO;
	std::vector<Vertex> m_vertexBuffer;
	std::vector<GLint> m_triangleBuffer;


	int BindMesh();
public:
	Mesh(const std::vector<Vertex>& vertexBuffer, const std::vector<GLint>& triangleBuffer);
	~Mesh();


};
