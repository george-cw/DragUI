#include "stdafx.h"
#include "WinBase.hpp"
#include "MainFrameUI.h"
#include "ControlEx.h"
#include "DragOptionUI.h"

#pragma warning(disable:4996)

CMainFrameUI::CMainFrameUI()
{

}

CMainFrameUI::~CMainFrameUI()
{

}

LPCTSTR CMainFrameUI::GetWindowClassName() const
{
	return _T("CActionsDragUI");
}

CString CMainFrameUI::GetSkinFile()
{
	return _T("Skin.xml");
}

CString CMainFrameUI::GetSkinFolder()
{
	return CString(CPaintManagerUI::GetInstancePath()) + _T("skin\\");
}

void CMainFrameUI::Init()
{
	m_pMainDragLayout = static_cast<CVerticalLayoutUI*>(paint_manager_.FindControl(_T("main_drag")));
}
CControlUI* CMainFrameUI::CreateControl(LPCTSTR pstrClass)
{
	if( _tcscmp(pstrClass, _T("ComputerExamine")) == 0 ) 
		return new ComputerExamineUI;
	else if (_tcscmp(pstrClass, _T("DragOption")) == 0)
		return new CDragOptionUI(paint_manager_);

	return NULL;
}

void CMainFrameUI::Notify(TNotifyUI& msg)
{
	if (_tcsicmp(msg.sType, _T("windowinit")) == 0)
	{
		
	}
	else if (_tcsicmp(msg.sType, _T("timer")) == 0)
	{
		
	}

	else if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
		if (_tcsicmp(msg.pSender->GetName(), _T("Quit")) == 0)
		{
			//Close();
			PostQuitMessage(0);
		}
		else if (_tcsicmp(msg.pSender->GetName(), _T("maxbtn")) == 0)
		{
			::SendMessage(m_hWnd,WM_SYSCOMMAND, SC_MAXIMIZE, 0); 
		}
		else if (_tcsicmp(msg.pSender->GetName(), _T("restorebtn")) == 0)
		{
			::SendMessage(m_hWnd,WM_SYSCOMMAND, SC_RESTORE, 0); 
		}
		else if (_tcsicmp(msg.pSender->GetName(), _T("minimum")) == 0)
		{
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		}
	}
	else if(msg.sType==_T("selectchanged"))
	{
		CStdString name = msg.pSender->GetName();
		CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(paint_manager_.FindControl(_T("switch")));
		if(name==_T("item1"))
			pControl->SelectItem(0);
		else if(name==_T("item2"))
			pControl->SelectItem(1);
		else if(name==_T("item3"))
			pControl->SelectItem(2);
	}
	else if (msg.sType == DUI_DRAG_CLICK)
	{
		CDragOptionUI *pOption = (CDragOptionUI*)msg.pSender;
		if (_tcscmp(pOption->GetGroup(), _T("test")) == 0 &&  m_pMainDragLayout)
		{
			m_pMainDragLayout->SetAttribute(_T("bordersize"), _T("1"));
			m_pMainDragLayout->SetAttribute(_T("bordercolor"), _T("#ffff0000"));
		}
	}
	else if (msg.sType == DUI_DRAG_RELEASE)
	{
		CDragOptionUI *pOption = (CDragOptionUI*)msg.pSender;
		RECT child = pOption->GetMousePos();
		RECT parent = m_pMainDragLayout->GetPos();
		if (_tcscmp(pOption->GetGroup(), _T("test")) == 0 && m_pMainDragLayout)
		{
			m_pMainDragLayout->SetAttribute(_T("bordersize"), _T("0"));
			m_pMainDragLayout->SetAttribute(_T("bordercolor"), _T("#ffff0000"));
			if (InBox(parent, child) && pOption->GetName().Compare(_T("item1")) == 0)
			{
				OutputDebugStringA("inBox");
				ComputerExamineUI *item1 = new ComputerExamineUI;
				item1->SetFixedHeight(300);
				m_pMainDragLayout->Add(item1);
				SIZE size;
				size.cx = 0;
				size = m_pMainDragLayout->GetScrollRange();
				size.cy += m_pMainDragLayout->GetFixedHeight()+300;
				m_pMainDragLayout->Invalidate();
				CStdString cs = _T("");
				cs.Format(_T("size cy: %d"), size.cy);
				OutputDebugString(cs);
			}
		}
	}
}

bool CMainFrameUI::InBox(RECT parent, RECT child)
{
	if (child.left > parent.right || child.right < parent.left || child.top > parent.bottom || child.bottom < parent.top)
		return false;
	return true;
}