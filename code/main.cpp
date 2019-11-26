#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Vertex.h"
#include "Mesh.h"
#include "Shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void processMesh(const aiScene *scene, const aiNode *root, std::vector<WSYEngine::Mesh> &meshList);
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//const char *vertexShaderSource = "#version 330 core\n"
//"layout (location = 0) in vec3 aPos;\n"
//"void main()\n"
//"{\n"
//"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//"}\0";
//const char *fragmentShaderSource = "#version 330 core\n"
//"out vec4 FragColor;\n"
//"void main()\n"
//"{\n"
//"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
//"}\n\0";

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}



	// ------------------------Global OpenGL Config-----------------------------------------------------
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);


	// -----------------------Shader Setup-------------------------------------------------------------
	WSYEngine::Shader testShader("Shaders/Phong.vert", "Shaders/Phong.frag");


	// ------------------------------------------------------------------------------------------
	// -----------------------------Mesh Loading test - will abstract away later-----------------

	std::string modelPath = "../models/BlenderMonkey.obj";
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return 1;
	}

	std::vector<WSYEngine::Mesh> meshList;
	processMesh(scene, scene->mRootNode, meshList);
	for (size_t i = 0; i < meshList.size(); i++)
	{
		std::cout << "Mesh ID "<<i<<" is "<<meshList[i].getMeshID()<<std::endl;
	}
	
	//WSYEngine::Mesh blenderMonkey(vertexlist, triangleList);
	// ----------------------------Matrix Setup--------------------------------------------------
	 // projection matrix
	testShader.bind();
	testShader.setMat4("model", glm::mat4(1.0f));
	glm::mat4 projection = glm::perspective(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	testShader.setMat4("projection", projection);
	testShader.unbind();
	// ------------------------------------------------------------------------------------------

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// draw our first triangle
		//glUseProgram(shaderProgram);
		testShader.bind();
		// view matrix
		glm::mat4 view = glm::lookAt(glm::vec3(sin(glfwGetTime()) * 10, 0.0f, cos(glfwGetTime()) * 10),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));
		testShader.setMat4("view", view);
		for (size_t i = 0; i < meshList.size(); i++)
		{
			glBindVertexArray(meshList[i].getMeshID());
			glDrawElements(GL_TRIANGLES, meshList[i].getNumberOfTriangles(), GL_UNSIGNED_INT, 0);
		}

	
		//these two calls were used to render the quad for testing purpose
		//glBindVertexArray(VAO); // 
		//glBindVertexArray(blenderMonkey.getMeshID());
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		//glDrawElements(GL_TRIANGLES, blenderMonkey.getNumberOfTriangles(), GL_UNSIGNED_INT, 0);
		// glBindVertexArray(0); // no need to unbind it every time 

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void processMesh(const aiScene *scene, const aiNode *root, std::vector<WSYEngine::Mesh> &meshList) {

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
		meshList.push_back(WSYEngine::Mesh(vList, triList));
		
	}
	for (size_t n = 0; n < root->mNumChildren; n++)
	{
		processMesh(scene, root->mChildren[n], meshList);
	}
}