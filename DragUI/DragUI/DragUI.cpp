// DragUI.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "DragUI.h"
#include "WinBase.hpp"
#include "MainFrameUI.h"

#include <atlbase.h>
CComModule _Module;
#include <atlwin.h>

TCHAR	g_szINI[520] = { 0 };
LPCTSTR	g_szApp = _T("c89c86deafffe41dcda8a762f074ab99aa");
HANDLE	g_hApp	= (HANDLE)17815; // 2017-08-15

void	AppDump(HWND hWnd, HINSTANCE hInstRich)
{
	__try
	{	
		// 主窗口创建后设置，为窗口附加一个属性
		::SetProp(hWnd, g_szApp, g_hApp);
		::SetProp(hWnd, IniManager()->GetIniFile(), g_hApp);

		CPaintManagerUI::MessageLoop();			

		_Module.Term();

		::OleUninitialize();
		::CoUninitialize();

		::FreeLibrary(hInstRich);
	}
	__except (ACEIPAPP()->MiniDump(GetExceptionCode(), GetExceptionInformation(), _T("WinMain")))
	{		
	}	
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
    CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());

	HINSTANCE hInstRich = ::LoadLibrary(_T("Riched20.dll"));

	::CoInitialize(NULL);
	::OleInitialize(NULL);

	_Module.Init( 0, hInstance );

	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) )
		return 0;

	CMainFrameUI* pMainFrame = new CMainFrameUI();
	if( pMainFrame == NULL )
		return 0;

	pMainFrame->Create(NULL, _T(""), UI_WNDSTYLE_FRAME, 0/*WS_EX_TOPMOST*/, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	pMainFrame->SetIcon(IDI_DRAGUI);
	
	pMainFrame->CenterWindow();	
	::ShowWindow(*pMainFrame, SW_SHOW);

	AppDump(*pMainFrame, hInstRich);

 	return 0;
}

