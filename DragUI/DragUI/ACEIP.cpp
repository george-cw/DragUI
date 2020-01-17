/////////////////////////////////////////////////////////////////////
//
// Actions Customer Experience Improvement Program(ACEIP)
//
/////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "ACEIP.h"

#if defined(_DEBUG)
#define new   new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define _CRTDBG_MAP_ALLOC
#endif

#pragma warning(disable:4996)

CAppACEIP	CAppACEIP::s_ceip;
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

CAppACEIP::CAppACEIP()
: m_hACEIP(NULL)
, m_pfnInit(NULL)
, m_pfnFinal(NULL)
, m_pfnFilter(NULL)
, m_pfnEvent(NULL)
, m_pfnDump(NULL)
, m_pfnStack(NULL)
{
}

CAppACEIP::~CAppACEIP()
{
	if (m_hACEIP)
	{
		if (m_pfnFinal)
			m_pfnFinal();

		m_pfnFinal = NULL;

		FreeLibrary(m_hACEIP);
		m_hACEIP = NULL;
	}
}

BOOL	CAppACEIP::Init()
{
	if (m_hACEIP == NULL)
	{
		m_hACEIP = ::LoadLibrary(_T("ACEIP.dll"));
		if (m_hACEIP)
		{
			m_pfnInit	= (ACEIP_Init)GetProcAddress(m_hACEIP, "ACEIP_Init");
			m_pfnFinal	= (ACEIP_Final)GetProcAddress(m_hACEIP, "ACEIP_Final");
			m_pfnFilter = (ACEIP_LogFilter)GetProcAddress(m_hACEIP, "ACEIP_LogFilter");
			m_pfnEvent	= (ACEIP_LogEvent)GetProcAddress(m_hACEIP, "ACEIP_LogEvent");

			m_pfnDump	= (ACEIP_Dump)GetProcAddress(m_hACEIP, "ACEIP_Dump");
			m_pfnStack	= (ACEIP_Stack)GetProcAddress(m_hACEIP, "ACEIP_Stack");
		}
	}

	if (m_pfnInit == NULL || m_pfnFinal == NULL || m_pfnFilter == NULL || m_pfnEvent == NULL)
		return FALSE;

	BOOL bInit = FALSE;
	if (IniManager()->GetConfigInt(_T("ACEIP")) != 1)
		bInit = m_pfnInit("PreData", NULL, "caihaiqun@actions-semi.com");
	else
		bInit = m_pfnInit("PreData", "whql@actions-semi.com", "caihaiqun@actions-semi.com");

	if (bInit)
	{
		m_pfnFilter(_T("ButtonUI"),		_T("Click"));
		m_pfnFilter(_T("OptionUI"),		_T("SelectChanged"));
		m_pfnFilter(_T("ListUI"),		_T("ItemSelect"));
		m_pfnFilter(_T("EditUI"),		_T("KillFocus"));
		m_pfnFilter(_T("ComboUI"),		_T("ItemSelect"));
	}

	return bInit;
}

void	CAppACEIP::Final()
{
	if (m_pfnFinal)
		m_pfnFinal();
}

int		CAppACEIP::MiniDump(int dwExceptionCode, void* pExceptInfo, LPCTSTR lpszExtraInfo)
{
	if (m_pfnDump)
		return m_pfnDump(dwExceptionCode, pExceptInfo, lpszExtraInfo);
	return -1;
}

int		CAppACEIP::ExceptionStack(int dwExceptionCode, void* pExceptInfo, LPCTSTR lpszExtraInfo)
{
	if (m_pfnStack)
		return m_pfnStack(dwExceptionCode, pExceptInfo, lpszExtraInfo);
	return -1;
}

void	CAppACEIP::LogEvent(LPCTSTR lpszDialog, CControlUI* pControl, LPCTSTR lpszEvent)
{
	if (IniManager()->GetConfigInt(_T("ACEIP")) != 1)
		return ;

	if (m_pfnEvent)
	{
		LPCTSTR lpszClass = pControl->GetClass();
		if (_tcsicmp(lpszClass, _T("OptionUI")) == 0)
		{
			m_pfnEvent(lpszDialog, lpszClass, pControl->GetName(), lpszEvent, 
				((COptionUI*)pControl)->IsSelected()? _T("1") : _T("0"));
		}
		else if (_tcsicmp(lpszClass, _T("EditUI")) == 0)
			m_pfnEvent(lpszDialog, lpszClass, pControl->GetName(), lpszEvent, pControl->GetText());

		else if (_tcsicmp(lpszClass, _T("ComboUI")) == 0)
			m_pfnEvent(lpszDialog, lpszClass, pControl->GetName(), lpszEvent, pControl->GetText());

		else
			m_pfnEvent(lpszDialog, lpszClass, pControl->GetName(), lpszEvent, _T(""));		
	}
}