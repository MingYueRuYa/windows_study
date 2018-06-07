// 1. 这种方式加载dll需要.lib，.dll，.h文件，可以使用c++语法导出类
//		不能采用LoadLibrary方式导出dll，因为c++中存在name mangling	
#ifdef LOAD_METHOD_DLL_EXPORTS
#define LOAD_METHOD_DLL_API __declspec(dllexport)
#else
#define LOAD_METHOD_DLL_API __declspec(dllimport)
#endif

 // 2. 这种方式以C语言的方式导出，可以采用LoadLibrary方式导出
//#ifdef __cplusplus
//#define LOAD_METHOD_DLL_API extern "C" __declspec(dllexport)
//#else
//#define LOAD_METHOD_DLL_API __declspec(dllimport)
//#endif

// 此类是从 load_method_dll.dll 导出的
class LOAD_METHOD_DLL_API Cload_method_dll {
public:
	Cload_method_dll(void);
	// TODO:  在此添加您的方法。
};

extern LOAD_METHOD_DLL_API int nload_method_dll;

LOAD_METHOD_DLL_API int fnload_method_dll(void);
