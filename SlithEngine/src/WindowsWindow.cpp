#include "se_pch.h"
#include "WindowsWindow.h"
namespace SlithEngine {
	static bool isGLFWInittialized = false;
	Window* Window::Create(const WindowProps& props) {
		return new WindowsWindow(props);
	}
	WindowsWindow::WindowsWindow(const WindowProps & props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::OnUpdate()
	{
		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		m_Data.vSync = enabled;
		if (m_Data.vSync) {
			glfwSwapInterval(1);
		}
		else {
			glfwSwapInterval(0);
		}
	}

	void WindowsWindow::SetEvnentCallback(const EventCallbackFn & callback)
	{
	}

	void WindowsWindow::Init(const WindowProps & props)
	{
		m_Data.title = props.title;
		m_Data.width = props.width;
		m_Data.height = props.height;

		if (!isGLFWInittialized) {

			int status = glfwInit();
			SE_ASSERT(status, "Failed to initialize GLFW");
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			isGLFWInittialized = true;
		}

		m_Window = glfwCreateWindow(m_Data.width, m_Data.height, m_Data.title.c_str(), nullptr, nullptr);
		SE_ASSERT(m_Window != nullptr, "Failed to create GLFW window");
		SetVSync(true);
	}

	void WindowsWindow::Shutdown()
	{
		glfwTerminate();
	}
}