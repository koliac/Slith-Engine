#pragma once
namespace SlithEngine {
#ifdef SE_PLATFORM_WINDOWS
	#ifdef SE_BUILD_DLL
		#define SLITH_API __declspec(dllexport)
	#else
		#define SLITH_API __declspec(dllimport)
	#endif 
#else
	#error This is windows only
#endif
#ifdef HZ_ENABLE_ASSERTS
	#define SE_ASSERT(x, ...) { if(!(x)) { SE_ERROR("Assertion Failed: {0}", __VA_ARGS__); HZ_DEBUGBREAK(); } }
	#define SE_CORE_ASSERT(x, ...) { if(!(x)) { SE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); HZ_DEBUGBREAK(); } }
#else
	#define SE_ASSERT(x, ...)
	#define HSE_CORE_ASSERT(x, ...)
#endif


#define BIT(x) (1 << x)






}