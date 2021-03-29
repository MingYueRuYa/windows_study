/****************************************************************************
**
** Copyright (C) 2020 liushixiongcpp@163.cn
** All rights reserved.
**
****************************************************************************/

#ifndef registerfile_h
#define registerfile_h

#include "../common/registerhelper.h"

#include <string>

using std::wstring;

/*! \brief	    RegisterFile
	\author	    liushixiong (liushixiongcpp@163.cn)
	\version	0.01
	\date		2020-05-06 10:27:29
	TabButton

    注册文件关联
*/
class RegisterFile
{
public:
    RegisterFile(const wstring &suffix, const wstring &appKey, 
                 const wstring &iconPath, const wstring &exePath);
    ~RegisterFile();

    bool IsRegister();
    bool RegistFile();

private:
    wstring mSuffix;
    wstring mAppKey;
    wstring mIconPath;
    wstring mExePath;

};

#endif // registerfile_h