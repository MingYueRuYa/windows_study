#include <windows.h>

#ifdef __cplusplus
	#define EXPORT extern "C" __declspec (dllexport)
#else
	#define EXPORT __declspec (dllexport)
#endif // __cplusplus


EXPORT BOOL CALLBACK showWindow(HINSTANCE hInstance);
