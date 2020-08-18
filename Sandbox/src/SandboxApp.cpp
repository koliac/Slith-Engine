#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "SlithEngine.h"
class SandboxApp : public SlithEngine::Application
{
public:
	SandboxApp()
	{
	}

	~SandboxApp() {
	}
};

SlithEngine::Application* SlithEngine::CreateApplication()
{
	return new SandboxApp();
}

