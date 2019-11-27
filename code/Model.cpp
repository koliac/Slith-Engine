#include "Model.h"
#include <tiny_obj_loader.h>
#include <iostream>

namespace WSYEngine {

	Model::Model(std::string modelPathString)
	{
	
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::string err;

		bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, modelPathString.c_str());

		if (!err.empty()) {
			std::cerr << err << std::endl;
		}

		if (!ret) {
			std::cout << "Fail to load mesh: " << modelPathString << std::endl;
		}
		m_MeshList = std::vector<Mesh*>();
		// Loop over shapes
		for (size_t s = 0; s < shapes.size(); s++) {
			std::vector<GLuint> triangleList;
			std::vector<WSYEngine::Vertex> vertexlist;
			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
				int fv = shapes[s].mesh.num_face_vertices[f];

				// Loop over vertices in the face.
				for (size_t v = 0; v < fv; v++) {
					// access to vertex
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					tinyobj::real_t vx, vy, vz, nx, ny, nz, tx, ty;
					if (attrib.vertices.size()>0) {
						vx = attrib.vertices[3 * idx.vertex_index + 0];
						vy = attrib.vertices[3 * idx.vertex_index + 1];
						vz = attrib.vertices[3 * idx.vertex_index + 2];
					}
					else {
						vx = vy = vz = 0.0f;
					}
					if (attrib.normals.size()>0) {
						nx = attrib.normals[3 * idx.normal_index + 0];
						ny = attrib.normals[3 * idx.normal_index + 1];
						nz = attrib.normals[3 * idx.normal_index + 2];
					}
					else {
						nx = ny = nz = 0.0f;
					}
					if (attrib.texcoords.size()>0) {
						tx = attrib.texcoords[2 * idx.texcoord_index + 0];
						ty = attrib.texcoords[2 * idx.texcoord_index + 1];
					}
					else {
						tx = ty = 0.0f;
					}
					vertexlist.push_back(WSYEngine::Vertex(
						glm::vec3(vx, vy, vz),
						glm::vec3(nx, ny, nz),
						glm::vec2(tx, ty),
						glm::vec3(0.0f),
						glm::uvec4(1.0f)
					));
					triangleList.push_back(index_offset + v);


				}
				index_offset += fv;

				// per-face material
				shapes[s].mesh.material_ids[f];
			}
			WSYEngine::Mesh* m = new WSYEngine::Mesh(vertexlist, triangleList);
			m_MeshList.push_back(m);
		}
	}

	std::vector<Mesh*> Model::getMeshList() const
	{
		return m_MeshList;
	}
	Model::~Model()
	{
		if (m_MeshList.size()!=0) {
			for (auto m : m_MeshList) {
				delete m;
			}
		}

		m_MeshList.clear();
	}
}