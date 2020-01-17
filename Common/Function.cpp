/*****************************************************************

** 文件名: Function.h

** 创建人: caihaiqun  

** 日  期: 2010-08-26    

** 描  述: 公共函数实现

** 版  本: 1.0 

** 修  改: 
       
******************************************************************/
#include "stdafx.h"
#include <assert.h>
#include "Function.h"
#include <math.h>
#include <sys/stat.h>
#include <ShlObj.h>
#include <winuser.h>
#include <CommDlg.h>
#include "ICMP.H"

#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "version.lib")
#pragma warning(disable:4996)

//==================================================================
// 文件检查
BOOL	IsFileExistA(LPCSTR lpszFileName)
{	
	if (lpszFileName == NULL || strlen(lpszFileName) == 0)
		return FALSE;

	struct _stat32 st;
	if(_stat32(lpszFileName, &st)==0)
		return TRUE;
	
	return FALSE;
}

BOOL	IsFileExistW(LPCWSTR lpszFileName)
{	
	if (lpszFileName == NULL || wcslen(lpszFileName) == 0)
		return FALSE;

	struct _stat32 st;
	if(_wstat32(lpszFileName, &st)==0)
		return TRUE;

	return FALSE;
}

BOOL	IsFileExistExA(LPCSTR lpszFileName)
{	
	if (lpszFileName == NULL || strlen(lpszFileName) == 0)
		return FALSE;

	// network shared
	if (strncmp(lpszFileName, "\\\\", 2) == 0)
	{
		char host[256] = { 0 };
		SpliterStringA(host, lpszFileName, 2, '\\');
		if (strlen(host) == 0)
			return IsFileExistA(lpszFileName);

		if (Ping(host, 100))
			return IsFileExistA(lpszFileName);

		return FALSE; // ping failed
	}

	return IsFileExistA(lpszFileName);
}

BOOL	IsFileExistExW(LPCWSTR lpszFileName)
{	
	if (lpszFileName == NULL || wcslen(lpszFileName) == 0)
		return FALSE;

	// network shared
	if (wcsncmp(lpszFileName, _T("\\\\"), 2) == 0)
	{
		WCHAR wszHost[256] = { 0 };
		SpliterStringW(wszHost, lpszFileName, 2, _T('\\'));
		if (wcslen(wszHost) == 0)
			return IsFileExistW(lpszFileName);

		char szHost[256] = { 0 };
		Unicode2MultiByte(wszHost, szHost, GetACP());
		if (Ping(szHost, 100))
			return IsFileExistW(lpszFileName);

		return FALSE; // ping failed
	}

	return IsFileExistW(lpszFileName);
}

#include <share.h>
#include <stdio.h>
ULONG64	GetFileSizeA(LPCSTR lpszFileName)
{
	FILE* fp = _fsopen(lpszFileName, "rb", _SH_DENYWR);
	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		ULONG64 uSize = _ftelli64(fp);
		fclose(fp);

		return uSize;
	}

	return 0;
}

ULONG64	GetFileSizeW(LPCWSTR lpszFileName)
{
	FILE* fp = _wfsopen(lpszFileName, L"rb", _SH_DENYWR);
	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		ULONG64 uSize = _ftelli64(fp);
		fclose(fp);

		return uSize;
	}

	return 0;
}

//==================================================================
// 字符串处理、数字与字符串互转、进制互转

// 格式化字符串
LPTSTR	Format(LPTSTR szDest, LPCTSTR szFormat,...)
{
	if (szDest == NULL || szFormat == NULL || _tcslen(szFormat) == 0)
		return szDest;

	va_list argList;
	va_start( argList, szFormat );
	_vstprintf(szDest, szFormat, argList);
	va_end( argList );

	return szDest;
}

LPTSTR	FormatError(DWORD dwErrorCode, LPTSTR lpszErrorInfo)
{
	LPVOID lpszErrMsg;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		dwErrorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpszErrMsg,
		0, 
		NULL
		);	

	while(1)
	{
		LPTSTR lpsz = _tcschr((LPTSTR)lpszErrMsg, _T('\r'));
		if (lpsz == NULL)
			break;
		lpsz[0] = _T('');
	}

	while(1)
	{
		LPTSTR lpsz = _tcschr((LPTSTR)lpszErrMsg, _T('\n'));
		if (lpsz == NULL)
			break;
		lpsz[0] = _T('');
	}

	_stprintf(lpszErrorInfo, _T("%s(%d)"), (LPCTSTR)lpszErrMsg, dwErrorCode);

	::LocalFree(lpszErrMsg);

	return lpszErrorInfo;
}

int		SpliterStringA(CHAR* szSub, LPCSTR lpszFullString, int iSubString, CHAR chSep)
{
	if (szSub == NULL || lpszFullString == NULL || strlen(lpszFullString) == 0)
		return -1;

	szSub[0] = 0;

	if (iSubString < 0)
	{
		strcpy(szSub, lpszFullString);
		return strlen(szSub);
	}

	while (iSubString--)
	{
		lpszFullString = strchr(lpszFullString, chSep);
		if (lpszFullString == NULL)
			return -1;

		lpszFullString++;       // point past the separator
	}

	LPCSTR lpchEnd = strchr(lpszFullString, chSep);
	int nLen = (lpchEnd == NULL)? lstrlenA(lpszFullString) : (int)(lpchEnd - lpszFullString);

	if (nLen > 0)
	{
		strncpy(szSub, lpszFullString, nLen);
		szSub[nLen] = 0;
	}

	return nLen;
}

int		SpliterStringW(WCHAR* szSub, LPCWSTR lpszFullString, int iSubString, WCHAR chSep)
{
	if (szSub == NULL || lpszFullString == NULL || wcslen(lpszFullString) == 0)
		return -1;

	szSub[0] = 0;

	if (iSubString < 0)
	{
		wcscpy(szSub, lpszFullString);
		return wcslen(szSub);
	}

	while (iSubString--)
	{
		lpszFullString = wcschr(lpszFullString, chSep);
		if (lpszFullString == NULL)
			return -1;

		lpszFullString++;       // point past the separator
	}

	LPCWSTR lpchEnd = wcschr(lpszFullString, chSep);
	int nLen = (lpchEnd == NULL)? lstrlenW(lpszFullString) : (int)(lpchEnd - lpszFullString);

	if (nLen > 0)
	{
		wcsncpy(szSub, lpszFullString, nLen);
		szSub[nLen] = 0;
	}

	return nLen;
}

// ASCII字符转换为8个二进制字符
LPSTR	ctob(char* szDest, char c)
{	
	if (szDest == NULL)
		return NULL;

	memset(szDest, 0, 8);

	int ref[8] = { 128, 64, 32, 16, 8, 4, 2, 1 };

	for( int i = 0; i < 8; i++ )
	{
		szDest[i] = ( c & ref[i] ) == 0? 48 : 49;
	}

	szDest[8] = 0; // add this code after GTest

	return szDest;
}


