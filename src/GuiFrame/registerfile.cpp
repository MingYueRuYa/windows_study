#include "registerfile.h"

using XIBAO::RegisterHelper;

RegisterFile::RegisterFile(const wstring &suffix, const wstring &appKey, 
                const wstring &iconPath, 
                const wstring &exePath)
                : mSuffix(suffix),
                  mAppKey(appKey),
                  mIconPath(iconPath),
                  mExePath(exePath)
{
}

RegisterFile::~RegisterFile()
{
}

bool RegisterFile::IsRegister()
{
    bool result = true;
    do {
        RegisterHelper helper(HKEY_CLASSES_ROOT, mSuffix, KEY_QUERY_VALUE);
        DWORD ret_code = helper.IsExsitKey(L"");
        if (NOERROR != ret_code) { 
            result = false;
            break;
        }

        wstring key_value;
        helper.GetSZ(L"", key_value);

        if (key_value.empty()) { 
            result = false;
            break;
        }
    } while(0);

    return result;
}

bool RegisterFile::RegistFile()
{
    RegisterHelper helper(HKEY_CLASSES_ROOT, mSuffix, KEY_READ | KEY_WRITE);
    helper.SetSZ(L"", mAppKey);

    RegisterHelper icon_helper(HKEY_CLASSES_ROOT, mAppKey + wstring(L"\\DefaultIcon"), KEY_READ | KEY_WRITE);
    icon_helper.SetSZ(L"", mIconPath);

    RegisterHelper exe_helper(HKEY_CLASSES_ROOT, mAppKey + wstring(L"\\shell\\open\\command"), KEY_READ | KEY_WRITE);
    exe_helper.SetSZ(L"", mExePath);

    return false;
}
