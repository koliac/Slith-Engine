#include "Model.h"
#include <tiny_obj_loader.h>
#include <iostream>

namespace WSYEngine {
	void Model::processMesh(const aiScene *scene, const aiNode *root, std::vector<WSYEngine::Mesh*> &meshList) {

	for (size_t i = 0; i < root->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[root->mMeshes[i]];
		std::vector<WSYEngine::Vertex> vList;
		std::vector<GLuint> triList;
		for (size_t v = 0; v < mesh->mNumVertices; v++)
		{
			WSYEngine::Vertex vertex;
			glm::vec3 position, normal, tangent;
			glm::vec2 uv;
			glm::vec4 vColor;
			if (mesh->HasPositions()) {
				position = { mesh->mVertices[v].x,  
					mesh->mVertices[v].y, 
					mesh->mVertices[v].z };
			}
			else {
				position = { 0.0f,0.0f,0.0f };
				std::cout << "Mesh does not have a vertex position" << std::endl;
			}
			if (mesh->HasNormals()) {
				normal = {
				mesh->mNormals[v].x,
				mesh->mNormals[v].y,
				mesh->mNormals[v].z
				};
			}
			else {
				normal = { 0.0f,0.0f,0.0f };
				std::cout << "Mesh does not have a vertex normal" << std::endl;
			}
			if (mesh->HasTangentsAndBitangents()) {
				tangent = {
				mesh->mTangents[v].x,
				mesh->mTangents[v].y,
				mesh->mTangents[v].z
				};
			}
			else {
				tangent = { 0.0f,0.0f,0.0f };
				std::cout << "Mesh does not have a vertex tangent" << std::endl;
			}
			if (mesh->HasTextureCoords(0)) {
				uv = {
				mesh->mTextureCoords[0][v].x,
				mesh->mTextureCoords[0][v].y
				};
			}
			else {
				uv = { 0.0f,0.0f };
				std::cout << "Mesh does not have texture coordinate" << std::endl;
			}
			if (mesh->HasVertexColors(0)) {
				vColor = {
				mesh->mColors[0][v].r,
				mesh->mColors[0][v].g,
				mesh->mColors[0][v].b,
				mesh->mColors[0][v].a
				};
			}
			else {
				vColor = { 1.0f,1.0f,1.0f,1.0f };
				//std::cout << "Mesh does not have a vertex color" << std::endl;
			}
			vertex.position = position;
			vertex.normal = normal;
			vertex.texcoord = uv;
			vertex.tangent = tangent;
			vertex.color = vColor;

			vList.push_back(vertex);
			
		}


		for (unsigned int f = 0; f < mesh->mNumFaces; f++)
		{
			aiFace face = mesh->mFaces[f];
			for (unsigned int fIndex = 0; fIndex < face.mNumIndices; fIndex++) {
				triList.push_back(face.mIndices[fIndex]);
			}
				
		}
		std::cout << "Mesh " << meshList.size() << " vertex count: " << vList.size() << std::endl;
		std::cout << "Mesh " << meshList.size() << " triangle count: " << triList.size() << std::endl;
		WSYEngine::Mesh* m = new WSYEngine::Mesh(vList, triList);
		meshList.push_back(m);
		
	}
	for (size_t n = 0; n < root->mNumChildren; n++)
	{
		processMesh(scene, root->mChildren[n], meshList);
	}
}
	Model::Model(std::string modelPathString):m_MeshList(std::vector<Mesh*>())
	{
		Assimp::Importer importer;
		const aiScene *scene = importer.ReadFile(modelPathString, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
			
		}

		processMesh(scene, scene->mRootNode, m_MeshList);
	
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