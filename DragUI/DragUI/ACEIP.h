/////////////////////////////////////////////////////////////////////
//
// Actions Customer Experience Improvement Program(ACEIP)
//
/////////////////////////////////////////////////////////////////////
#ifndef __ACEIP_APP_INCLUDE__
#define __ACEIP_APP_INCLUDE__
#pragma once

typedef BOOL	(WINAPI*ACEIP_Init)(LPCSTR lpszProduct, LPCSTR lpszLogMail, LPCSTR lpszDumpMail);
typedef void	(WINAPI*ACEIP_Final)();

/// @lpszControlClass: duilib control class name, not be NULL or empty
/// @lpszEventList: duilib event name£¬use '|' to seperate multiple event
typedef void	(WINAPI*ACEIP_LogFilter)(LPCTSTR lpszControlClass, LPCTSTR lpszEventList);
typedef void	(WINAPI*ACEIP_LogEvent)(LPCTSTR lpszDialog, LPCTSTR lpszClass, 
										LPCTSTR lpszControl, LPCTSTR lpszEvent, LPCTSTR lpszValue);

/// Dump file for WinDbg.exe !Analyze -v Command
typedef int		(WINAPI*ACEIP_Dump)(int dwExceptionCode, void* pExceptInfo, LPCTSTR lpszExtraInfo);
typedef int		(WINAPI*ACEIP_Stack)(int dwExceptionCode, void* pExceptInfo, LPCTSTR lpszExtraInfo);

//////////////////////////////////////////////////////////////////////////
namespace DuiLib
{
	class CControlUI;
}
using namespace DuiLib;

//=============================================================
class  CAppACEIP
{
public:	
	CAppACEIP();
	virtual ~CAppACEIP();

public:

	BOOL	Init();
	void	Final();

	void	LogEvent(LPCTSTR lpszDialog, CControlUI* pControl, LPCTSTR lpszEvent);

	int		MiniDump(int dwExceptionCode, void* pExceptInfo, LPCTSTR lpszExtraInfo = NULL);
	int		ExceptionStack(int dwExceptionCode, void* pExceptInfo, LPCTSTR lpszExtraInfo = NULL);

protected:

	HINSTANCE		m_hACEIP;

	ACEIP_Init		m_pfnInit;
	ACEIP_Final		m_pfnFinal;
	ACEIP_LogFilter	m_pfnFilter;
	ACEIP_LogEvent	m_pfnEvent;

	ACEIP_Dump		m_pfnDump;
	ACEIP_Stack		m_pfnStack;
	
protected:
	
	static	CAppACEIP	s_ceip;
	friend	CAppACEIP*	ACEIPAPP();
};

//////////////////////////////////////////////////////////////////////////

inline	CAppACEIP*	ACEIPAPP()
{
	return &CAppACEIP::s_ceip;
}

/////////////////////////////////////////////////////////////////////
#endif // !__ACEIP_APP_INCLUDE__





















