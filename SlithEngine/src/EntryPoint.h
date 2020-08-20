#pragma once

#ifdef SE_PLATFORM_WINDOWS

extern SlithEngine::Application* SlithEngine::CreateApplication();




int main(int argc, char** argv) {
	SlithEngine::Log::Init();
	SE_CORE_WARN("Initializing Log!");
	SE_CORE_INFO("Welcome to Slith Engine");
	auto app = SlithEngine::CreateApplication();
	app->Run();
	delete app;
}
#endif