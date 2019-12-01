#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <string>

#include "Vertex.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Model.h"
#include "Time.h"
#include "Camera.h"

#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void genQuad(unsigned int* quadVAO, unsigned int* quadVBO );

unsigned int loadCubemap(std::vector<std::string> faces);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float deltaTime = 0.0f;
float lastFrameTime = 0.0f;

unsigned int quadVAO=0, quadVBO=0;

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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}



	// ------------------------Global OpenGL Config-----------------------------------------------------
	glEnable(GL_DEPTH_TEST);

	// -----------------------Camera Setup ------------------------------------------------------------
	WSYEngine::Camera cam(window);
	// -----------------------Shader Setup-------------------------------------------------------------
	WSYEngine::Shader phongShader("Shaders/Phong.vert", "Shaders/Phong.frag");
	WSYEngine::Shader skyboxShader("Shaders/skybox.vert", "Shaders/skybox.frag");
	WSYEngine::Shader pbrShader("Shaders/PBR.vert", "Shaders/PBR.frag");
	WSYEngine::Shader toneMappingShader("Shaders/HDRToneMapping.vert", "Shaders/HDRToneMapping.frag");

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
		"../textures/skybox/DaylightBox/DaylightBox_Right.bmp",
		"../textures/skybox/DaylightBox/DaylightBox_Left.bmp",
		"../textures/skybox/DaylightBox/DaylightBox_Top.bmp",
		"../textures/skybox/DaylightBox/DaylightBox_Bottom.bmp",
		"../textures/skybox/DaylightBox/DaylightBox_Front.bmp",
		"../textures/skybox/DaylightBox/DaylightBox_Back.bmp"
	};

	unsigned int cubemapTexture = loadCubemap(faces);

	skyboxShader.setInt("skybox", 0);

	// -----------------------------------------Full Screen Render Quad----------------------------------

	genQuad(&quadVAO, &quadVBO);
	toneMappingShader.bind();
	toneMappingShader.setInt("hdrBuffer", 0);
	toneMappingShader.unbind();

	// -----------------------------Mesh Loading test - will abstract away later-----------------
	std::string inputfile = "../Models/drakefire_pistol_low.obj";
	WSYEngine::Model testMesh(inputfile);

	std::string diffuseTexture = "../textures/base_albedo.jpg";
	std::string normalTexture = "../textures/base_normal.jpg";
	std::string roughnessTexture = "../textures/base_roughness.jpg";
	std::string aoTexture = "../textures/base_AO.jpg";
	std::string metallicTexture = "../textures/base_metallic.jpg";
	WSYEngine::Texture testTexture(diffuseTexture);
	WSYEngine::Texture testTexture2(normalTexture);
	WSYEngine::Texture testTexture3(roughnessTexture);
	WSYEngine::Texture testTexture4(aoTexture);
	WSYEngine::Texture testTexture5(metallicTexture);



	
  // ------------configure render buffer------------------------
	unsigned int hdrFBO;
	glGenFramebuffers(1, &hdrFBO);
	// create floating point color buffer
	unsigned int colorBuffer;
	glGenTextures(1, &colorBuffer);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// create depth buffer (renderbuffer)
	unsigned int rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
	// attach buffers
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// ----------------------------Light ----------------------------------------------------------
	// (no light directions were specified, set it to whatever you like)
	// vec3[4]
	glm::vec3 lightPositions[4] = {
		{0, 10, 0},
		{-5, 0, 0},
		{5, 5, 0},
		{0, -5, 0}
	};
	glm::vec3 lightColors[4] = {
		{50.0f,40.0f,40.0f},
		{40.0f,50.0f,600.0f},
		{200.0f,50.0f,50.0f},
		{50.0f,50.0f,50.0f}
	};
	//float lightDirections[12] = {};

	// ----------------------------Shader setting--------------------------------------------------   

	phongShader.bind();

	phongShader.setInt("diffuse", 0);
	phongShader.setInt("normal", 1);
	phongShader.setInt("roughness", 2);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model,90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	phongShader.setMat4("model", model);
	
	phongShader.unbind();

	pbrShader.bind();

	pbrShader.setMat4("model", model);

	pbrShader.setInt("albedoMap", 0);
	pbrShader.setInt("normalMap", 1);
	pbrShader.setInt("roughnessMap", 2);
	pbrShader.setInt("aoMap", 3);
	pbrShader.setInt("metallicMap", 4);

	pbrShader.setVec3("lightPositions[0]", lightPositions[0]);
	pbrShader.setVec3("lightPositions[1]", lightPositions[1]);
	pbrShader.setVec3("lightPositions[2]", lightPositions[2]);
	pbrShader.setVec3("lightPositions[3]", lightPositions[3]);

	pbrShader.setVec3("lightColors[0]", lightColors[0]);
	pbrShader.setVec3("lightColors[1]", lightColors[1]);
	pbrShader.setVec3("lightColors[2]", lightColors[2]);
	pbrShader.setVec3("lightColors[3]", lightColors[3]);



	pbrShader.unbind();
	
	// ------------------------------------------------------------------------------------------
	std::vector<WSYEngine::Mesh*> meshes = testMesh.getMeshList();
	// render loop
	// -----------
	
	while (!glfwWindowShouldClose(window))
	{
		// main-loop time logic
		WSYEngine::Time::time = glfwGetTime();
		WSYEngine::Time::deltaTime = WSYEngine::Time::time - lastFrameTime;
		lastFrameTime = WSYEngine::Time::time;


		// input
		// -----
		processInput(window);
		cam.orbitControl();
		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		// main pass
		glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// ----------------------------Matrix Setup--------------------------------------------------   
		glm::mat4 view = cam.getViewMatrix();
		glm::mat4 skyBoxView = glm::mat4(glm::mat3(view));
		glm::mat4 projection = cam.getPerspectiveMatrix();
		// draw our first triangle
		//glUseProgram(shaderProgram);
		
		// ----------------Phong 
		//testShader.bind();
		//testShader.setMat4("projection", projection);
		//testShader.setMat4("view", view);

		// ----------------PBR
		pbrShader.bind();
		pbrShader.setMat4("projection", projection);
		pbrShader.setMat4("view", view);
		pbrShader.setVec3("camPos", cam.getPosition());

		// -------------draw the mesh
		for (unsigned m = 0; m < meshes.size(); m++)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, testTexture.getTextureID()); // diffuse
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, testTexture2.getTextureID()); // normal
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, testTexture3.getTextureID()); // roughness
			
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, testTexture4.getTextureID()); // ao
			glActiveTexture(GL_TEXTURE4);
			glBindTexture(GL_TEXTURE_2D, testTexture5.getTextureID()); // metallic
			

			glBindVertexArray(meshes[m]->getMeshID());
			glDrawElements(GL_TRIANGLES, meshes[m]->getNumberOfTriangles(), GL_UNSIGNED_INT, 0);
		}
		pbrShader.unbind();
		glBindVertexArray(0);
		// --------------draw skybox as last
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.bind();
		skyboxShader.setMat4("view", skyBoxView);
		skyboxShader.setMat4("projection", projection);
		
		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default
		skyboxShader.unbind();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//----------------------------------------- Full Screen Quad Render
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		toneMappingShader.bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,colorBuffer);
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
		toneMappingShader.unbind();
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteBuffers(1, &quadVBO);
	glDeleteVertexArrays(1, &quadVAO);

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

void genQuad(unsigned int* quadVAO, unsigned int* quadVBO) {

	float quadVertices[] = {
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};
	// setup plane VAO
	glGenVertexArrays(1, quadVAO);
	glGenBuffers(1, quadVBO);
	glBindVertexArray(*quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, *quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}