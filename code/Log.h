#pragma once
#include  "SlithCore.h"
//#ifndef FMT_DEPRECATED
//	#define FMT_DEPRECATED
//#endif
#include "spdlog/spdlog.h"
//#include "spdlog/sinks/stdout_color_sinks.h"
#include <memory>
namespace SlithEngine {
	class SLITH_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return m_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return m_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> m_CoreLogger;
		static std::shared_ptr<spdlog::logger> m_ClientLogger;
	};
}

// Core Logger
#define SE_CORE_TRACE(...) ::SlithEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SE_CORE_INFO(...) ::SlithEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SE_CORE_WARN(...) ::SlithEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SE_CORE_ERROR(...) ::SlithEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SE_CORE_FATAL(...) ::SlithEngine::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client Logger
#define SE_TRACE(...) ::SlithEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SE_INFO(...) ::SlithEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define SE_WARN(...) ::SlithEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SE_ERROR(...) ::SlithEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define SE_FATAL(...) ::SlithEngine::Log::GetClientLogger()->fatal(__VA_ARGS__)


