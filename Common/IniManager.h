#pragma once

/////////////////////////////////////////////////////////////////////
//// CIniManager
class CIniManager
{
public:
	CIniManager();
	virtual ~CIniManager();

	HINSTANCE	GetInstance()	{ return m_hInstance; }

	// ָ������Ŀ¼
	void	SetWorkPath(LPCTSTR lpszPath);
	
	// ��ʼ��
	BOOL	Initialize(HINSTANCE hInstance, LPCTSTR lpszIniFile);	

	// ��ȡUI
	LPTSTR	GetUI(LPCTSTR lpszKey, LPTSTR lpszValue);	
	LPTSTR	GetUI(int nIndex, LPTSTR lpszValue);	
	
	// Config
	LPTSTR	GetConfig(LPCTSTR lpszKey, LPTSTR lpszValue);
	DWORD	GetConfigInt(LPCTSTR lpszKey);
	BOOL	UpdateConfig(LPCTSTR lpszKey, LPCTSTR lpszValue);

	LPTSTR	GetConfig(LPCTSTR lpszSection, LPCTSTR lpszKey, LPTSTR lpszValue, bool &bFound);
	DWORD	GetConfigInt(LPCTSTR lpszSection, LPCTSTR lpszKey, bool &bFound);
	BOOL	UpdateConfig(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszValue);

	// 
	LPCTSTR	GetModulePath()	{ return m_szModulePath;}
	LPCTSTR	GetIniFile()	{ return m_szIniFile;	}
	LPCTSTR	GetHelpDoc()	{ return m_szHelp;		}
	
	// ��ʽ���ַ���
	LPTSTR Format(LPTSTR szDest, LPCTSTR szFormat,...);
	
protected:
	TCHAR		m_szLanguage[64];
	TCHAR		m_szIniFile[MAX_PATH];
	TCHAR		m_szModulePath[MAX_PATH];
	TCHAR		m_szHelp[MAX_PATH];

	HINSTANCE	m_hInstance;

	void		InitPath(HINSTANCE hInstance);
};