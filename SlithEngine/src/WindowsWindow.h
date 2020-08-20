#pragma once
#include "Window.h"
#include "GLFW/glfw3.h"

namespace SlithEngine{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		
		// Inherited via Window
		virtual void OnUpdate() override;
		virtual uint32_t GetWidth() const override { return m_Data.width; };
		virtual uint32_t GetHeight() const override {return m_Data.height;
	};
		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() const override { return m_Data.vSync; };
		virtual void * GetNativeWindow() const override { return m_Window; };
		virtual void SetEvnentCallback(const EventCallbackFn & callback) override;

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		struct WindowData
		{
			std::string title;
			uint32_t width, height;
			bool vSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
		
		
	};
}

