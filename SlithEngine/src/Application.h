#pragma once
#include "SE_Core.h"
namespace SlithEngine {


	class SLITH_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	//define in client
	Application* CreateApplication();


}

