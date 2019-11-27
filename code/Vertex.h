#pragma once
#include<glm/glm.hpp>
namespace WSYEngine{
	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texcoord;
		glm::vec3 tangent;
		glm::vec4 color;

		Vertex(const glm::vec3 &p, const glm::vec3 &n, const glm::vec2 &uv, const glm::vec3 &t, const glm::vec4 &c):
		position(p), normal(n), texcoord(uv), tangent(t), color(c) 
		{
		}
		Vertex() {
		}
	};
}
