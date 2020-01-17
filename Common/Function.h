/*****************************************************************

** �ļ���: Function.h

** ������: caihaiqun  

** ��  ��: 2010-08-26    

** ��  ��: ��������ʵ��

** ��  ��: 1.0 

** ��  ��: 
       
******************************************************************/
#pragma once
#include <shlobj.h>
#include <algorithm>

// �������
#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif

//==================================================================
// �ļ����
BOOL	IsFileExistA(LPCSTR lpszFileName);
BOOL	IsFileExistW(LPCWSTR lpszFileName);

BOOL	IsFileExistExA(LPCSTR lpszFileName);
BOOL	IsFileExistExW(LPCWSTR lpszFileName);

ULONG64	GetFileSizeA(LPCSTR lpszFileName);
ULONG64	GetFileSizeW(LPCWSTR lpszFileName);

//==================================================================
// �ַ��������������ַ�����ת�����ƻ�ת
// ASCII�ַ�ת��Ϊ8���������ַ�
LPSTR	ctob(char* szDest, char c);

// ASCII���ַ���ת���ɶ������ַ���
LPSTR	atob(char* szDest, const char *szSource, int nLen);

// �������ַ���ת��ΪASCII�ַ���
LPSTR	btoa(char* szDest, const char *szSource, int nLen);

// �������ַ���ת��������ֵ
UINT	btoi(const char *szBin, int nLen);

// �������ַ���ת����ʮ��������ʽ
LPSTR	btoh(char* szHex, const char *szBin, int nLen);

// ASCII�ַ���ת��Ϊ16������ʽ
LPSTR	atoh(char* szHex, const char* szAcsii, int nLen, bool bSpace = true);

// 16������ʽת��ΪASCII�ַ���
int		htoa(char* szAscii, const char* szHex);

// �ַ���ת�������� HEX������0x��ͷ
ULONG64 stoi(LPCSTR str);
ULONG64 stoi(LPCWSTR str);

// base64�����
char *	base64_encode(const unsigned char * data, char* base64, int length);
int		base64_decode(const char* base64, unsigned char* data);

LPTSTR	Format(LPTSTR szDest, LPCTSTR szFormat,...);
LPTSTR	FormatError(DWORD dwErrorCode, LPTSTR lpszErrorInfo);

int		GetSubStrPos( const char* strSource, int nLen, const char* strSub, int nSubLen );

//int	SpliterString(TCHAR* szSub, LPCTSTR lpszFullString, int iSubString, TCHAR chSep);
int		SpliterStringW(WCHAR* szSub, LPCWSTR lpszFullString, int iSubString, WCHAR chSep);
int		SpliterStringA(CHAR* szSub, LPCSTR lpszFullString, int iSubString, CHAR chSep);

//////////////////////////////////////////////////////////////////////////
/// �ַ�����ת��

// Multi-Byte to UNICODE
int		MultiByte2Unicode(CONST CHAR* szSource, LPWSTR wszDest, int nEncoding = CP_ACP);	
// UNICODE to Multi-Byte
int		Unicode2MultiByte(LPCWSTR wszSource, CHAR* szDest, int nEncoding = CP_ACP);
void UNICODE2UTF8(LPCWSTR  strText, CHAR* szDest);


// KMP�ַ���ƥ�� 
// ���� 0 ����ƥ�䲻�ɹ�
// ���طǸ�ֵ �����������ƥ���λ��
int		IndexKMP(const char* pSrc, int nSrcLen, const char* pSub, int nSubLen);

/// Get version information from DLL
BOOL	GetDllVersion(LPCTSTR lpszDLL, VS_FIXEDFILEINFO* ptrVersionInfo);

///=================================================================
/// Windows Configuration
LPTSTR	GetCpuInfo(LPTSTR szInfo);
LPTSTR	GetOsInfo(LPTSTR szOS);

///=================================================================
/// File & Path

// OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST
LPTSTR	OpenFile(HWND hWnd, LPTSTR lpszTitle, LPCTSTR lpszFilter, LPCTSTR lpszInitDir, TCHAR* szDest, DWORD dwMax);

