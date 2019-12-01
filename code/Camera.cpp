#include "Camera.h"
#include <iostream>
#include "Time.h"
namespace WSYEngine {
	float Camera::SCROLL_OFFSET = 0.0f;
	bool Camera::NEED_UPDATE = false;
	float Camera::WIDTH = 800.0f;
	float Camera::HEIGHT = 600.0f;
	float Camera::MOUSE_XPOS = 0.0f;
	float Camera::MOUSE_YPOS = 0.0f;

	void Camera::camera_mouse_callback(GLFWwindow * window, double xpos, double ypos)
	{
		Camera::MOUSE_XPOS = xpos;
		Camera::MOUSE_YPOS = ypos;


	}
	void Camera::camera_scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
	{
		Camera::SCROLL_OFFSET = yoffset;
		Camera::NEED_UPDATE = true;
	}
	void Camera::camera_framebuffer_size_callback(GLFWwindow * window, int width, int height)
	{
		Camera::WIDTH = width;
		Camera::HEIGHT = height;
		glViewport(0, 0, width, height);
	
	}
	void Camera::processKeybordInput()
	{
	}
	void Camera::processScroll()
	{
		if (Camera::NEED_UPDATE) {
			m_FOV -= Camera::SCROLL_OFFSET;
			if (m_FOV > 100.0f) {
				m_FOV = 100.0f;
			}

			if (m_FOV < 1.0f) {
				m_FOV = 1.0f;
			}
			Camera::NEED_UPDATE = false;
		}
	}
	void Camera::processMouseInput()
	{


			float dx = 0.5f*(Camera::MOUSE_XPOS - m_LastXPos);
			float dy = 0.5f*(Camera::MOUSE_YPOS - m_LastYPos);

			m_LastXPos = Camera::MOUSE_XPOS;
			m_LastYPos = Camera::MOUSE_YPOS;


			m_yaw += dx;
			m_pitch += dy;

			if (m_pitch > 89.0) {
				m_pitch = 89.0;
			}

			if (m_pitch < -89.0) {
				m_pitch = -89.0;
			}
			


	}
	void Camera::callbackSetup()
	{
		glfwSetScrollCallback(m_Contex, camera_scroll_callback);
		glfwSetFramebufferSizeCallback(m_Contex, camera_framebuffer_size_callback);
		glfwSetCursorPosCallback(m_Contex, camera_mouse_callback);

	}
	void Camera::orbitControl()
	{
		processScroll();
		processMouseInput();
	}
	glm::vec3 Camera::getPosition() const
	{
		return m_CameraPosition;
	}

	Camera::Camera(GLFWwindow* contex,
		const glm::vec3 &cameraPosition,
		const glm::vec3 &lookAtPoint,
		const glm::vec3 &up,
		float fov, float nearPlane, float farPlane) :
		m_Contex(contex),
		m_CameraPosition(cameraPosition),
		m_LookAtPoint(lookAtPoint),
		m_UP(up),
		m_FOV(fov),
		m_NearPlane(nearPlane),
		m_FarPlane(farPlane),
		m_LastXPos(0.0f),
		m_LastYPos(0.0f),
		m_yaw(0.0f),
		m_pitch(0.0f)

	{
		callbackSetup();
	}
	Camera::~Camera()
	{
	}

	void Camera::printTime() const {

		std::cout << "Current Time: " << WSYEngine::Time::time << std::endl;
	}
	glm::mat4 Camera::getPerspectiveMatrix() const
	{
		
		return glm::perspective(glm::radians(m_FOV), Camera::WIDTH/Camera::HEIGHT, m_NearPlane,m_FarPlane);
		
	}
	glm::mat4 Camera::getViewMatrix() const
	{
		
		return glm::lookAt(
			glm::vec3(
				glm::cos(glm::radians(m_yaw))*glm::cos(glm::radians(m_pitch))*5.0f, 
				glm::sin(glm::radians(m_pitch))*5.0f,
				glm::sin(glm::radians(m_yaw))*glm::cos(glm::radians(m_pitch))*5.0f),
			glm::vec3(0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));
	}
}