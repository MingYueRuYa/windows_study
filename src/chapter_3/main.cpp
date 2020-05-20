
// #include "BrowserPassword.hh"


#include <windows.h>
#include <conio.h>
#include <commctrl.h>

#ifdef TINYFILE
#include "tinyfile.h"
#endif

// for GCC 3.2 (old headers)
#ifndef LVM_SETEXTENDEDLISTVIEWSTYLE
#define LVM_SETEXTENDEDLISTVIEWSTYLE (LVM_FIRST + 54)
#endif
#ifndef LVM_GETEXTENDEDLISTVIEWSTYLE
#define LVM_GETEXTENDEDLISTVIEWSTYLE (LVM_FIRST + 55)
#endif
#ifndef LVS_EX_SNAPTOGRID
#define LVS_EX_SNAPTOGRID 0x00080000
#endif

static TCHAR Desktop[] = TEXT("Desktop");

void SaveLoadDesktopItemsPosition(TCHAR *loadfile) {
	TCHAR filename[MAX_PATH], buf[MAX_PATH];
	SYSTEMTIME t;
	LV_ITEM lvi;
	POINT pt;
	HWND lv;
	DWORD dw;
	BYTE *p;
	HANDLE h, m;
	int i, cnt, l;
  // first of all find desktop window
  lv = FindWindow(TEXT("WorkerW"), NULL);
  lv = (HWND)0x00081E38;
  // Progman -> SHELLDLL_DefView -> SysListView32
  for (i = 0; i < 2; i++) {
    if (lv) {
      lv = FindWindowEx(lv, 0, NULL, NULL);
    }
  }
  // found desktop ListView
  if (lv) {
    dw = 0;
    GetWindowThreadProcessId(lv, &dw);
    // process id found
    if (dw) {
      h = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, dw);
      // can open process
      if (h) {
        // sizeof LV_ITEM
        dw = sizeof(lvi);
        SetLastError(0);
        // allocate memory in the process
        l = dw + (MAX_PATH * sizeof(TCHAR));
        p = (BYTE *)VirtualAllocEx(h, NULL, l, MEM_COMMIT, PAGE_READWRITE);
        // PATCH: Windows 98
        m = 0;
        if ((!p) && (GetLastError() == ERROR_CALL_NOT_IMPLEMENTED)) {
          /*
            https://groups.google.com/d/topic/microsoft.public.win32.programmer.kernel/y1QT3OTeMIs

            If I recall correctly, memory mapped files are visible to all processes on 9x.
            If my recollection is correct you might be able to take advantage of that.

            Another "feature" of Win9x is that you can VirtualAlloc memory above the 2G mark
            in one process and it will be visible at the same address in all processes.
            ---
            http://www.osronline.com/showThread.cfm?link=15846

            Use CreateFileMapping() / MapViewOfFile() - it will allocate memory in the
            range 0x80000000 - 0xB0000000, which is shared between all of the applications
            (use PageReserve/PageCommit to do this in the driver).
          */
          m = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE | SEC_COMMIT, 0, l, NULL);
          // mapping ok?
          if (m) {
            p = (BYTE *) MapViewOfFile(m, FILE_MAP_WRITE, 0, 0, 0);
            // view bad?
            if (!p) {
              CloseHandle(m);
            }
          }
        }
        // memory allocated
        if (p) {
          // init internal structs on process memory
          ZeroMemory(&lvi, dw);
          lvi.cchTextMax = MAX_PATH;
          lvi.pszText = (TCHAR *) &p[dw];
          WriteProcessMemory(h, p, &lvi, dw, NULL);
          if (loadfile) {
            // if file to load specified
            GetFullPathName(loadfile, MAX_PATH, filename, NULL);
            // turn off auto arrange
            dw = GetWindowLong(lv, GWL_STYLE);
            if ((dw & LVS_AUTOARRANGE) == LVS_AUTOARRANGE) {
              if (!m) {
                SetWindowLong(lv, GWL_STYLE, dw ^ LVS_AUTOARRANGE);
              } else {
                /*
                  PATCH: Windows 98 don't allow to change window attributes directly,
                  but this can be emulated by using certain command id from popup menu;
                  also note that this message must be sent to the parent window
                */
                SendMessage(GetParent(lv), WM_COMMAND, 0x7041, 0);
              }
              dw = 1;
            } else {
              dw = 0;
            }
            // turn off snap to grid
            l = SendMessage(lv, LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
            if (l & LVS_EX_SNAPTOGRID) {
              SendMessage(lv, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_SNAPTOGRID, 0);
              dw |= 2;
            }
          } else {
            // file to load not specified - save to file
            l = 0;
            // find executable path
            GetModuleFileName(NULL, filename, MAX_PATH);
            for (i = 0; filename[i]; i++) {
              if (filename[i] == TEXT('\\')) {
                l = i + 1;
              }
            }
            // add ini filename
            lstrcpyn(&filename[l], TEXT("0.ini"), MAX_PATH - i);
            // normalizes day of week
            // before: Sunday[0], <...>, Saturday[6]
            // after: Monday[1], <...>, Sunday[7]
            GetLocalTime(&t);
            // add day of week
            filename[l] += (((t.wDayOfWeek + 6) % 7) + 1);
            // clear previous file
            WritePrivateProfileSection(Desktop, TEXT(""), filename);
          }
          // get total items count
          cnt = SendMessage(lv, LVM_GETITEMCOUNT, 0, 0);
          for (i = 0; i < cnt; i++) {
            // get Desktop item
            l = SendMessage(lv, LVM_GETITEMTEXT, i, (LPARAM) p);
			DWORD errorcode = GetLastError();
            // everything ok?
            if (l > 0) {
              // read item name
              ReadProcessMemory(h, lvi.pszText, buf, l + 1, NULL);
              // escape '=' character because of ini file separator
              for (l = 0; buf[l]; l++) {
                if (buf[l] == TEXT('=')) {
                  // '|' invalid in file name so it's
                  // safe to use it as replacement
                  buf[l] = TEXT('|');
                }
              }
              if (loadfile) {
                // load file
                if (GetPrivateProfileStruct(Desktop, buf, &pt, sizeof(pt), filename)) {
                  // item found - restore position
                  SendMessage(lv, LVM_SETITEMPOSITION, i, MAKELONG(pt.x, pt.y));
                }
              } else {
                // save file
                SendMessage(lv, LVM_GETITEMPOSITION, i, (LPARAM) lvi.pszText);
                ReadProcessMemory(h, lvi.pszText, &pt, sizeof(pt), NULL);
                // save item position
                WritePrivateProfileStruct(Desktop, buf, &pt, sizeof(pt), filename);
              }
            }
          }
          // restore styles
          if (loadfile) {
            if (dw & 1) {
              if (!m) {
                SetWindowLong(lv, GWL_STYLE, GetWindowLong(lv, GWL_STYLE) | LVS_AUTOARRANGE);
              } else {
                // PATCH: Windows 98
                SendMessage(GetParent(lv), WM_COMMAND, 0x7041, 0);
              }
            }
            if (dw & 2) {
              SendMessage(lv, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_SNAPTOGRID, LVS_EX_SNAPTOGRID);
            }
          }
          // cleanup
          if (!m) {
            VirtualFreeEx(h, p, 0, MEM_RELEASE);
          } else {
            // PATCH: Windows 98
            UnmapViewOfFile(p);
            CloseHandle(m);
          }
        }
        CloseHandle(h);
      } // OpenProcess done
    } // pid found
  } // window found
}

