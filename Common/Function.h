/*****************************************************************

** 文件名: Function.h

** 创建人: caihaiqun  

** 日  期: 2010-08-26    

** 描  述: 公共函数实现

** 版  本: 1.0 

** 修  改: 
       
******************************************************************/
#pragma once
#include <shlobj.h>
#include <algorithm>

// 数组计数
#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif

//==================================================================
// 文件检查
BOOL	IsFileExistA(LPCSTR lpszFileName);
BOOL	IsFileExistW(LPCWSTR lpszFileName);

BOOL	IsFileExistExA(LPCSTR lpszFileName);
BOOL	IsFileExistExW(LPCWSTR lpszFileName);

ULONG64	GetFileSizeA(LPCSTR lpszFileName);
ULONG64	GetFileSizeW(LPCWSTR lpszFileName);

//==================================================================
// 字符串处理、数字与字符串互转、进制互转
// ASCII字符转换为8个二进制字符
LPSTR	ctob(char* szDest, char c);

// ASCII码字符串转化成二进制字符串
LPSTR	atob(char* szDest, const char *szSource, int nLen);

// 二进制字符串转换为ASCII字符串
LPSTR	btoa(char* szDest, const char *szSource, int nLen);

// 二进制字符串转换成整型值
UINT	btoi(const char *szBin, int nLen);

// 二进制字符串转换成十六进制形式
LPSTR	btoh(char* szHex, const char *szBin, int nLen);

// ASCII字符串转换为16进制形式
LPSTR	atoh(char* szHex, const char* szAcsii, int nLen, bool bSpace = true);

// 16进制形式转换为ASCII字符串
int		htoa(char* szAscii, const char* szHex);

// 字符串转换成数字 HEX必须以0x开头
ULONG64 stoi(LPCSTR str);
ULONG64 stoi(LPCWSTR str);

// base64编解码
char *	base64_encode(const unsigned char * data, char* base64, int length);
int		base64_decode(const char* base64, unsigned char* data);

LPTSTR	Format(LPTSTR szDest, LPCTSTR szFormat,...);
LPTSTR	FormatError(DWORD dwErrorCode, LPTSTR lpszErrorInfo);

int		GetSubStrPos( const char* strSource, int nLen, const char* strSub, int nSubLen );

//int	SpliterString(TCHAR* szSub, LPCTSTR lpszFullString, int iSubString, TCHAR chSep);
int		SpliterStringW(WCHAR* szSub, LPCWSTR lpszFullString, int iSubString, WCHAR chSep);
int		SpliterStringA(CHAR* szSub, LPCSTR lpszFullString, int iSubString, CHAR chSep);

//////////////////////////////////////////////////////////////////////////
/// 字符编码转换

// Multi-Byte to UNICODE
int		MultiByte2Unicode(CONST CHAR* szSource, LPWSTR wszDest, int nEncoding = CP_ACP);	
// UNICODE to Multi-Byte
int		Unicode2MultiByte(LPCWSTR wszSource, CHAR* szDest, int nEncoding = CP_ACP);
void UNICODE2UTF8(LPCWSTR  strText, CHAR* szDest);


// KMP字符串匹配 
// 返回 0 代表匹配不成功
// 返回非负值 则代表主串中匹配的位置
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

///从指定模块中读取指定类型的资源数据 
///需要两次调用，第一次调用@ptrBuffer == NULL,返回数据大小
///第二次传入分配好的@ptrBuffer用于填充资源数据
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