// ASCII码字符串转化成二进制字符串
LPSTR	atob(char* szDest, const char *szSource, int nLen)
{
	if (szDest == NULL)
		return szDest;

	if (szSource == NULL || nLen <= 0)
	{
		szDest[0] = 0;
		return szDest;
	}

	char* strAddr = szDest;

	int ref[8] = { 128, 64, 32, 16, 8, 4, 2, 1 };
	for (int i = 0; i < nLen; i++)
	{
		*szDest++ = (szSource[i] & ref[0]) == ref[0]?  '1' : '0';
		*szDest++ = (szSource[i] & ref[1]) == ref[1]?  '1' : '0';
		*szDest++ = (szSource[i] & ref[2]) == ref[2]?  '1' : '0';
		*szDest++ = (szSource[i] & ref[3]) == ref[3]?  '1' : '0';
		*szDest++ = (szSource[i] & ref[4]) == ref[4]?  '1' : '0';
		*szDest++ = (szSource[i] & ref[5]) == ref[5]?  '1' : '0';
		*szDest++ = (szSource[i] & ref[6]) == ref[6]?  '1' : '0';
		*szDest++ = (szSource[i] & ref[7]) == ref[7]?  '1' : '0';
	}
	*szDest++ = 0;

	return strAddr;
}

// 二进制字符串转换为ASCII字符串
LPSTR	btoa(char* szDest, const char *szSource, int nLen)
{
	if (szDest == NULL || szSource == NULL || nLen <= 0)
		return szDest;

	char* strAddr = szDest;

	int nLeft = nLen % 8;

	nLen /= 8;

	for ( int i = 0; i < nLen; i++ )
	{
		*szDest++ = (char)btoi(szSource, 8);
		szSource += 8;
	}

	if ( nLeft > 0 )
	{
		*szDest++ = (char)( btoi(szSource, nLeft) << ( 8 - nLeft) );
	}

	return strAddr;
}


// 二进制字符串转换成整型值
unsigned int btoi(const char *szBin, int nLen)
{
	if (szBin == NULL || nLen == 0) 
		return -1;

	unsigned int nRet = 0;
	unsigned int nBase = int(pow(2.0, nLen-1));
	for (int i = 0; i < nLen; i++)
	{
		if (szBin[i] == '1') 
			nRet += nBase;

		nBase /= 2;
	}

	return nRet;	
}

// 二进制字符串转换成十六进制形式
LPSTR	btoh(char* szHex, const char *szBin, int nLen)
{
	if (szHex == NULL || szBin == NULL || nLen <= 0)
		return szHex;

	char* strAddr = szHex;	

	unsigned int ret4 = 0;
	while (nLen >= 4)
	{
		ret4 = btoi(szBin, 4);
		*szHex++ = ret4 > 9? ret4 + 55 : ret4 + 48;

		szBin	+= 4;
		nLen	-= 4;
	}

	return strAddr;
}

// ASCII字符串转换为16进制形式
LPSTR	atoh(char* szHex, const char* szAcsii, int nLen, bool bSpace/* = true*/)
{
	if (szHex == NULL)
		return szHex;

	if (szAcsii == NULL || nLen <= 0)
	{
		szHex[0] = 0;
		return szHex;
	}
	
	char* strAddr = szHex;	

	char bits4 = 0;

	for( int i = 0; i < nLen; i++ )
	{
		bits4 = (szAcsii[i] & 0xF0) >> 4;
		*szHex++ = (bits4 < 10? 48 + bits4 : 55 + bits4);

		bits4 = szAcsii[i] & 0x0F;
		*szHex++ = (char)(bits4 < 10? 48 + bits4 : 55 + bits4);

		if ( bSpace ) *szHex++ = ' ';
	}

	*szHex++ = 0;

	return strAddr;
}

// 16进制形式转换为ASCII字符串
int		htoa(char* szAscii, const char* szHex)
{
	if (szHex == NULL || szAscii == NULL)
		return 0;
	
	char ch		 = 0;
	int  nLength = strlen(szHex);
	int  nAscii  = 0;

	while(szHex[0] == ' ')
	{
		nLength--;
		szHex++;
	}

	while( nLength >= 2 )
	{
		if ( szHex[0] >= '0' && szHex[0] <= '9' )
			ch = (szHex[0] & 0xFF) - 48;
		else if ( szHex[0] >= 'A' && szHex[0] <= 'F' )
			ch = (szHex[0] & 0xFF) - 'A' + 10;
		else if ( szHex[0] >= 'a' && szHex[0] <= 'f' )
			ch = (szHex[0] & 0xFF) - 'a' + 10;
		else break;

		ch *= 16;

		if ( szHex[1] >= '0' && szHex[1] <= '9' )
			ch += (szHex[1] & 0xFF) - 48;
		else if ( szHex[1] >= 'A' && szHex[1] <= 'F' )
			ch += (szHex[1] & 0xFF) - 'A' + 10;
		else if ( szHex[0] >= 'a' && szHex[0] <= 'f' )
			ch += (szHex[0] & 0xFF) - 'a' + 10;
		else break;

		*szAscii++ = (char)ch;
		nAscii++;

		szHex += 2;
		nLength -= 2;

		while(szHex[0] == ' ')
		{
			nLength--;
			szHex++;
		}
	}

	*szAscii++ = 0;

	return nAscii;
}

// 字符串转换成数字 HEX必须以0x开头
ULONG64	stoi(LPCSTR str)
{
	if (str == NULL || strlen(str) == 0)
		return 0;

	ULONG64 ulRet = 0;

	int nLength = strlen(str);

	// hex
	if (strnicmp(str, "0x", 2) == 0)
	{
		str += 2;
		nLength -= 2;

		while(nLength > 0)
		{
			if(str[0] >= '0' && str[0] <= '9')
				ulRet += (str[0] - '0') * (ULONG64)pow(16.0, nLength - 1);
			else if(str[0] >= 'A' && str[0] <= 'F')
				ulRet += (str[0] - 'A' + 10) * (ULONG64)pow(16.0, nLength - 1);
			else if(str[0] >= 'a' && str[0] <= 'f')
				ulRet += (str[0] - 'a' + 10) * (ULONG64)pow(16.0, nLength - 1);
			else 
				return 0; 

			str++;
			nLength--;
		}
	}

	// decimal
	while(nLength > 0)
	{
		if(*str >= _T('0') && *str <= _T('9'))
			ulRet += (*str - _T('0')) * (ULONGLONG)pow(10.0, nLength-1);
		else 
			return 0; 

		str++;
		nLength--;
	}

	return ulRet;
}

ULONG64	stoi(LPCWSTR str)
{
	if (str == NULL || wcslen(str) == 0)
		return 0;

	ULONG64 ulRet = 0;

	int nLength = wcslen(str);
	
	// hex
	if (wcsnicmp(str, _T("0x"), 2) == 0)
	{
		str += 2;
		nLength -= 2;
		
		while(nLength > 0)
		{
			if(str[0] >= _T('0') && str[0] <= _T('9'))
				ulRet += (str[0] - _T('0')) * (ULONG64)pow(16.0, nLength - 1);
			else if(str[0] >= _T('A') && str[0] <= _T('F'))
				ulRet += (str[0] - _T('A') + 10) * (ULONG64)pow(16.0, nLength - 1);
			else if(str[0] >= _T('a') && str[0] <= _T('f'))
				ulRet += (str[0] - _T('a') + 10) * (ULONG64)pow(16.0, nLength - 1);
			else 
				return 0; 
			
			str++;
			nLength--;
		}
	}
	
	// decimal
	while(nLength > 0)
	{
		if(*str >= _T('0') && *str <= _T('9'))
			ulRet += (*str - _T('0')) * (ULONGLONG)pow(10.0, nLength-1);
		else 
			return 0; 
		
		str++;
		nLength--;
	}

	return ulRet;
}