#include <exception>
	
using std::exception;

    #include <tchar.h>
    #include <Commctrl.h>


    #include <Shlwapi.h>
    #pragma comment(lib,"Shlwapi.lib")


    typedef void (WINAPI *LPFN_PGNSI)(LPSYSTEM_INFO);


    typedef struct tagLVITEM64A
    {
    UINT mask;
    int iItem;
    int iSubItem;
    UINT state;
    UINT stateMask;
    INT64 pszText;
    int cchTextMax;
    int iImage;
    LPARAM lParam;
    #if (_WIN32_IE >= 0x0300)
    int iIndent;
    #endif
    #if (_WIN32_WINNT >= 0x501)
    int iGroupId;
    UINT cColumns; // tile view columns
    PUINT puColumns;
    #endif
    } LVITEM64A, *LPLVITEM64A;

BOOL Is64Bit_OS();
BOOL GetIconRect32(HWND hDeskWnd, char* strIconName, LPRECT lpRect);
BOOL GetIconRect64(HWND hDeskWnd, char* strIconName, LPRECT lpRect);

    //==取桌面图标位置=======================
BOOL GetIconRect(char* strIconName, LPRECT lpRect, HWND &hDeskListView)
{
    HWND hDeskWnd = NULL;//桌面上SysListView32的窗口句柄
    HWND hWnd = ::FindWindow(_T("WorkerW"), NULL);//先当WIN7系统查找
    while (hWnd) {
    HWND hShellView = ::FindWindowEx(hWnd, NULL, _T("SHELLDLL_DefView"), NULL);

    if (hShellView) {
		hDeskWnd = ::FindWindowEx(hShellView, NULL, _T("SysListView32"), NULL);
		break;
		}
		hWnd = ::GetWindow(hWnd, GW_HWNDNEXT);
    }
    if (!hDeskWnd) {//如果没找到,再按XP方式查找
		hWnd = ::FindWindow(_T("Progman"), _T("Program Manager"));
		if (hWnd) {
			hWnd = ::FindWindowEx(hWnd, NULL, _T("SHELLDLL_DefView"), NULL);
			hDeskWnd = ::FindWindowEx(hWnd, NULL, _T("SysListView32"), NULL);
		}
    }
    if (!hDeskWnd) {
		return FALSE;
    }
    hDeskListView = hDeskWnd;
    BOOL bRet = FALSE;
    if (Is64Bit_OS()) {
		bRet = GetIconRect64(hDeskWnd, strIconName, lpRect);
    }
    else {
		bRet = GetIconRect32(hDeskWnd, strIconName, lpRect);
    }
    if (bRet) {
		POINT pt;
		pt.x = lpRect->left;
		pt.y = lpRect->top;
		::ClientToScreen(hDeskWnd, &pt);
		OffsetRect(lpRect, pt.x - lpRect->left, pt.y - lpRect->top);
    }
    return bRet;
}


