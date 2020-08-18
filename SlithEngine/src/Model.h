#pragma once
#include  "SE_Core.h"
#include <string>
#include <vector>
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
namespace SlithEngine {
	class SLITH_API Model
	{
	private:
		std::vector<Mesh*> m_MeshList;
		void processMesh(const aiScene *scene, const aiNode *root, std::vector<SlithEngine::Mesh*> &meshList);
	public:
		std::vector<Mesh*> getMeshList() const;
		Model(std::string modelPathString);
		~Model();
	};

}