// base64编解码

const char * base64char = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char* base64_encode(const unsigned char *data, char *base64, int length)
{
	int i, j;
	unsigned char current;
	
	for (i = 0, j = 0; i < length; i += 3 )
	{
		current = (data[i] >> 2) ;
		current &= (unsigned char)0x3F;
		base64[j++] = base64char[(int)current];

		current = ( (unsigned char)(data[i] << 4 ) ) & ( (unsigned char)0x30 ) ;
		if ( i + 1 >= length )
		{
			base64[j++] = base64char[(int)current];
			base64[j++] = '=';
			base64[j++] = '=';
			break;
		}

		current |= ( (unsigned char)(data[i+1] >> 4) ) & ( (unsigned char) 0x0F );
		base64[j++] = base64char[(int)current];

		current = ( (unsigned char)(data[i+1] << 2) ) & ( (unsigned char)0x3C ) ;
		if ( i + 2 >= length )
		{
			base64[j++] = base64char[(int)current];
			base64[j++] = '=';
			break;
		}

		current |= ( (unsigned char)(data[i+2] >> 6) ) & ( (unsigned char) 0x03 );
		base64[j++] = base64char[(int)current];

		current = ( (unsigned char)data[i+2] ) & ( (unsigned char)0x3F ) ;
		base64[j++] = base64char[(int)current];
	}

	base64[j] = '\0';

	return base64;
}

int		base64_decode(const char* base64, unsigned char *data )
{
	int i, j;
	unsigned char k;
	unsigned char temp[4];
	for (i = 0, j = 0; base64[i] != '\0'; i += 4)
	{
		memset( temp, 0xFF, sizeof(temp) );
		for ( k = 0 ; k < 64 ; k ++ )
		{
			if ( base64char[k] == base64[i] )
				temp[0]= k;
		}

		for ( k = 0 ; k < 64 ; k ++ )
		{
			if ( base64char[k] == base64[i+1] )
				temp[1]= k;
		}

		for ( k = 0 ; k < 64 ; k ++ )
		{
			if ( base64char[k] == base64[i+2] )
				temp[2]= k;
		}

		for ( k = 0 ; k < 64 ; k ++ )
		{
			if ( base64char[k] == base64[i+3] )
				temp[3]= k;
		}

		data[j++] = ((unsigned char)(((unsigned char)(temp[0] << 2))&0xFC)) |
			((unsigned char)((unsigned char)(temp[1]>>4)&0x03));
		if ( base64[i+2] == '=' )
			break;

		data[j++] = ((unsigned char)(((unsigned char)(temp[1] << 4))&0xF0)) |
			((unsigned char)((unsigned char)(temp[2]>>2)&0x0F));
		if ( base64[i+3] == '=' )
			break;

		data[j++] = ((unsigned char)(((unsigned char)(temp[2] << 6))&0xF0)) |
			((unsigned char)(temp[3]&0x3F));
	}

	return j;
}


//////////////////////////////////////////////////////////////////////////
/// 字符编码转换
#define	CHINESE_SIMPLIFIED	1
#define	CHINESE_TRADITIONAL	2

// Multi-Byte to UNICODE
int		MultiByte2Unicode(CONST CHAR* szSource, LPWSTR wszDest, int nEncoding/* = CP_ACP*/)
{
	if (szSource == NULL || strlen(szSource) == 0)
		return 0;

	int nLanguage = CP_ACP;
	if(nEncoding == CHINESE_SIMPLIFIED)
		nLanguage = 936;
	else if(nEncoding == CHINESE_TRADITIONAL)
		nLanguage = 950;
	
	int nLength = ::MultiByteToWideChar(nLanguage, 0, szSource, strlen(szSource), NULL, 0);

	if (wszDest == NULL)
		return nLength;
	
	MultiByteToWideChar(nLanguage,0,(LPCSTR)szSource, strlen(szSource), (LPWSTR)wszDest, nLength);
	wszDest[nLength] = 0;

	return nLength;
}

// UNICODE to Multi-Byte
int		Unicode2MultiByte(LPCWSTR wszSource, CHAR* szDest, int nEncoding/* = CP_ACP*/)
{
	if (wszSource == NULL || wcslen(wszSource) == 0)
		return 0;

	int nLanguage = CP_ACP;
	if (nEncoding == CHINESE_SIMPLIFIED)
		nLanguage = 936;
	else if (nEncoding == CHINESE_TRADITIONAL)
		nLanguage = 950;

	CHAR szUnknown[2] = { 0x20, 0x00 };

	int nLength = WideCharToMultiByte(nLanguage, 0, wszSource, wcslen(wszSource),NULL, 0, szUnknown, FALSE);
	if (szDest == NULL)
		return nLength;
	
	WideCharToMultiByte(nLanguage, 0, wszSource, wcslen(wszSource), szDest, nLength, szUnknown, FALSE);
	szDest[nLength] = 0;

	return nLength;
}

void UNICODE2UTF8(LPCWSTR  strText, CHAR* szDest) 
{
	int len;  
	len = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)strText, -1, NULL, 0, NULL, NULL);  

	WideCharToMultiByte (CP_UTF8, 0, (LPCWSTR)strText, -1, szDest, len, NULL,NULL);
	
}

// KMP字符串匹配 
int		IndexKMP(const char* pSrc, int nSrcLen, const char* pSub, int nSubLen)
{
	#define MAX_SUBLEN	128

	assert(nSubLen > 1 && nSubLen <= MAX_SUBLEN);

	// get_next
	int i = 1, j = 0;

	int next[MAX_SUBLEN] = { 0 };    
    next[0] = -1;
	
    while(i < nSubLen)    //整个过程是模式串跟自己的一个匹配过程
	{
        if(0 == j || pSub[i] == pSub[j])
		{
            i++;
            j++;
            next[i] = j;
        }
        else
		{
            j = next[j];
		}
    }


    i = 0;
	j = 1;	
	
    while(i < nSrcLen && j < nSubLen)
	{
        if( j == 0 || pSrc[i] == pSub[j])
		{
            i++;
            j++;
        }
        else
		{
            j = next[j];    // 
        }
    }
	
    if( j >= nSubLen)
        return (i - nSubLen + 1);   
    
	return 0;
}

