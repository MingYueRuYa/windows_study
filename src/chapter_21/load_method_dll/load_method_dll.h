// 1. ���ַ�ʽ����dll��Ҫ.lib��.dll��.h�ļ�������ʹ��c++�﷨������
//		���ܲ���LoadLibrary��ʽ����dll����Ϊc++�д���name mangling	
#ifdef LOAD_METHOD_DLL_EXPORTS
#define LOAD_METHOD_DLL_API __declspec(dllexport)
#else
#define LOAD_METHOD_DLL_API __declspec(dllimport)
#endif

 // 2. ���ַ�ʽ��C���Եķ�ʽ���������Բ���LoadLibrary��ʽ����
//#ifdef __cplusplus
//#define LOAD_METHOD_DLL_API extern "C" __declspec(dllexport)
//#else
//#define LOAD_METHOD_DLL_API __declspec(dllimport)
//#endif

// �����Ǵ� load_method_dll.dll ������
class LOAD_METHOD_DLL_API Cload_method_dll {
public:
	Cload_method_dll(void);
	// TODO:  �ڴ�������ķ�����
};

extern LOAD_METHOD_DLL_API int nload_method_dll;

LOAD_METHOD_DLL_API int fnload_method_dll(void);
