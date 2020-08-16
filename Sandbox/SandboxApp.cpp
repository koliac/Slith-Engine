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

int main() {
	SandboxApp* app = new SandboxApp();
	app->Run();
	delete app;
}