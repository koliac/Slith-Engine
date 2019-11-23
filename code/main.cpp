#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <string>

#include "Vertex.h"
#include "Mesh.h"
#include "Shader.h"


#include <tiny_obj_loader.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

unsigned int loadCubemap(std::vector<std::string> faces);

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


	// -----------------------Shader Setup-------------------------------------------------------------
	WSYEngine::Shader testShader("Shaders/Phong.vert", "Shaders/Phong.frag");
	WSYEngine::Shader skyboxShader("Shaders/skybox.vert", "Shaders/skybox.frag");

	// --------------------------- Skybox--------------------------------------------------------
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};
	// skybox VAO
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	std::vector<std::string> faces =
	{
		"resources/textures/skybox/DaylightBox/DaylightBox_Right.bmp",
		"resources/textures/skybox/DaylightBox/DaylightBox_Left.bmp",
		"resources/textures/skybox/DaylightBox/DaylightBox_Top.bmp",
		"resources/textures/skybox/DaylightBox/DaylightBox_Bottom.bmp",
		"resources/textures/skybox/DaylightBox/DaylightBox_Front.bmp",
		"resources/textures/skybox/DaylightBox/DaylightBox_Back.bmp"
	};

	unsigned int cubemapTexture = loadCubemap(faces);

	skyboxShader.setInt("skybox", 0);

	// ------------------------------------------------------------------------------------------
	// -----------------------------Mesh Loading test - will abstract away later-----------------

	std::string inputfile = "../Models/BlenderMonkey.obj";
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
	std::vector<GLuint> triangleList;
	std::vector<WSYEngine::Vertex> vertexlist;
	size_t index_offset = 0;
	for (size_t f = 0; f < shapes[0].mesh.num_face_vertices.size(); f++) {
		int fv = shapes[0].mesh.num_face_vertices[f];

		// Loop over vertices in the face.
		for (size_t v = 0; v < fv; v++) {
			// access to vertex
			tinyobj::index_t idx = shapes[0].mesh.indices[index_offset + v];
			tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
			tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
			tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
			tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
			tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
			tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];
			tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index + 0];
			tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index + 1];

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
		shapes[0].mesh.material_ids[f];
	}
	WSYEngine::Mesh blenderMonkey(vertexlist, triangleList);
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

		//these two calls were used to render the quad for testing purpose
		//glBindVertexArray(VAO); // 
		glBindVertexArray(blenderMonkey.getMeshID());
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, blenderMonkey.getNumberOfTriangles(), GL_UNSIGNED_INT, 0);
		// glBindVertexArray(0); // no need to unbind it every time 

		// draw skybox as last
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.bind();
		//view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);
		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default

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



unsigned int loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}