BOOL GetIconRect32(HWND hDeskWnd, char* strIconName, LPRECT lpRect)
{
    BOOL bRet = FALSE;
    //-----------------------------
    //遍历外部进程所有图标
    DWORD PID = 0;
    GetWindowThreadProcessId(hDeskWnd, &PID);
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, PID);
    if (hProcess != NULL) {
    LVITEMA* pLVITEM = (LVITEMA*)VirtualAllocEx(hProcess, NULL, sizeof(LVITEM), MEM_COMMIT, PAGE_READWRITE);
    char* pszText = (char*)VirtualAllocEx(hProcess, NULL, 512, MEM_COMMIT, PAGE_READWRITE);
    RECT* pItemRc = (RECT*)VirtualAllocEx(hProcess, NULL, sizeof(RECT), MEM_COMMIT, PAGE_READWRITE);
    RECT rc;
    if (pItemRc != NULL && pLVITEM != NULL) {
		LVITEMA LVITEM;
		LVITEM.mask = LVIF_TEXT;
		LVITEM.cchTextMax = 512;
		LVITEM.pszText = pszText;
		char ItemBuf[512];
		int nCount = ::SendMessage(hDeskWnd, LVM_GETITEMCOUNT, 0, 0);
		for (int iItem = 0; iItem < nCount; iItem++) {
			LVITEM.iItem = iItem;
			LVITEM.iSubItem = 0;
			//将设置好的结构插入目标进程
			WriteProcessMemory(hProcess, pLVITEM, &LVITEM, sizeof(LVITEM), NULL);
			//发送LVM_GETITEM消息
			BOOL r = (BOOL)::SendMessage(hDeskWnd, LVM_GETITEMTEXTA, iItem, (LPARAM)pLVITEM);
			//获取pszText
			ReadProcessMemory(hProcess, pszText, ItemBuf, 512, NULL);
			if (StrCmpA(ItemBuf, strIconName) == 0) {
					::SendMessage(hDeskWnd, LVM_GETITEMRECT, iItem, (LPARAM)pItemRc);
					ReadProcessMemory(hProcess, pItemRc, &rc, sizeof(RECT), NULL);
					memcpy(lpRect, &rc, sizeof(RECT));
					bRet = TRUE;
					break;
				}
			}
			VirtualFreeEx(hProcess, pLVITEM, 0, MEM_RELEASE);
			VirtualFreeEx(hProcess, pszText, 0, MEM_RELEASE);
			VirtualFreeEx(hProcess, pItemRc, 0, MEM_RELEASE);//释放内存
		}
		CloseHandle(hProcess);
    }

    return bRet;
}


