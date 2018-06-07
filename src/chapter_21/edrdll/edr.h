/****************************************************************************
**
** Copyright (C) 2018 liushixiong@2144.cn
** All rights reserved.
**
****************************************************************************/

#include <windows.h>

#ifdef __cplusplus
	#define EXPORT extern "C" __declspec (dllexport)
#else
	#define EXPORT __declspec (dllexport)
#endif // __cplusplus

EXPORT BOOL CALLBACK EdrCenterTextA(HDC hdc, PRECT pre, PCSTR pString);
EXPORT BOOL CALLBACK EdrCenterTextW(HDC hdc, PRECT pre, PCWSTR pString);

#ifdef UNICODE
	#define EdrCenterText EdrCenterTextW;
#else
	#define EdrCenterText EdrCenterTextA;
#endif //UNICODE