LPTSTR	GetPath(LPCTSTR lpszFile, TCHAR* szPath);
LPWSTR	GetFile(LPCWSTR lpszCurrentPath, LPCWSTR lpszInputFile, WCHAR* szDest);
LPSTR	GetFile(LPCSTR lpszCurrentPath, LPCSTR lpszInputFile, CHAR* szDest);
LPTSTR	GetRelativePath(LPCTSTR lpszAbsolutePath1, LPCTSTR lpszAbsolutePath2, LPTSTR szRelative);
LPTSTR	BrowsePath(TCHAR* szDest, HWND hOwer, LPCTSTR lpszTitle, BFFCALLBACK pfnCallBack, LPARAM lParam);
enum OpenType{save_f,open_f};
CStdString ShowFileBrowse(HWND hWnd,CStdString initPath,CStdString title,OpenType open_type,CStdString suffix,TCHAR * fileter);

void	RemoveFile(LPCTSTR lpszPath, LPCTSTR lpszExt, BOOL bRecurse);

///��ָ��ģ���ж�ȡָ�����͵���Դ���� 
///��Ҫ���ε��ã���һ�ε���@ptrBuffer == NULL,�������ݴ�С
///�ڶ��δ������õ�@ptrBuffer���������Դ����
UINT	GetResource(LPCTSTR lpszModule, LPCTSTR lpszResourceName, LPCTSTR lpszResourceType, LPBYTE ptrBuffer);
BOOL	SaveResource(LPCTSTR lpszModule, LPCTSTR lpszResourceName, LPCTSTR lpszResourceType, LPCTSTR lpszDestFile);

//////////////////////////////////////////////////////////////////////////
/// Create application data path
BOOL	GetSystemPathA(CHAR* szAppPath, UINT csidl, LPCSTR lpszCreateSubFolder);
BOOL	GetSystemPathW(WCHAR* szAppPath, UINT csidl, LPCWSTR lpszCreateSubFolder);

//==================================================================
/// Debug print
VOID	DbgPrintA(const CHAR* ptrFormat, ...);
VOID	DbgPrintW(const WCHAR* ptrFormat, ...);

//////////////////////////////////////////////////////////////////////////

string RemovePreAndLastSpaceA(const string& str);
wstring RemovePreAndLastSpaceW(const wstring& str);

DWORD64 GetDiskSpaceInfo(LPCTSTR pszDrive);

#ifdef _UNICODE
	#define IsFileExist		IsFileExistW
	#define IsFileExistEx	IsFileExistExW
	#define SpliterString	SpliterStringW
	#define GetSystemPath	GetSystemPathW
	#define DbgPrint		DbgPrintW
	#define RemovePreAndLastSpace RemovePreAndLastSpaceW
#else
	#define IsFileExist		IsFileExistA
	#define IsFileExistEx	IsFileExistExA
	#define SpliterString	SpliterStringA
	#define GetSystemPath	GetSystemPathA
	#define DbgPrint		DbgPrintA	
	#define RemovePreAndLastSpace RemovePreAndLastSpaceA
#endif


void Trim(CStdString & s) ;
void Trim(CStdString & s ,  TCHAR* pTrim) ;
int	CallExe(LPCWSTR lptszExeName , LPCWSTR lptszParam ,   LPCWSTR lpwszCurdir  , bool bAsync , bool bEnableForceTerminate);

inline string& ltrim(string &str) {
    string::iterator p = find_if(str.begin(), str.end(), not1(ptr_fun<int, int>(isspace)));
    str.erase(str.begin(), p);
    return str;
}
 
inline string& rtrim(string &str) {
    string::reverse_iterator p = find_if(str.rbegin(), str.rend(), not1(ptr_fun<int , int>(isspace)));
    str.erase(p.base(), str.end());
    return str;
}
 
inline string& trim(string &str) {
    ltrim(rtrim(str));
    return str;
}

inline int isDigit(string str){
	if (str.length() == 0)
		return 0;
	for(int i=0; i < str.length(); i++)
	{
		if (isdigit(str[i]) == 0)
			return 0;
	}
	return 1;
}