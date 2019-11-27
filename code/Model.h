#pragma once
#include <string>
#include <vector>
#include "Mesh.h"
namespace WSYEngine {
	class Model
	{
	private:
		std::vector<Mesh*> m_MeshList;
	public:
		std::vector<Mesh*> getMeshList() const;
		Model(std::string modelPathString);
		~Model();
	};

}