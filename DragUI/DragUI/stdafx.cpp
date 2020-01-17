// stdafx.cpp : source file that includes just the standard includes
// DragUI.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

CIniManagerApp	CIniManagerApp::s_Manager;

//////////////////////////////////////////////////////////////////////////
/// Font manager
BOOL	AddCustomFont(CPaintManagerUI& pm, LPCTSTR lpszFontXmlFile)
{
	//if (!IsFileExist(lpszFontXmlFile))
	//	return FALSE;

	// 获取系统当前语言
	LANGID  LangID  = GetUserDefaultUILanguage();
	TCHAR	szLang[8] = { 0 };
	if(LangID == 0x804) 
		_tcscpy(szLang, _T("L804"));
	else if(LangID == 0x404 || LangID == 0xC04 || LangID == 0x1004) 
		_tcscpy(szLang, _T("L404"));
	else
		_stprintf(szLang, _T("L4%02x"), LangID & 0xFF);

	CMarkup xml;
	if (xml.LoadFromFile(lpszFontXmlFile))
	{		
		CMarkupNode root = xml.GetRoot();
		if (root.IsValid())
		{
			CMarkupNode font = root.GetChild(szLang);
			if (!font.IsValid())
				font = root.GetChild();// default english

			if (font.IsValid())
			{
				int nIndex = 0;
				CMarkupNode node = font.GetChild();
				while(node.IsValid())
				{
					pm.AddFontAt(nIndex, 
						node.GetAttributeValue(_T("name")),			// name
						_ttoi(node.GetAttributeValue(_T("size"))),	// size
						_tcsicmp(node.GetAttributeValue(_T("bold")), _T("true")) == 0,	// bold
						false,														// underline
						_tcsicmp(node.GetAttributeValue(_T("italic")), _T("true")) == 0);		// italic
					nIndex++;

					node = node.GetSibling();
				}

				return TRUE;
			}
		}		
	}	

	return FALSE;
}