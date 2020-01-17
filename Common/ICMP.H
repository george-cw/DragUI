namespace ICMP
{
	#include <time.h>
	#include <winsock.h>
	#pragma comment(lib, "ws2_32.lib")

	#include <mmsystem.H>
	#pragma comment(lib,"winmm.lib")

	typedef struct tagIPINFO
	{
		u_char Ttl;				// Time To Live
		u_char Tos;				// Type Of Service
		u_char IPFlags;			// IP flags
		u_char OptSize;			// Size of options data
		u_char FAR *Options;	// Options data buffer
	}IPINFO, *PIPINFO;

	typedef struct tagICMPECHO
	{
		u_long Source;			// Source address
		u_long Status;			// IP status
		u_long RTTime;			// Round trip time in milliseconds
		u_short DataSize;		// Reply data size
		u_short Reserved;		// Unknown
		void FAR *pData;		// Reply data buffer
		IPINFO	ipInfo;			// Reply options
	}ICMPECHO, *PICMPECHO;

	// ICMP.DLL Export Function Pointers
	typedef HANDLE	(WINAPI *ICMP_CreateFile)(VOID);
	typedef BOOL	(WINAPI *ICMP_CloseHandle)(HANDLE);
	typedef DWORD	(WINAPI *ICMP_SendEcho)(HANDLE, DWORD, LPVOID, WORD, PIPINFO, LPVOID, DWORD, DWORD);

	BOOL Ping(char *host, int nTimeout = 100, int nCount = 1, int nPacketSize = 32)
	{			
		// Load the ICMP.DLL
		HINSTANCE hICMP = LoadLibrary(_T("ICMP.DLL"));
		if (hICMP == NULL)
		{
			DbgPrint(_T("Load ICMP.DLL failed!"));
			return TRUE;
		}
		
		ICMP_CreateFile		pfnCreateFile	= (ICMP_CreateFile)GetProcAddress(hICMP,"IcmpCreateFile");
		ICMP_CloseHandle	pfnCloseHandle	= (ICMP_CloseHandle)GetProcAddress(hICMP,"IcmpCloseHandle");
		ICMP_SendEcho		pfnSendEcho		= (ICMP_SendEcho)GetProcAddress(hICMP,"IcmpSendEcho");
		if (pfnCreateFile == NULL || pfnCloseHandle == NULL || pfnSendEcho == NULL)
		{
			DbgPrint(_T("Invalid ICMP.DLL"));
			FreeLibrary(hICMP);
			return TRUE;
		}
		
		// Init WinSock
		WSADATA wsaData;
		int nRet = WSAStartup(0x0101, &wsaData);
		if (nRet != 0 || wsaData.wVersion != 0x0101)
		{
			DbgPrint(_T("WSAStartup failed, %d, %x"), nRet, wsaData.wVersion);
			FreeLibrary(hICMP);
			WSACleanup();
			return TRUE;
		}

		// Use inet_addr() to determine if we're dealing with a name or an address
		struct in_addr iaHost;		// Internet address structure
		LPHOSTENT pHost = NULL;		// Pointer to host entry structure
		iaHost.s_addr = inet_addr(host);
		if (iaHost.s_addr == INADDR_NONE)
			pHost = gethostbyname(host);
		else
			pHost = gethostbyaddr((const char *)&iaHost, sizeof(struct in_addr), AF_INET);		
		if (pHost == NULL)
		{
			DbgPrintA("Invalid Host: %s", host);
			WSACleanup();
			FreeLibrary(hICMP);
			return TRUE;
		}

		// Copy the IP address
		DWORD* dwAddress = (DWORD *)(*pHost->h_addr_list);		

		// Get an ICMP echo request handle        
		HANDLE hFile = pfnCreateFile();

		//Set up the data which will be sent
		char* buffer = new char[nPacketSize];
		memset(buffer, 'E', nPacketSize);

		//Do the actual Ping
		int nReplySize = sizeof(ICMPECHO) + nPacketSize;
		unsigned char* reply = new unsigned char[nReplySize]; 
		// Reqest an ICMP echo
		ICMPECHO* echo = (ICMPECHO*)reply;

		IPINFO ipInfo;
		ipInfo.Ttl = 255;
		ipInfo.Tos = 0;
		ipInfo.IPFlags = 0;
		ipInfo.OptSize = 0;
		ipInfo.Options = NULL;

		int nReponse = 0;
		for (int i = 0; i < nCount; i++)
		{	
			DWORD dwRet = pfnSendEcho(hFile, *dwAddress, buffer, nPacketSize, &ipInfo, echo, nReplySize, nTimeout);
			if(dwRet == 0) 
			{
				DbgPrintA("Ping [%s] timeout", host);
			}
			else
			{
				nReponse++;
				break;
			}
		}
			
		delete [] reply;
		delete [] buffer;

		// Close the echo request file handle
		pfnCloseHandle(hFile);
		FreeLibrary(hICMP);

		WSACleanup();

		return nReponse > 0;
	}
};

using namespace ICMP;