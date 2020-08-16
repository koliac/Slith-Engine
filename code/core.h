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
}