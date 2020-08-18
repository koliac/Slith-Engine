#pragma once
#include "SE_Core.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace SlithEngine {
	class SLITH_API Camera
	{
	private:
		// default settings
		GLFWwindow* m_Contex;
		glm::vec3 m_CameraPosition;
		glm::vec3 m_LookAtPoint;
		glm::vec3 m_UP;
		float m_FOV;
		float m_NearPlane;
		float m_FarPlane;
		float m_width;
		float m_height;

		float m_LastXPos;
		float m_LastYPos;
		float m_yaw;
		float m_pitch;

		static unsigned int COLOR_BUFFER_TEX;
		static unsigned int FINAL_FRAME_BUFFER_OBJ;

		static float SCROLL_OFFSET;
		static float WIDTH;
		static float HEIGHT;
		static float MOUSE_XPOS;
		static float MOUSE_YPOS;
		static bool NEED_UPDATE;


		static void camera_mouse_callback(GLFWwindow* window, double xpos, double ypos);
		static void camera_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		static void camera_framebuffer_size_callback(GLFWwindow* window, int width, int height);
		void processKeybordInput();
		void processScroll();
		void processMouseInput();


		void callbackSetup();
		
	public:
		Camera(GLFWwindow* contex, 
			const glm::vec3 &cameraPosition = glm::vec3(0.0f,0.0f,5.0f),
			const glm::vec3 &lookAtPoint = glm::vec3(0.0f),
			const glm::vec3 &up = glm::vec3(0.0f,1.0f,0.0f),
			float fov = 65.0f,
			float nearPlane = 0.1f,
			float farPlane = 500.0f);
		~Camera();
		void printTime() const;
		glm::mat4 getPerspectiveMatrix() const;
		glm::mat4 getViewMatrix() const;
		void orbitControl();
		void setColorBufferTexture(int tex);
		void setFinalFrameBufferObject(int fbo);
		unsigned int getColorBufferTexture();

		glm::vec3 getPosition() const;
	};
}