//==================================================================
/// Get version information from DLL, like as FILEVERSION in rc file
BOOL	GetDllVersion(LPCTSTR lpszDLL, VS_FIXEDFILEINFO* ptrVersionInfo)
{ 
	if (ptrVersionInfo == NULL)
		return FALSE;

	DWORD dwVerHnd = 0; 
	DWORD dwVerInfoSize = ::GetFileVersionInfoSize ((TCHAR*)lpszDLL, &dwVerHnd); 
	if (!dwVerInfoSize) // Cannot reach the DLL file 
		return FALSE; 
 
	LPSTR lpstrVffInfo =  (LPSTR) malloc (dwVerInfoSize); // Alloc memory for file info 
	if (lpstrVffInfo == NULL) 
		return FALSE; // Allocation failed 
 
	// Try to get the info 
	if (!::GetFileVersionInfo((TCHAR*)lpszDLL, dwVerHnd, dwVerInfoSize, lpstrVffInfo))
	{ 
		free (lpstrVffInfo); 
		return FALSE; // Cannot read the file information wierd, since we could read the information size 
	} 

	/* 
	The below 'hex' value looks a little confusing, but 
	essentially what it is, is the hexidecimal representation 
	of a couple different values that represent the language 
	and character set that we are wanting string values for. 
	040904E4 is a very common one, because it means: 
	US English, Windows MultiLingual characterset 
	Or to pull it all apart: 
	04------ = SUBLANG_ENGLISH_USA 
	--09---- = LANG_ENGLISH 
	----04E4 = 1252 = Code page for Windows:Multilingual 
	*/ 	

	LPSTR lpVersion; // String pointer to 'version' text 
	UINT uVersionLen; 
	if (!VerQueryValue(lpstrVffInfo,  _T("\\"),  (LPVOID *)&lpVersion, (UINT *)&uVersionLen))
	{ 
		free (lpstrVffInfo); 
		return FALSE; // Query was unsuccessful 
	} 

	memset(ptrVersionInfo, 0, sizeof(ptrVersionInfo));
	memcpy(ptrVersionInfo, lpVersion, uVersionLen);

	free (lpstrVffInfo); 

	return TRUE;  
} 

///=================================================================
/// Windows Configuration
LPTSTR	GetCpuInfo(LPTSTR szInfo)
{
	szInfo[0] = 0;
	HKEY hKey;
	LPCTSTR lpszKey = _T("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0");
	if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpszKey, NULL, KEY_ALL_ACCESS, &hKey)) 
	{ 
		DWORD dwSize = 255, dwType = REG_SZ;  
		TCHAR szCPU[256] = { 0 }; 
		LPCTSTR lpszValue = _T("ProcessorNameString");
		if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, lpszValue, 0, &dwType, (LPBYTE)szCPU, &dwSize)) 
		{
			// TrimLeft
			DWORD dwIndex = 0;
			while (dwIndex < dwSize)
			{
				if (szCPU[dwIndex] != _T(' '))
					break;
				dwIndex++;
			}

			_tcscat(szInfo, &szCPU[dwIndex]);
		}
		::RegCloseKey(hKey); 
	} 

	return szInfo;

// 	利用利用CPUID 汇编指令(机器码:0FH A2H, 如果你的编译器不支持CPUID指令,只有emit 机器码了)
// 	该指令可以被如下CPU识别: Intel 486 以上的CPU, Cyrix M1 以上的CPU, AMD Am486 以上的CPU
// 
//  (1) 取CPU OEM 字符串, 判断CPU 厂商. 让EAX=0,调用CPUID 
// 　　 Inel的CPU将返回"GenuineIntel"
//			EBX:756E6547H 'Genu'
//			EDX:49656E69H 'ineI'
//			ECX:6C65746EH 'ntel'
// 
// 　　Cyrix 的CPU 将返回"CyrixInstead"
//			EBX:43797269H
//			EDX:78496E73H
//			ECX:74656164H
// 
// 　　AMD 的CPU 将返回"AuthenticAMD"
//			EBX:41757468H
//			EDX:656E7469H
//			ECX:63414D44H
// 
//  (2)CPU 到底是几86, 是否支持MMX. 让EAX=1,调用CPUID 
// 　　EAX的8到11位就表明是x86 
// 　　		3 - 386
// 　　		4 - i486
// 　　		5 - Pentium
// 　　		6 - Pentium Pro Pentium II
// 　　		2 - Dual Processors
// 　　EDX 的第0位：有无FPU
// 　　EDX的第23位:CPU是否支持IA MMX,很重要啊! 如果你想用那57条新增的指令,先检查这一位吧.
//	   否则就等着看Windows的"该程序执行了非法指令,将被关闭" 。 
// 
//  (3) 专门检测是否P6架构.让EAX=1,再调用CPUID 
// 　　如果AL=1,就是Pentium Pro 或Pentium II 

//  (4) 专门检测AMD的CPU信息. 先让EAX=80000001H,再调用CPUID 
// 　　EAX=51H, AMD K5; EAX=66H, AMD K6 
// 　　EDX第0 位: 是否有FPU( 多余的 ！谁用过没FPU的K5,K6?) 
// 　　EDX 第23 位，CPU是否支持MMX, 

// //------CPUID Instruction Demo Program------------
// #include 
// #include 
// #pragma hdrstop
// //------------------------------------------------
// #pragma inline
// #pragma argsused
// int main(int argc, char **argv)
// {
// 　char OEMString[13];
// 　int iEAXValue,iEBXValue,iECXValue,iEDXValue;
// 　_asm {
// 　　mov eax,0
// 　　cpuid
// 　　mov DWORD PTR OEMString,ebx
// 　　mov DWORD PTR OEMString+4,edx
// 　　mov DWORD PTR OEMString+8,ecx
// 　　mov BYTE PTR OEMString+12,0
// 　}
// 　cout< < "This CPU 's OEM String is:"< < OEMString< < endl;
// 　_asm {
// 　　mov eax,1
// 　　cpuid
// 　　mov iEAXValue,eax
// 　　mov iEBXValue,ebx
// 　　mov iECXValue,ecx
// 　　mov iEDXValue,edx
// 　}
// 　if(iEDXValue&0x800000)
// 　　cout < < "This is MMX CPU"< < endl;
// 　else
// 　　cout < < "None MMX Support."< < endl;
// 　　int iCPUFamily=(0xf00 & iEAXValue) > >8;
// 　　cout < < "CPU Family is:"< < iCPUFamily< < endl;
// 　　_asm{
// 　　　mov eax,2
// 　　　CPUID
// 　　}
// 　　if(_AL==1)
// 　　　cout < < "Pentium Pro or Pentium II Found";
// 　　　getch();
// 　return 0;
// }
}

LPTSTR	GetOsInfo(LPTSTR szOS)
{
	szOS[0] = 0;
	HKEY hKey;
	LPCTSTR lpszKey = _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion");
	if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpszKey, NULL, KEY_ALL_ACCESS, &hKey)) 
	{ 		
		TCHAR szInfo[256] = { 0 }; 
		DWORD dwSize = sizeof(szInfo) - 1, dwType = REG_SZ;  

		LPCTSTR lpszProductName = _T("ProductName");
		if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, lpszProductName, 0, &dwType, (LPBYTE)szInfo, &dwSize)) 
		{			
			_tcscpy(szOS, szInfo);

			dwSize = sizeof(szInfo) - 1;
			LPCTSTR lpszVersion = _T("CurrentVersion");
			if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, lpszVersion, 0, &dwType, (LPBYTE)szInfo, &dwSize)) 
			{
				_tcscat(szOS, _T(" (Ver: "));
				_tcscat(szOS, szInfo);
				_tcscat(szOS, _T(", "));

				dwSize = sizeof(szInfo) - 1;
				LPCTSTR lpszBuild = _T("BuildLab");
				if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, lpszBuild, 0, &dwType, (LPBYTE)szInfo, &dwSize)) 
				{
					_tcscat(szOS, _T("Build: "));
					_tcscat(szOS, szInfo);
				}
				_tcscat(szOS, _T(")"));
			}
		}
		::RegCloseKey(hKey); 
	} 
	
	return szOS;
}

