#include "se_pch.h"
#include "Application.h"

namespace SlithEngine {


	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}


	Application::~Application()
	{
	}

	void Application::Run()
	{
		std::cout << "Starting the Sandbox" << std::endl;
		while (true) {
			m_Window->OnUpdate();
		}
	}
}
