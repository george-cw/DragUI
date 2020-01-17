#include "StdAfx.h"
#include "IniManager.h"
#include "Function.h"
#include <shlwapi.h>

#define KEY_VAL_LEN	2048

///////////////////////////////////////////////////////////////////////////////
// Ini File
CIniManager::CIniManager()
{
	memset(m_szHelp, 0, sizeof(m_szHelp));
	memset(m_szIniFile, 0, sizeof(m_szIniFile));
	memset(m_szLanguage, 0, sizeof(m_szLanguage));
	memset(m_szModulePath, 0, sizeof(m_szModulePath));

	InitPath(NULL);	
}

CIniManager::~CIniManager()
{
}

void	CIniManager::InitPath(HINSTANCE hInstance)
{
	m_hInstance = hInstance;

	memset(m_szModulePath, 0, sizeof(m_szModulePath));
	DWORD dwLength = ::GetModuleFileName(hInstance, m_szModulePath, sizeof(m_szModulePath));
	if (dwLength == 0)
		return;

	for (int dw = dwLength - 1; dw > 0; dw--)
	{
		if (m_szModulePath[dw] == _T('\\'))
		{
			m_szModulePath[dw+1] = _T('\0');
			break;
		}	
	}	
}

// 指定工作目录
void	CIniManager::SetWorkPath(LPCTSTR lpszPath)
{
	if (PathFileExists(lpszPath))
	{
		_tcscpy(m_szModulePath, lpszPath);
	}
}

// 初始化
BOOL CIniManager::Initialize(HINSTANCE hInstance, LPCTSTR lpszIniFile)
{
	if (hInstance)
		InitPath(hInstance);

	if (_tcslen(lpszIniFile) < 5)
		return FALSE;

	_stprintf(m_szIniFile, _T("%s%s"), m_szModulePath, lpszIniFile);
	if (!PathFileExists(m_szIniFile))
	{
		memset(m_szIniFile, 0, sizeof(m_szIniFile));
		return FALSE;
	}
	
	// 获取系统当前语言
	LANGID  LangID  = GetUserDefaultUILanguage/*GetSystemDefaultLangID*/();
	TCHAR	szLang[8] = { 0 };
	if(LangID == 0x804) 
		_tcscpy(szLang, _T("804"));
	else if(LangID == 0x404 || LangID == 0xC04 || LangID == 0x1004) 
		_tcscpy(szLang, _T("404"));
	else
		_stprintf(szLang, _T("4%02x"), LangID & 0xFF);
	
	TCHAR szValue[256] = { 0 };
	GetPrivateProfileString(_T("LanguageMap"), szLang, szValue, szValue, _countof(szValue)-1, m_szIniFile);
	if (szValue[0] == _T('\0')) // 语言配置不存在,获取默认语言配置
	{
		memset(szValue, 0, sizeof(szValue));
		GetPrivateProfileString(_T("LanguageMap"), _T("default"), szValue, szValue, _countof(szValue)-1, m_szIniFile);
	}
	if (szValue[0] == _T('\0'))
	{
		_tcscpy(m_szLanguage,  _T("English"));
	}
	else 
	{
		SpliterString(m_szLanguage, szValue, 0, _T(','));
		SpliterString(m_szHelp, szValue, 1, _T(','));
	}
	
	return TRUE;
}

// 获取UI资源 
LPTSTR	CIniManager::GetUI(LPCTSTR lpszKey, LPTSTR lpszValue)
{
	if (lpszValue == NULL)
		return NULL;

	TCHAR szValue[KEY_VAL_LEN] = { 0 };
	GetPrivateProfileString(m_szLanguage, lpszKey, szValue, szValue, _countof(szValue)-1, m_szIniFile);
	int nLength = _tcslen(szValue);	
	_tcscpy(lpszValue, szValue);
	lpszValue[nLength] = 0;

	return lpszValue;
}

