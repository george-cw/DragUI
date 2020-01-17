// Copyright (c) 2010-2011, duilib develop team(www.duilib.com).
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or 
// without modification, are permitted provided that the 
// following conditions are met.
//
// Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above 
// copyright notice, this list of conditions and the following
// disclaimer in the documentation and/or other materials 
// provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
// CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
// INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#if defined(UILIB_EXPORTS)
#if defined(_MSC_VER)
#define UILIB_API __declspec(dllexport)
#else
#define UILIB_API 
#endif
#else
#if defined(_MSC_VER)
#define UILIB_API __declspec(dllimport)
#else
#define UILIB_API 
#endif
#endif

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <stddef.h>
#include <richedit.h>
#include <tchar.h>
#include <assert.h>
#include <crtdbg.h>
#include <malloc.h>

#include "UIBase.h"
#include "UIManager.h"
#include "UIDelegate.h"
#include "UIControl.h"
#include "UIContainer.h"
#include "UIList.h"
#include "UICombo.h"
#include "UICommonControls.h"
#include "UIActiveX.h"
#include "UIWebBrowser.h"
#include "UIRichEdit.h"

#include "UIMarkup.h"
#include "UIDlgBuilder.h"
#include "UIRender.h"

//#include "WebBrowserEventHandler.h"


//////////////////////////////////////////////////////////////////////////
//加载库
#ifndef UILIB_EXPORTS
#pragma message("----------------------------------------------------------------------------------------------")

#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib10ud.lib")
#		pragma message("动态连接库 <DuiLib10ud.lib> 连接的 <DuiLib10ud.dll> 被连接")	
#   else
#       pragma comment(lib, "DuiLib10d.lib")
#		pragma message("动态连接库 <DuiLib10d.lib> 连接的 <DuiLib10d.dll> 被连接")	
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib10u.lib")
#		pragma message("动态连接库 <DuiLib10u.lib> 连接的 <DuiLib10u.dll> 被连接")	
#   else
#       pragma comment(lib, "DuiLib10.lib")
#		pragma message("动态连接库 <DuiLib10.lib> 连接的 <DuiLib10.dll> 被连接")	
#   endif
#endif

#pragma message("----------------------------------------------------------------------------------------------")
#endif