///=================================================================
/// File & Path

LPTSTR	GetPath(LPCTSTR lpszFile, TCHAR* szPath)
{
	if (szPath == NULL)
		return NULL;

	// find last single character
	LPCTSTR lpsz = _tcsrchr(lpszFile, _T('\\'));
	
	if (lpsz)
	{
		int nLen = (int)(lpsz - lpszFile + 1);
		_tcsncpy(szPath, lpszFile, nLen);
		szPath[nLen] = 0;
	}		

	return szPath;
}

LPWSTR	GetFile(LPCWSTR lpszCurrentPath, LPCWSTR lpszInputFile, WCHAR* szDest)
{
	if(lpszCurrentPath == NULL || lpszInputFile == NULL || szDest == NULL)	
		return NULL;

	szDest[0] = 0;

	int nLength = wcslen(lpszCurrentPath);
	if (nLength == 0)
	{
		wcscpy(szDest, lpszInputFile);
		return szDest;
	}
	
	if (wcslen(lpszInputFile) < 3)		// Input file name
	{
		if (lpszCurrentPath[nLength-1] == _T('\\'))
			swprintf(szDest, _T("%s%s"), lpszCurrentPath, lpszInputFile);
		else
			swprintf(szDest, _T("%s\\%s"), lpszCurrentPath, lpszInputFile);
		return szDest;
	}
	
	if (lpszInputFile[1] == _T(':'))	// Input absolute file path
	{
		wcscpy(szDest, lpszInputFile);
		return szDest;
	}
	
	if (lpszInputFile[0] == _T('.'))	// relative
	{
		if (lpszInputFile[1] == '.')	// back
		{
			TCHAR szDIR[32][MAX_PATH] = { { 0 } };
			
			int nCount = 0;
			if (lpszCurrentPath[0] == _T('\\')) // Network shared 
			{
				lpszCurrentPath += 2;
				wcscpy(szDIR[0], _T("\\"));
				nCount++;
			}
			
			int nIndex = 0;
			TCHAR szSub[MAX_PATH] = { 0 };
			while(SpliterStringW(szSub, lpszCurrentPath, nIndex, _T('\\')) > 0)
			{				
				nIndex++;
				wcscpy(szDIR[nCount], szSub);
				memset(szSub, 0, sizeof(szSub));
				nCount++;
			}
			
			nIndex = 0;

			memset(szSub, 0, sizeof(szSub));
			while(SpliterStringW(szSub, lpszInputFile, nIndex, _T('\\')) > 0)
			{
				if (wcsicmp(szSub, _T("..")) != 0)
					break;
				nIndex++;
				memset(szSub, 0, sizeof(szSub));
			}

			lpszInputFile += nIndex * 3;
			
			if (nIndex >= nCount)
				swprintf(szDest, _T("%s\\%s"), szDIR[0], lpszInputFile);
			else {
				
				nCount -= nIndex;
				if (wcscmp(szDIR[0], _T("\\")) == 0 && nCount < 2) nCount = 2; // network
				for (int i = 0; i < nCount; i++)
				{
					wcscat(szDest, szDIR[i]);
					wcscat(szDest, _T("\\"));
				}
				wcscat(szDest, lpszInputFile);
			}
		}
		else if (lpszInputFile[1] == _T('\\'))	// sub ".\"
		{
			swprintf(szDest, _T("%s%s"), lpszCurrentPath, (LPCTSTR)&lpszInputFile[2]);
		}

		return szDest;
	}
	
	// @lpszInputFile is file name
	
	if (lpszCurrentPath[nLength-1] == _T('\\'))
		swprintf(szDest, _T("%s%s"), lpszCurrentPath, lpszInputFile);
	else
		swprintf(szDest, _T("%s\\%s"), lpszCurrentPath, lpszInputFile);

	return szDest;
}

LPSTR	GetFile(LPCSTR lpszCurrentPath, LPCSTR lpszInputFile, CHAR* szDest)
{
	if(lpszCurrentPath == NULL || lpszInputFile == NULL || szDest == NULL)	
		return NULL;

	szDest[0] = 0;

	int nLength = strlen(lpszCurrentPath);
	if (nLength == 0)
	{
		strcpy(szDest, lpszInputFile);
		return szDest;
	}

	if (strlen(lpszInputFile) < 3)		// Input file name
	{
		if (lpszCurrentPath[nLength-1] == '\\')
			sprintf(szDest, "%s%s", lpszCurrentPath, lpszInputFile);
		else
			sprintf(szDest, "%s\\%s", lpszCurrentPath, lpszInputFile);
		return szDest;
	}

	if (lpszInputFile[1] == _T(':'))	// Input absolute file path
	{
		strcpy(szDest, lpszInputFile);
		return szDest;
	}

	if (lpszInputFile[0] == '.')	// relative
	{
		if (lpszInputFile[1] == '.')	// back
		{
			CHAR szDIR[32][MAX_PATH] = { { 0 } };

			int nCount = 0;
			if (lpszCurrentPath[0] == '\\') // Network shared 
			{
				lpszCurrentPath += 2;
				strcpy(szDIR[0], "\\");
				nCount++;
			}

			int nIndex = 0;
			CHAR szSub[MAX_PATH] = { 0 };
			while(SpliterStringA(szSub, lpszCurrentPath, nIndex, '\\') > 0)
			{				
				nIndex++;
				strcpy(szDIR[nCount], szSub);
				memset(szSub, 0, sizeof(szSub));
				nCount++;
			}

			nIndex = 0;

			memset(szSub, 0, sizeof(szSub));
			while(SpliterStringA(szSub, lpszInputFile, nIndex, '\\') > 0)
			{
				if (stricmp(szSub, "..") != 0)
					break;
				nIndex++;
				memset(szSub, 0, sizeof(szSub));
			}

			lpszInputFile += nIndex * 3;

			if (nIndex >= nCount)
				sprintf(szDest, "%s\\%s", szDIR[0], lpszInputFile);
			else {

				nCount -= nIndex;
				if (strcmp(szDIR[0], "\\") == 0 && nCount < 2) nCount = 2; // network
				for (int i = 0; i < nCount; i++)
				{
					strcat(szDest, szDIR[i]);
					strcat(szDest, "\\");
				}
				strcat(szDest, lpszInputFile);
			}
		}
		else if (lpszInputFile[1] == '\\')	// sub ".\"
		{
			sprintf(szDest, "%s%s", lpszCurrentPath, (LPCTSTR)&lpszInputFile[2]);
		}

		return szDest;
	}

	// @lpszInputFile is file name

	if (lpszCurrentPath[nLength-1] == '\\')
		sprintf(szDest, "%s%s", lpszCurrentPath, lpszInputFile);
	else
		sprintf(szDest, "%s\\%s", lpszCurrentPath, lpszInputFile);

	return szDest;
}