LPTSTR	CIniManager::GetUI(int nIndex, LPTSTR lpszValue)
{
	if (lpszValue == NULL)
		return NULL;

	TCHAR szKey[16] = { 0 };
	Format(szKey, _T("UI%02d"), nIndex);
	
	TCHAR szValue[KEY_VAL_LEN] = { 0 };
	GetPrivateProfileString(m_szLanguage, szKey, szValue, szValue, _countof(szValue)-1, m_szIniFile);	
	int nLength = _tcslen(szValue);
	
	lpszValue[0] = 0;

	LPCTSTR lpszFull = &szValue[0];
	LPCTSTR lpszFind = _tcsstr(lpszFull, _T("\\n"));
	while(lpszFind)
	{
		nIndex = lpszFind - lpszFull;
		_tcsncat(lpszValue, lpszFull, nIndex);
		_tcscat(lpszValue, _T("\n"));
		lpszFull += (nIndex + 2);	

		lpszFind = _tcsstr(lpszFull, _T("\\n"));
		if (lpszFind == NULL)
			_tcscat(lpszValue, lpszFull);
	}
	
	nLength = _tcslen(lpszValue);
	if (nLength == 0)
		_tcscpy(lpszValue, szValue);
	else
		lpszValue[nLength] = 0;

	return lpszValue;
}

// 获取Config
LPTSTR	CIniManager::GetConfig(LPCTSTR lpszKey, LPTSTR lpszValue)
{
	if (lpszValue == NULL)
		return NULL;

	TCHAR szValue[KEY_VAL_LEN] = { 0 };
	GetPrivateProfileString(_T("CONFIG"), lpszKey, szValue, szValue, _countof(szValue)-1, m_szIniFile);	
	int nLength = _tcslen(szValue);	
	_tcscpy(lpszValue, szValue);	
	lpszValue[nLength] = 0;

	return lpszValue;
}

DWORD CIniManager::GetConfigInt(LPCTSTR lpszKey)
{
	TCHAR szValue[32] = { 0 };
	GetPrivateProfileString(_T("CONFIG"), lpszKey, szValue, szValue, _countof(szValue)-1, m_szIniFile);	
	return (DWORD)_ttoi(szValue);
}

DWORD CIniManager::GetConfigInt(LPCTSTR lpszSection, LPCTSTR lpszKey, bool &bFound)
{
	TCHAR szValue[32] = { 0 };
	GetPrivateProfileString(lpszSection, lpszKey, _T("KEYNOTFOUND"), szValue, _countof(szValue)-1, m_szIniFile);	
	if (wcscmp(_T("KEYNOTFOUND"), szValue) == 0)
		bFound = false;
	else
		bFound = true;
	return (DWORD)_ttoi(szValue);
}

BOOL CIniManager::UpdateConfig(LPCTSTR lpszKey, LPCTSTR lpszValue)
{
	return WritePrivateProfileString(_T("CONFIG"), lpszKey, lpszValue, m_szIniFile);
}

LPTSTR	CIniManager::GetConfig(LPCTSTR lpszSection, LPCTSTR lpszKey, LPTSTR lpszValue, bool &bFound)
{
	if (lpszValue == NULL)
		return NULL;

	TCHAR szValue[KEY_VAL_LEN] = { 0 };
	GetPrivateProfileString(lpszSection, lpszKey, _T("KEYNOTFOUND"), szValue, _countof(szValue)-1, m_szIniFile);	
	int nLength = _tcslen(szValue);	
	_tcscpy(lpszValue, szValue);	
	lpszValue[nLength] = 0;
	if (wcscmp(_T("KEYNOTFOUND"), szValue) == 0)
		bFound = false;
	else
		bFound = true;

	return lpszValue;
}

BOOL CIniManager::UpdateConfig(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszValue)
{
	return WritePrivateProfileString(lpszSection, lpszKey, lpszValue, m_szIniFile);
}

// 格式化字符串
LPTSTR CIniManager::Format(LPTSTR szDest, LPCTSTR szFormat,... )
{
	assert( szFormat != NULL );

	va_list argList;
	va_start( argList, szFormat );
	_vstprintf(szDest, szFormat, argList);
	va_end( argList );
	
	return szDest;	
}