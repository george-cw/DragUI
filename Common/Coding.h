////////////////////////////////////////////////////////
#ifndef __CODING_H__
#define __CODING_H__

#ifdef _MSC_VER
#pragma once
#endif

namespace Coding {

	#pragma warning(disable: 4996)

	LPSTR  WideToAnsi(LPCWSTR str, int len = -1);
	LPSTR  WideToUtf8(LPCWSTR str, int len = -1);
	LPWSTR WideStrDup(LPCWSTR str, int len = -1);

	LPWSTR AnsiToWide(LPCSTR  str, int len = -1);
	LPSTR  AnsiToUtf8(LPCSTR  str, int len = -1);
	LPSTR  AnsiStrDup(LPCSTR  str, int len = -1);

	LPWSTR Utf8ToWide(LPCSTR  str, int len = -1);
	LPSTR  Utf8ToAnsi(LPCSTR  str, int len = -1);
	static __inline LPSTR Utf8StrDup(LPCSTR str, int len = -1)    { return AnsiStrDup(str, len); }

	//StringAlloc takes the number of characters, not bytes!
	LPWSTR StringAllocW(size_t len);
	LPSTR  StringAllocA(size_t len);

	void   StringFree(LPWSTR str);
	void   StringFree(LPSTR str);

	#define StringFreeW StringFree
	#define StringFreeA StringFree

#ifdef _UNICODE
	static __inline LPTSTR WideToTchar(LPCWSTR str, int len = -1) { return WideStrDup(str, len);  }
	static __inline LPTSTR AnsiToTchar(LPCSTR  str, int len = -1) { return AnsiToWide(str, len);  }
	static __inline LPTSTR Utf8ToTchar(LPCSTR  str, int len = -1) { return Utf8ToWide(str, len);  }
	static __inline LPWSTR TcharToWide(LPCTSTR str, int len = -1) { return WideStrDup(str, len);  }
	static __inline LPSTR  TcharToAnsi(LPCTSTR str, int len = -1) { return WideToAnsi(str, len);  }
	static __inline LPSTR  TcharToUtf8(LPCTSTR str, int len = -1) { return WideToUtf8(str, len);  }
	static __inline LPTSTR TcharStrDup(LPCTSTR str, int len = -1) { return WideStrDup(str, len);  }
	static __inline LPTSTR StringAlloc(size_t  len)               { return StringAllocW(len); }
#else
	static __inline LPTSTR WideToTchar(LPCWSTR str, int len = -1) { return WideToAnsi(str, len);  }
	static __inline LPTSTR AnsiToTchar(LPCSTR  str, int len = -1) { return AnsiStrDup(str, len);  }
	static __inline LPTSTR Utf8ToTchar(LPCSTR  str, int len = -1) { return Utf8ToAnsi(str, len);  }
	static __inline LPWSTR TcharToWide(LPCTSTR str, int len = -1) { return AnsiToWide(str, len);  }
	static __inline LPSTR  TcharToAnsi(LPCTSTR str, int len = -1) { return AnsiStrDup(str, len);  }
	static __inline LPSTR  TcharToUtf8(LPCTSTR str, int len = -1) { return AnsiToUtf8(str, len);  }
	static __inline LPTSTR TcharStrDup(LPCTSTR str, int len = -1) { return AnsiStrDup(str, len);  }
	static __inline LPTSTR StringAlloc(size_t  len)               { return StringAllocA(len); }
#endif

	#pragma warning(default: 4996)

} // namespace Coding

#endif //__CODING_H__