LPTSTR	GetRelativePath(LPCTSTR lpszAbsolutePath1, LPCTSTR lpszAbsolutePath2, LPTSTR szRelative)
{
	// e:\duilib v1.0\Production Tool - DUI\bin\Plug In\Image\
	// e:\duilib v1.0\Production Tool - DUI\bin\skin\
	//
	
	if (lpszAbsolutePath1 == NULL || lpszAbsolutePath2 == NULL || szRelative == NULL)
		return NULL;

	szRelative[0] = 0;

	int nLength = min(_tcslen(lpszAbsolutePath1), _tcslen(lpszAbsolutePath2));

	for (int i = 0; i < nLength; i++)
	{
		if (lpszAbsolutePath1[i] == lpszAbsolutePath2[i])
			continue;

		int nDepth = 0;
		nLength = _tcslen(lpszAbsolutePath2);
		for (int j = i+1; j < nLength; j++)
		{
			if (lpszAbsolutePath2[j] == _T('\\'))
				nDepth++;
		}

		if (nDepth == 0)
			_tcscat(szRelative, _T(".\\"));
		else for (int k = 0; k < nDepth; k++)
			_tcscat(szRelative, _T("..\\"));
		_tcscat(szRelative, &lpszAbsolutePath1[i]);

		break;
	}

	nLength = _tcslen(szRelative);
	if (nLength > 0)
		szRelative[nLength] = 0;

	return szRelative;
}

// 从注册表读Open/Save对话框最后一次的记录
BOOL	GetOpenSaveMRU(LPCTSTR lpszExtName, LPTSTR lpszMRUPath, BOOL bSave)
{
	/*
	HKEY hKEY;
	TCHAR szMRU[MAX_PATH] = { 0 };
	_stprintf(szMRU, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ComDlg32\\OpenSaveMRU\\%s"), lpszExtName);
	LONG lRet = RegOpenKeyEx(HKEY_CURRENT_USER, szMRU, 0, KEY_READ, &hKEY);
	if (lRet != ERROR_SUCCESS )
		return FALSE;

	DWORD dwType = REG_SZ;
	DWORD dwByte = 520;
	BYTE  buffer[520] = { 0 };
	lRet = RegQueryValueEx(hKEY, _T("MRUList"), NULL, &dwType, buffer, &dwByte);
	if (lRet != ERROR_SUCCESS || strlen((char*)buffer) == 0)
		return FALSE;

	dwByte = 520;
	char szLatest[16] = {buffer[0], 0};
	lRet = RegQueryValueEx(hKEY,(LPTSTR)szLatest, NULL, &dwType, (LPBYTE)lpszMRUPath, &dwByte);
	if (lRet != ERROR_SUCCESS)
		return FALSE;

	RegCloseKey(hKEY);
	
	TCHAR szAppPath[520] = { 0 };
	if (!GetSystemPath(szAppPath, CSIDL_COMMON_APPDATA, APP_FOLDER_W))
		return FALSE;

	_tcscat(szAppPath, _T("\\MRU.INI"));

	if (bSave)
	{
		if (lpszMRUPath)
			WritePrivateProfileString(_T("MRU"), lpszExtName, lpszMRUPath, szAppPath);
	}
	else
	{	
		TCHAR szValue[MAX_PATH] = { 0 };
		GetPrivateProfileString(_T("MRU"), lpszExtName, szValue, szValue, MAX_PATH, szAppPath);	
		if (lpszMRUPath)
			_tcscpy(lpszMRUPath, szValue);
	}	

	*/
	return TRUE;
}

LPTSTR	OpenFile(HWND hWnd, LPTSTR lpszTitle, LPCTSTR lpszFilter, LPCTSTR lpszInitDir, TCHAR* szDest, DWORD dwMax)
{	
	TCHAR szMRU[MAX_PATH] = { 0 };
	memset(szDest, 0, dwMax);

	OPENFILENAME ofn; 
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize		= sizeof(ofn);
	ofn.hwndOwner		= hWnd;
	ofn.lpstrFile		= szDest;			
	ofn.nMaxFile		= dwMax;
	ofn.lpstrFilter		= lpszFilter;
	ofn.nFilterIndex	= 0;
	ofn.lpstrFileTitle	= lpszTitle;
	ofn.nMaxFileTitle	= lpszTitle? _tcslen(lpszTitle) : 0;
	ofn.lpstrInitialDir = lpszInitDir;
	ofn.Flags			= OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST;
/*
	if (lpszInitDir == NULL)
	{
		// apply latest directory as default
	}
	else if (_tcslen(lpszInitDir) == 0)
	{
		GetCurrentDirectory(_countof(szMRU)-1, szMRU);
	}
	else if (_tcsncmp(lpszInitDir, _T("\\\\"), 2) == 0)
	{// network shared
		TCHAR szHost[256] = { 0 };
		SpliterString(szHost, lpszInitDir, 2, _T('\\'));
		if (_tcslen(szHost) == 0)
		{
			szMRU[0] = 0;
			GetCurrentDirectory(_countof(szMRU)-1, szMRU);// invalid profile
		}
		else
		{
#ifdef _UNICODE
			char host[256] = { 0 };
			Unicode2MultiByte(szHost, host, GetACP());
			if (!Ping(host, 100))
#else
			if (!Ping(szHost, 100))
#endif
			{
				szMRU[0] = 0;
				GetCurrentDirectory(_countof(szMRU)-1, szMRU);// ping failed
			}
		}
	}
	else _tcscpy(szMRU, lpszInitDir);

	//DbgPrint(_T("Init: %s"), szInit);
	if (_tcslen(szMRU) > 0)
		ofn.lpstrInitialDir = szMRU;
*/
	if (GetOpenFileName(&ofn))
	{
		_tcscpy(szDest, ofn.lpstrFile);
	}

	return szDest;	
} 

///Drowse directory
LPTSTR	BrowsePath(TCHAR* szDest, HWND hOwer, LPCTSTR lpszTitle, BFFCALLBACK pfnCallBack, LPARAM lParam)
{
	BROWSEINFO bi;	
	LPITEMIDLIST pIDList = NULL;
	//初始化入口参数bi开始
	bi.hwndOwner	= hOwer;
	bi.pidlRoot		= pIDList;	
	bi.lpszTitle	= lpszTitle;
	bi.ulFlags		= BIF_RETURNONLYFSDIRS;
	bi.lpfn			= pfnCallBack;
	bi.lParam		= lParam;
	bi.iImage		= NULL;
	bi.pszDisplayName = szDest;//此参数如为NULL则不能显示对话框
	
	//初始化入口参数bi结束
	pIDList = SHBrowseForFolder(&bi);//调用显示选择对话框
	if(pIDList)
	{
		SHGetPathFromIDList(pIDList, szDest);
		
		int nLength = _tcslen(szDest);
		if (nLength > 0 && szDest[nLength-1] != '\\')
			szDest[nLength] = '\\';
		else
			szDest[0] = 0;
	}
	else
	{
		szDest[0] = 0;
	}
	
	//释放内存
	LPMALLOC lpMalloc;
	if(SUCCEEDED(SHGetMalloc(&lpMalloc))) 
	{
		lpMalloc->Free(pIDList);
		lpMalloc->Release();
	}
	
	return szDest;
}

