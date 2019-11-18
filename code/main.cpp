#define TINYOBJLOADER_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "Vertex.h"
#include "Mesh.h"
#include <tiny_obj_loader.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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
	else {
		std::cout << "GLFW window initialization completed successfully" << std::endl;
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
	// -----------------------------------For test only: Importing Models ---------------------------------------
	
	std::string inputfile = "../models/BlenderMonkey.obj";
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;


	std::string err;

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile.c_str());


	if (!err.empty()) {
		std::cerr << err << std::endl;
	}

	if (!ret) {
		exit(1);
	}
	std::vector<WSYEngine::Vertex> vertexList;
	std::vector<GLuint> triangleList;
	std::cout << shapes.size();
	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];
			glm::vec3 pos, normal, tangent;
			glm::vec2 texcoord;
			glm::vec4 color;
			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
				tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
				tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
				tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
				tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
				tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];
				tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index + 0];
				tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index + 1];
				// Optional: vertex colors
				// tinyobj::real_t red = attrib.colors[3*idx.vertex_index+0];
				// tinyobj::real_t green = attrib.colors[3*idx.vertex_index+1];
				// tinyobj::real_t blue = attrib.colors[3*idx.vertex_index+2];

				pos = { vx,vy,vz };
				normal = { nx,ny,nz };
				tangent = { 0.0f,0.0f,0.0f };
				texcoord = { tx,ty };
				color = { 1.0f,1.0f,1.0f,1.0f };
				vertexList.push_back(WSYEngine::Vertex(
					pos,normal,texcoord,
					tangent,color
				));
				triangleList.push_back(index_offset+v);
				
			}
			index_offset += fv;

			// per-face material
			shapes[s].mesh.material_ids[f];
		}
	}
	WSYEngine::Mesh monkeyMesh(vertexList, triangleList);

	std::vector<WSYEngine::Vertex> triVertexList;
	std::vector<GLuint> triFaceIndex = { 0,2,1 };
	glm::vec3 v1{ 0.0,10.0,0.0 };
	glm::vec3 v2{ -5.0,0.0,0.0 };
	glm::vec3 v3{ 5.0,0.0,0.0 };

	triVertexList.push_back(WSYEngine::Vertex(
		v1,glm::vec3(0), glm::vec2(0),
		glm::vec3(0), glm::vec4(0)
	));
	triVertexList.push_back(WSYEngine::Vertex(
		v2, glm::vec3(0), glm::vec2(0),
		glm::vec3(0), glm::vec4(0)
	));
	triVertexList.push_back(WSYEngine::Vertex(
		v3, glm::vec3(0), glm::vec2(0),
		glm::vec3(0), glm::vec4(0)
	));

	WSYEngine::Mesh triMesh(triVertexList, triFaceIndex);



	// ------------------------------------Test Shader & Camera Setup ----------------------------------
	WSYEngine::Shader testShader("Shaders/Phong.vert", "Shaders/Phong.frag");
	testShader.bind();
	
	// view
	glm::mat4 view = glm::lookAt(
		glm::vec3(0.0f,0.0f,10.0f), 
		glm::vec3(0.0f,0.0f,0.0f), 
		glm::vec3(0.0f,1.0f,0.0f));
	testShader.setMat4("view", view);
	// projection
	glm::mat4 projection = glm::perspective(65.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	testShader.setMat4("projection", projection);
	testShader.unbind();


	// ----------------------------------------------------------------------------------------

	// Open GL global configuration
	glEnable(GL_DEPTH_TEST | GL_DEPTH_BUFFER_BIT);

	// render loop
	// -----------
	std::cout << "Rendering: " << triMesh.getMeshID()<<std::endl;
	glBindVertexArray(triMesh.getMeshID());
	testShader.bind();
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		
		glDrawElements(GL_TRIANGLES, triMesh.getNumberOfTriangles(), GL_UNSIGNED_INT, 0);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glBindVertexArray(0);
	testShader.unbind();

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