BOOL GetIconRect64(HWND hDeskWnd, char* strIconName, LPRECT lpRect)
{
    BOOL bRet = FALSE;
    //遍历外部进程所有图标
    DWORD PID = 0;
    GetWindowThreadProcessId(hDeskWnd, &PID);
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, PID);
    if (hProcess != NULL) {
		LVITEM64A* pLVITEM = (LVITEM64A*)VirtualAllocEx(hProcess, NULL, sizeof(LVITEM64A), MEM_COMMIT, PAGE_READWRITE);
		char* pszText = (char*)VirtualAllocEx(hProcess, NULL, 512, MEM_COMMIT, PAGE_READWRITE);
		RECT* pItemRc = (RECT*)VirtualAllocEx(hProcess, NULL, sizeof(RECT), MEM_COMMIT, PAGE_READWRITE);
		RECT rc;
		if (pItemRc != NULL || pLVITEM != NULL) {
		LVITEM64A LVITEM;
		LVITEM.mask = LVIF_TEXT;
		LVITEM.cchTextMax = 512;
		LVITEM.pszText = (INT64)pszText;
		char ItemBuf[512];
		int nCount = ::SendMessage(hDeskWnd, LVM_GETITEMCOUNT, 0, 0);
		for (int iItem = 0; iItem < nCount; iItem++) {
			LVITEM.iItem = iItem;
			LVITEM.iSubItem = 0;
			//将设置好的结构插入目标进程
			WriteProcessMemory(hProcess, pLVITEM, &LVITEM, sizeof(LVITEM), NULL);
			//发送LVM_GETITEM消息
			BOOL r = (BOOL)::SendMessage(hDeskWnd, LVM_GETITEMTEXTA, iItem, (LPARAM)pLVITEM);
			//获取pszText
			ReadProcessMemory(hProcess, pszText, ItemBuf, 512, NULL);
			if (StrCmpA(ItemBuf, strIconName) == 0) {
					::SendMessage(hDeskWnd, LVM_GETITEMRECT, iItem, (LPARAM)pItemRc);
					ReadProcessMemory(hProcess, pItemRc, &rc, sizeof(RECT), NULL);
					memcpy(lpRect, &rc, sizeof(RECT));
					bRet = TRUE;
					break;
				}
			}
			VirtualFreeEx(hProcess, pLVITEM, 0, MEM_RELEASE);
			VirtualFreeEx(hProcess, pszText, 0, MEM_RELEASE);
			VirtualFreeEx(hProcess, pItemRc, 0, MEM_RELEASE);//释放内存
		}
		CloseHandle(hProcess);
    }

    return bRet;
 }


BOOL Is64Bit_OS()
{
	BOOL bRetVal = FALSE;
	SYSTEM_INFO si = { 0 };
	LPFN_PGNSI pGNSI = (LPFN_PGNSI)GetProcAddress(GetModuleHandle(_T("kernel32.dll")), "GetNativeSystemInfo");
	if (pGNSI == NULL) {
		return FALSE;
	}
	pGNSI(&si);
	if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
		si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
	{
		bRetVal = TRUE;
	}
	return bRetVal;
} 

#include <string>
#include <iostream>

using std::wstring;

#pragma warning(disable:4996)

