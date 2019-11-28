#include "Camera.h"
#include <iostream>
#include "Time.h"
namespace WSYEngine {
	float Camera::SCROLL_OFFSET = 0.0f;
	bool Camera::NEED_UPDATE = false;
	void Camera::camera_mouse_callback(GLFWwindow * window, double xpos, double ypos)
	{
	}
	void Camera::camera_scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
	{
		Camera::SCROLL_OFFSET = yoffset;
		Camera::NEED_UPDATE = true;
	}
	void Camera::camera_framebuffer_size_callback(GLFWwindow * window, int width, int height)
	{
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
	void Camera::callbackSetup()
	{
		glfwSetScrollCallback(m_Contex, camera_scroll_callback);

	}
	void Camera::orbitControl()
	{
		processScroll();
	}
	Camera::Camera(GLFWwindow* contex,
		const glm::vec3 &cameraPosition,
		const glm::vec3 &lookAtPoint ,
		const glm::vec3 &up ,
		float fov, float nearPlane, float farPlane):
		m_Contex(contex),
		m_CameraPosition(cameraPosition),
		m_LookAtPoint(lookAtPoint),
		m_UP(up),
		m_FOV(fov),
		m_NearPlane(nearPlane),
		m_FarPlane(farPlane)

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
		
		return glm::perspective(glm::radians(m_FOV), 800.0f / 600.0f, m_NearPlane,m_FarPlane);
		
	}
	glm::mat4 Camera::getViewMatrix() const
	{
		return glm::mat4();
	}
}