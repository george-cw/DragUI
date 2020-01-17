// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <objbase.h>
#include <vector>
#include <string>
#include <map>


// TODO: reference additional headers your program requires here

#pragma warning(disable:4996)

#include "..\\..\\DuiLib\\include\\UIlib.h"
#include "..\\..\\DuiLib\\include\\Utils.h"
namespace DuiLib {
	typedef std::basic_string<TCHAR> CString;	
}

#define DUI_DRAG_CLICK _T("drag_click")
#define DUI_DRAG_MOVE _T("drag_move")
#define DUI_DRAG_RELEASE _T("drag_up")

using namespace DuiLib;
using namespace std;

#include "ACEIP.h"

//////////////////////////////////////////////////////////////////////////
// IniManager
#include "..\\..\\Common\\CJSON.h"
#include "..\\..\\Common\\Function.h"
#include "..\\..\\Common\\Coding.h"
#include "..\\..\\Common\\IniManager.h"

class CIniManagerApp : public CIniManager
{
protected:	
	static CIniManagerApp s_Manager;
	friend CIniManagerApp* IniManager();
};

inline CIniManagerApp* IniManager()
{
	return &CIniManagerApp::s_Manager;
}


//////////////////////////////////////////////////////////////////////////
/// Font manager
BOOL	AddCustomFont(CPaintManagerUI& pm, LPCTSTR lpszFontXmlFile);