wstring GetOSVersionName()
{
	wstring wstrOSName = _T("unknown OperatingSystem.");
	SYSTEM_INFO info;        //用SYSTEM_INFO结构判断64位AMD处理器   
	GetSystemInfo(&info);    //调用GetSystemInfo函数填充结构 
	OSVERSIONINFOEX version_info;
	ZeroMemory(&version_info, sizeof(OSVERSIONINFOEX));
	version_info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	if (::GetVersionEx(reinterpret_cast<OSVERSIONINFO*>(&version_info)))
	{
		switch (version_info.dwMajorVersion)
		{
		case 4: //
		{
			switch (version_info.dwMinorVersion)
			{
			case 0:
			{
				if (version_info.dwPlatformId == VER_PLATFORM_WIN32_NT)
					wstrOSName = _T("Microsoft Windows NT 4.0"); //1996年7月发布   
				else if (version_info.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
					wstrOSName = _T("Microsoft Windows 95");
			}
			break;
			case 10:
			{
				wstrOSName = _T("Microsoft Windows 98");
			}
			break;
			case 90:
			{
				wstrOSName = _T("Microsoft Windows Me");
			}
			break;
			}
		}
		break;
		case 5:
		{
			switch (version_info.dwMinorVersion)
			{
			case 0:
			{
				wstrOSName = _T("Microsoft Windows 2000");
			}
			break;
			case 1:
			{
				wstrOSName = _T("Microsoft Windows XP");
			}
			break;
			case 2:
			{
				if (version_info.wProductType == VER_NT_WORKSTATION
					&& info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
				{
					wstrOSName = _T("Microsoft Windows XP Professional x64 Edition");
				}
				else if (GetSystemMetrics(SM_SERVERR2) == 0)
					wstrOSName = _T("Microsoft Windows Server 2003");//2003年3月发布   
				else if (GetSystemMetrics(SM_SERVERR2) != 0)
					wstrOSName = _T("Microsoft Windows Server 2003 R2");
			}
			break;
			}
		}
		break;
		case 6:
		{
			switch (version_info.dwMinorVersion)
			{
			case 0:
			{
				if (version_info.wProductType == VER_NT_WORKSTATION)
					wstrOSName = _T("Microsoft Windows Vista");
				else
					wstrOSName = _T("Microsoft Windows Server 2008");//服务器版本   
				break;
			}
			break;
			case 1:
			{
				if (version_info.wProductType == VER_NT_WORKSTATION)
					wstrOSName = _T("Microsoft Windows 7");
				else
					wstrOSName = _T("Microsoft Windows Server 2008 R2");
				break;
			}
			break;
			case 2:
			{
				HMODULE hm;
				DWORD dwMajorVersion = 0, dwMinorVersion = 0, dwOptionalBuild = 0;
				if (hm = LoadLibrary(_T("ntdll.dll")))
				{

					typedef void (WINAPI *getver)(DWORD*, DWORD*, DWORD*);
					getver gv;
					gv = (getver)GetProcAddress(hm, "RtlGetNtVersionNumbers");
					if (gv)
					{
						gv(&dwMajorVersion, &dwMinorVersion, &dwOptionalBuild);
					}

					FreeLibrary(hm);
				}
				if (dwMajorVersion == 10)
				{
					wstrOSName = _T("Microsoft Windows 10");
				}
				else
				{
					if (version_info.wProductType == VER_NT_WORKSTATION)
						wstrOSName = _T("Microsoft Windows 8");
					else
						wstrOSName = _T("Microsoft Windows Server 2012");
				}
			}
			break;
			case 3:
			{
				if (version_info.wProductType == VER_NT_WORKSTATION)
					wstrOSName = _T("Microsoft Windows 8.1");
				else
					wstrOSName = _T("Microsoft Windows Server 2012 R2");
				break;
			}
			break;
			}
		}
		break;
		}
	}
	return wstrOSName;
}

#include <vector>
#include <winerror.h>
#include <Iphlpapi.h>
#include <Shlobj.h>

using std::vector;

void GetCurrentUserShortCut(vector<wstring>& vc_ShorCut)
{
	TCHAR Path[MAX_PATH + 1];
	wstring strDestDir;
	LPITEMIDLIST pidl;
	LPMALLOC pShell;
	if (SUCCEEDED(SHGetMalloc(&pShell)))
	{
		if (SUCCEEDED(SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOPDIRECTORY, &pidl)))
		{
			if (!SHGetPathFromIDList(pidl, Path))
			{
				pShell->Free(pidl);
			}
			pShell->Release();
			// strDestDir = FormatString(_T("%s"), Path);
			// LogOut("C:\\log.txt", WStringToString(strDestDir.c_str()).c_str());
			// GetFolderFileName(strDestDir, vc_ShorCut);
			std::wcout << L"SHGetPathFromIDList  " << Path << std::endl;
		} 
		else {
//			string errorstr = FormatString("%ld", GetLastError());
//			LogOut("C:\\log.txt", (errorstr + " SHGetSpecialFolderLocation get current user shortcut error.").c_str());
			// LogOut("C:\\log.txt", " SHGetSpecialFolderLocation get current user shortcut error.");
			std::cout << "SHGetSpecialFolderLocation  " << GetLastError() << std::endl;
		}
	} else {
		// LogOut("C:\\log.txt", "SHGetMalloc get current user shortcut error.");
		std::cout << "SHGetMalloc(&pShell)" << std::endl;
	}
}

int main(){

	// std::wcout << GetOSVersionName() << std::endl;

	vector<wstring>  vc_str;
	GetCurrentUserShortCut(vc_str);

	system("pause");
	return 0;
}

/*
typedef struct tagLVITEM64
{
    UINT mask;
    int iItem;
    int iSubItem;
    UINT state;
    UINT stateMask;
    _int64 pszText;
    int cchTextMax;
    int iImage;
    _int64 lParam;
#if (_WIN32_IE >= 0x0300)
    int iIndent;
#endif
#if (_WIN32_WINNT >= 0x0501)
    int iGroupId;
    UINT cColumns; // tile view columns
    _int64 puColumns;
#endif
#if _WIN32_WINNT >= 0x0600
    _int64 piColFmt;
    int iGroup; // readonly. only valid for owner data.
#endif
} LVITEM64;
 
void main(){
    HWND hDestTop; 
    hDestTop = ::FindWindow(L"progman", NULL); 
    hDestTop = ::FindWindowEx(hDestTop, 0, L"shelldll_defview", NULL); 
    hDestTop = ::FindWindowEx(hDestTop, 0, L"syslistview32", NULL); 
     
    int count=(int)::SendMessage( hDestTop, LVM_GETITEMCOUNT, 0, 0);
 
    LVITEM64 lvi, *_lvi;
    wchar_t item[512], subitem[512];
    wchar_t *_item, *_subitem;
    unsigned long pid;
    HANDLE process;
     
    GetWindowThreadProcessId( hDestTop, &pid);
    process=OpenProcess(PROCESS_VM_OPERATION|PROCESS_VM_READ|PROCESS_VM_WRITE|PROCESS_QUERY_INFORMATION, FALSE, pid);
     
    _lvi=(LVITEM64*)VirtualAllocEx(process, NULL, sizeof(LVITEM64), MEM_COMMIT, PAGE_READWRITE);
    _item=(wchar_t*)VirtualAllocEx(process, NULL, 512*sizeof(wchar_t), MEM_COMMIT, PAGE_READWRITE);
    _subitem=(wchar_t*)VirtualAllocEx(process, NULL, 512*sizeof(wchar_t), MEM_COMMIT, PAGE_READWRITE);
 
    lvi.cchTextMax=512;

    RECT  rc;
    rc.left = LVIR_ICON;  //这个一定要设定 可以去看MSDN关于LVM_GETITEMRECT的说明
    RECT* _rc =(RECT*)VirtualAllocEx( process, NULL, sizeof(RECT), MEM_COMMIT, PAGE_READWRITE); 
     
    for( int i=0; i< count; i++) {
 
 
        lvi.iSubItem=0;
        lvi.pszText=(_int64)_item;
        WriteProcessMemory(process, _lvi, &lvi, sizeof(LVITEM64), NULL);
        ::SendMessage( hDestTop, LVM_GETITEMTEXT, (WPARAM)i, (LPARAM)_lvi);
         
        lvi.iSubItem=1;
        lvi.pszText=(_int64)_subitem;
        WriteProcessMemory(process, _lvi, &lvi, sizeof(LVITEM64), NULL);
        ::SendMessage( hDestTop, LVM_GETITEMTEXT, (WPARAM)i, (LPARAM)_lvi);
         
        ::WriteProcessMemory( process, _rc, &rc, sizeof(rc), NULL);
        ::SendMessage( hDestTop, LVM_GETITEMRECT, (WPARAM)i, (LPARAM)_rc);
 
        ReadProcessMemory(process, _item, item, 512*sizeof(wchar_t), NULL);
        ReadProcessMemory(process, _subitem, subitem, 512*sizeof(wchar_t), NULL);
 
         
        ReadProcessMemory(process, _rc, &rc, sizeof(rc), NULL);
        _cwprintf(L"%s - %s LF:%d TP:%d RT:%d BT:%d\n", item, subitem, rc.left,rc.top,rc.right,rc.bottom);
 
    }
     
    VirtualFreeEx(process, _lvi, 0, MEM_RELEASE);
    VirtualFreeEx(process, _item, 0, MEM_RELEASE);
    VirtualFreeEx(process, _subitem, 0, MEM_RELEASE);
    VirtualFreeEx(process, _rc, 0, MEM_RELEASE);
 
    CloseHandle( process );
 
}
*/

//int main(int argc, char *argv[]) {
//  // https://msdn.microsoft.com/en-us/library/windows/desktop/ms724832.aspx
//  // current Windows must be older than Windows Vista
////   if ((GetVersion() & 0xFF) < 6) {
//    // SaveLoadDesktopItemsPosition((argc == 2) ? argv[1] : NULL);
////  }
//
//	test_move();
//
//  return(0);
//}

//int			main(int ac, char **av)
//{
//	// ShowWindow(GetConsoleWindow(), SW_HIDE); /* HIDE CONSOLE */
//	BrowserPassword	browser;
//	
//	browser.getBrowserPassword();
//	browser.printPassword();
//	return (0);
//}
