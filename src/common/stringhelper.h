#ifndef stringhelper_h
#define stringhelper_h

#include <string>
#include <windows.h>

using std::string;
using std::wstring;

/****************************************************************************
**
** Copyright (C) 2018 635672377@qq.com
** All rights reserved.
**
****************************************************************************/

/*! \brief common::stringhelper
 	\author liushixiong (liushixiongcpp@163.cn)
 	\version 0.01
 	\date 2018-11-29 10:58:13
*/
namespace XIBAO {

class StringHelper
{
public:	
	StringHelper() = default;
	~StringHelper() = default;
	StringHelper(const StringHelper &) = default;

	static string FormatString(LPCSTR format, ...);
	static wstring FormatString(LPCTSTR format, ...);

};

};

#endif // debughelper_h