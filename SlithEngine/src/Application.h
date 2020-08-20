#pragma once
#include "SE_Core.h"
#include "Window.h"
namespace SlithEngine {


	class SLITH_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	private:
		std::unique_ptr<Window> m_Window;
	};

	//define in client
	Application* CreateApplication();


}