CStdString ShowFileBrowse(HWND hWnd,CStdString initPath,CStdString title,OpenType open_type,CStdString suffix,TCHAR * fileter)
{
 
	int nIndex = initPath.ReverseFind('\\');

	CStdString strDec = initPath.Mid(0,nIndex);

 	TCHAR szFile[MAX_PATH] = {0};       // buffer for file name
	OPENFILENAME ofn;

	//_tcscpy(szFile,strFilename);   

	memset(&ofn, 0, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = MAX_PATH; 
	ofn.lpstrFilter = fileter; 
	ofn.lpstrDefExt = suffix.GetData();
	ofn.lpstrTitle = title.GetData();

	ofn.Flags = OFN_OVERWRITEPROMPT/*|OFN_ENABLEHOOK*/;
	// ofn.lpfnHook = OFNHookProc;

	ofn.nFilterIndex = 1; 
	//ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = strDec;

	if(open_type == save_f)
	{
		if(GetSaveFileName(&ofn))
		{
			OutputDebugString(_T("save success!!!"));
			return szFile;
		} 
	}
	else if(open_type == open_f)
	{
		if(GetOpenFileName(&ofn))
		{
			return szFile;
		} 

	}
	OutputDebugString(_T("save failed!!!"));

	return _T("");
}

void	RemoveFile(LPCTSTR lpszPath, LPCTSTR lpszExt, BOOL bRecurse)
{
	TCHAR szSearch[520] = { 0 };
	_stprintf(szSearch, _T("%s%s"), lpszPath, lpszExt);

	WIN32_FIND_DATA fData;  
	HANDLE hFindFile = ::FindFirstFile(szSearch, &fData);  
	if(hFindFile != INVALID_HANDLE_VALUE)  
	{         
		TCHAR	szTempPath[520] = { 0 }; 		
		BOOL	bFinish = FALSE;  

		while(!bFinish)  
		{  
			memset(szTempPath, 0, sizeof(szTempPath));
			_stprintf(szTempPath, _T("%s%s"), lpszPath, fData.cFileName);

			BOOL bIsDirectory = ((fData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);  
			if (!bIsDirectory)
			{
				::SetFileAttributes(szTempPath, FILE_ATTRIBUTE_NORMAL);
				::DeleteFile(szTempPath);
			}

			//如果是.或..  
			else if( bIsDirectory && (_tcscmp(fData.cFileName, _T(".")) == 0 || _tcscmp(fData.cFileName, _T("..")) == 0))   
			{         
				bFinish = (FindNextFile(hFindFile, &fData) == FALSE);  
				continue;  
			}            

			else if (bIsDirectory && bRecurse) //是子目录  
			{
				_tcscat(szTempPath, _T("\\"));
				RemoveFile(szTempPath, lpszExt, bRecurse);
				RemoveDirectory(szTempPath);
			}

			bFinish = (FindNextFile(hFindFile, &fData) == FALSE);  
		}  

		::FindClose(hFindFile);  
	}
}

///从指定模块中读取指定类型的资源数据 
///需要两次调用，第一次调用@ptrBuffer == NULL,返回数据大小
///第二次传入分配好的@ptrBuffer用于填充资源数据
UINT	GetResource(LPCTSTR lpszModule, LPCTSTR lpszResourceName, LPCTSTR lpszResourceType, LPBYTE ptrBuffer)
{
	UINT nLength = 0;

	// Load module
	HINSTANCE hInst = ::GetModuleHandle(lpszModule);
	if (hInst)
	{
		// Find the resource
		HRSRC hResource = ::FindResource(hInst, lpszResourceName, lpszResourceType);
		if (hResource)
		{
			// Load the resource
			HGLOBAL hHandle = ::LoadResource(hInst, hResource); 
			if (hHandle)
			{
				LPVOID lpResource = ::LockResource(hHandle);
				if (lpResource)
				{
					nLength = ::SizeofResource(hInst, hResource);
					if (ptrBuffer && nLength)
						memcpy(ptrBuffer, lpResource, nLength);

					UnlockResource(lpResource);					
				}
				::FreeResource(hHandle);				
			}
		}		
	}	

	return nLength;
}

BOOL	SaveResource(LPCTSTR lpszModule, LPCTSTR lpszResourceName, LPCTSTR lpszResourceType, LPCTSTR lpszDestFile)
{
	BOOL bSaveOK = FALSE;

	// Load module
	HINSTANCE hInst = ::GetModuleHandle(lpszModule);
	if (hInst)
	{
		// Find the resource
		HRSRC hResource = ::FindResource(hInst, lpszResourceName, lpszResourceType);
		if (hResource)
		{
			// Load the resource
			HGLOBAL hHandle = ::LoadResource(hInst, hResource); 
			if (hHandle)
			{
				LPVOID lpResource = ::LockResource(hHandle);
				if (lpResource)
				{
					int nLength = ::SizeofResource(hInst, hResource);
					if (nLength)
					{
						HANDLE hFile = ::CreateFile(lpszDestFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
						if (hFile != INVALID_HANDLE_VALUE)
						{
							DWORD dwWritten = 0;
							::WriteFile(hFile, lpResource, nLength, &dwWritten, NULL);
							::CloseHandle(hFile);
							bSaveOK = (dwWritten == nLength);
						}
					}
					
					UnlockResource(lpResource);					
				}

				::FreeResource(hHandle);				
			}
		}		
	}	

	return bSaveOK;
}

/// Create application data path
BOOL GetSystemPathA(CHAR* szAppPath, UINT csidl, LPCSTR lpszCreateSubFolder)
{
	CHAR szPath[520] = { 0 };

	LPITEMIDLIST	pidl;
	LPMALLOC		pShellMalloc;		
	if(SUCCEEDED(SHGetMalloc(&pShellMalloc)))
	{
		if(SUCCEEDED(SHGetSpecialFolderLocation(NULL, csidl, &pidl)))
		{
			memset(szPath, 0, sizeof(szPath));
			SHGetPathFromIDListA(pidl, szPath);
			pShellMalloc->Free(pidl);

			int nLength = strlen(szPath);
			if (szPath[nLength-1] != '\\')
			{
				szPath[nLength] = '\\';
				nLength++;
			}

			if (lpszCreateSubFolder)
			{
				strcat(szPath, lpszCreateSubFolder);
				strcat(szPath, "\\");
				if (!PathFileExistsA(szPath))
					CreateDirectoryA(szPath, NULL);		
			}				
		}

		pShellMalloc->Release();
	}

	strcpy(szAppPath, szPath);

	return strlen(szAppPath) > 0;
}

BOOL GetSystemPathW(WCHAR* szAppPath, UINT csidl, LPCWSTR lpszCreateSubFolder)
{
	WCHAR szPath[520] = { 0 };

	LPITEMIDLIST	pidl;
	LPMALLOC		pShellMalloc;		
	if(SUCCEEDED(SHGetMalloc(&pShellMalloc)))
	{
		if(SUCCEEDED(SHGetSpecialFolderLocation(NULL, csidl, &pidl)))
		{
			memset(szPath, 0, sizeof(szPath));
			SHGetPathFromIDListW(pidl, szPath);
			pShellMalloc->Free(pidl);

			int nLength = wcslen(szPath);
			if (szPath[nLength-1] != _T('\\'))
			{
				szPath[nLength] = _T('\\');
				nLength++;
			}

			if (lpszCreateSubFolder)
			{
				wcscat(szPath, lpszCreateSubFolder);
				wcscat(szPath, _T("\\"));
				if (!PathFileExistsW(szPath))
					CreateDirectoryW(szPath, NULL);	
			}	
		}

		pShellMalloc->Release();
	}

	wcscpy(szAppPath, szPath);

	return wcslen(szAppPath) > 0;
}

//==================================================================
/// Debug print
void	DbgPrintA(const CHAR* ptrFormat, ...)
{
	assert(ptrFormat != NULL);
	
	va_list argList;
	va_start( argList, ptrFormat );
	int ilen = 1024*1024;
	char* output = new char[ilen];
	output[0] = 0;
	vsprintf(output, ptrFormat, argList);
	va_end( argList );

	OutputDebugStringA(output);
	delete []output;
}

VOID	DbgPrintW(const WCHAR* ptrFormat, ...)
{
	assert(ptrFormat != NULL);

	va_list argList;
	va_start( argList, ptrFormat );
	int ilen = 1024*1024;
	WCHAR* output = new WCHAR[ilen];
	ZeroMemory(output , ilen);
	vswprintf(output, ptrFormat, argList);
	va_end( argList );

	OutputDebugStringW(output);
	delete []output;
}

//去除字符串前后空格
string RemovePreAndLastSpaceA(const string& str)    
{    
        
    int length = str.size();    
    int i = 0,j = length -1;    
    while(i < length && isspace(str[i])){i++;}    
    
    while(j >= 0 && iswspace(str[j])){j--;}    
	DbgPrintA("%d:%d", i , j);    
    
    if(j<i) return string();    
    
    return str.substr(i,j-i+1);    
} 
    
wstring RemovePreAndLastSpaceW(const wstring& str)    
{    
        
    int length = str.size();    
    int i = 0,j = length -1;    
    while(i < length && iswspace(str[i])){i++;}    
    
    while(j >= 0 && iswspace(str[j])){j--;}    
	DbgPrintW(L"%d:%d", i , j);    
    
    if(j<i) return wstring();    
    
    return str.substr(i,j-i+1);    
} 

/* ************************************ 
 * BOOL GetDiskSpaceInfo(LPCSTR pszDrive 
 * 功能   根据输入的驱动器，获取磁盘总容量 
 *          空闲空间、簇数量等磁盘信息 
 * 参数   驱动器根路径，比如“D:\”。 
 **************************************/  
DWORD64 GetDiskSpaceInfo(LPCTSTR pszDrive)  
{  
    DWORD64 qwFreeBytesToCaller, qwTotalBytes, qwFreeBytes = 0;  
    DWORD dwSectPerClust, dwBytesPerSect, dwFreeClusters,  dwTotalClusters;  
    BOOL bResult;  
      
    //使用GetDiskFreeSpaceEx获取磁盘信息并打印结果  
    bResult = GetDiskFreeSpaceEx (pszDrive,  
        (PULARGE_INTEGER)&qwFreeBytesToCaller,  
        (PULARGE_INTEGER)&qwTotalBytes,  
        (PULARGE_INTEGER)&qwFreeBytes);   
   
    return qwFreeBytes;  
} 

void Trim(CStdString & s) 
{
	if (s.IsEmpty()) 
	{
		return ;
	}

	wstring str = s.GetData();

	str.erase(0,str.find_first_not_of(_T(" ")));
	str.erase(str.find_last_not_of(_T(" ")) + 1);

 	s.Format(_T("%s"),str.c_str());
}

void Trim(CStdString & s ,  TCHAR* pTrim) 
{
	if (s.IsEmpty()) 
	{
		return ;
	}




	wstring str = s.GetData();

	str.erase(0,str.find_first_not_of(pTrim));
	str.erase(str.find_last_not_of(pTrim) + 1);

	s.Format(_T("%s"),str.c_str());
}

BOOL m_bUserTerminate = FALSE;
HANDLE m_hMpg123Handle = 0;
int	CallExe(LPCWSTR lptszExeName , LPCWSTR lptszParam ,   LPCWSTR lpwszCurdir  , bool bAsync , bool bEnableForceTerminate)
{
	std::wstring strExcute ;

// 	strExcute = m_csToolsPath ; 
// 	strExcute += _T("\\");
	strExcute += lptszExeName;


	std::wstring strCmd = L"";
	strCmd += lptszParam;
	// 	strCmd = strExcute;
	// 	strCmd += lptszParam;
	//	strCmd = lptszParam;

	STARTUPINFO rsyncStartInfo;
	rsyncStartInfo.wShowWindow = SW_MAXIMIZE;
	ZeroMemory(&rsyncStartInfo, sizeof(STARTUPINFO));

	rsyncStartInfo.cb = sizeof(STARTUPINFO);
	rsyncStartInfo.cbReserved2 = NULL;
	rsyncStartInfo.lpDesktop = NULL;

#ifdef _DEBUG
	rsyncStartInfo.wShowWindow = /*SW_HIDE*/SW_SHOW;
#else
	rsyncStartInfo.wShowWindow = SW_HIDE;
#endif

	rsyncStartInfo.dwFlags = STARTF_USESHOWWINDOW;

	//STARTUPINFOW si;

	PROCESS_INFORMATION pi;	
	ZeroMemory( &pi, sizeof(pi) );	
	//ZeroMemory( &si, sizeof(si) );

	//si.cb = sizeof(si);

	// Start the child process
	unsigned long ret = 0;
	strExcute += strCmd;
	//if(CreateProcessW(strExcute.c_str(), (wchar_t*)strCmd.c_str(), NULL, NULL, FALSE, 0, NULL , lpwszCurdir, &rsyncStartInfo, &pi))		
	
	if(CreateProcessW(NULL, (wchar_t*)strExcute.c_str(), NULL, NULL, FALSE, 0, NULL , lpwszCurdir, &rsyncStartInfo, &pi))			
	{
		DbgPrintW(_T("CreateProcessW %s") , strExcute.c_str());
		m_hMpg123Handle = pi.hProcess;
		
		if(!bAsync)
		{			
			if (bEnableForceTerminate == true)
			{
				while(WaitForSingleObject(pi.hThread , 200)!=WAIT_OBJECT_0)
				{
					
					if (m_bUserTerminate)
					{
						break;
					}
				}

				if (m_bUserTerminate)
				{
					
					TerminateProcess(pi.hProcess  , 0xff);
					ret = 0xff;
				}
				else
				{
					
					GetExitCodeProcess(pi.hProcess,&ret);
					CloseHandle( pi.hProcess );		
					CloseHandle( pi.hThread );
				}
			}
			else
			{
				
				WaitForSingleObject(pi.hThread , INFINITE);
				GetExitCodeProcess(pi.hProcess,&ret);
				CloseHandle( pi.hProcess );		
				CloseHandle( pi.hThread );
			}
		}		

	}

	
	return ret;
}