#include "stdafx.h"
#include "DragOptionUI.h"
#include "..\..\include\gdi\GdiPlus.h"
#include "..\..\include\gdi\GdiPlusColor.h"
using namespace Gdiplus;

CDragOptionUI::CDragOptionUI(CPaintManagerUI& paint_manager):paint_manager_(paint_manager)
{

}

CDragOptionUI::~CDragOptionUI()
{

}

LPCTSTR CDragOptionUI::GetClass() const
{
	return _T("DragOptionUI");
}

void CDragOptionUI::DoEvent(TEventUI& event)
{
	if( event.Type == UIEVENT_BUTTONDOWN && IsEnabled() )
	{
		if( ::PtInRect(&m_rcItem, event.ptMouse) ) 
		{
			m_uButtonState |= UISTATE_PUSHED | UISTATE_CAPTURED;
			m_ptLastMouse = event.ptMouse;
			m_rcNewPos = m_rcItem;
 
			if( m_pManager )
				m_pManager->AddPostPaint(this);
			if( m_pManager )
				m_pManager->SendNotify(this, DUI_DRAG_CLICK);
			return;
		}
	}
	if( event.Type == UIEVENT_BUTTONUP )
	{
		if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) 
		{
			m_uButtonState &= ~(UISTATE_PUSHED | UISTATE_CAPTURED);
			CHorizontalLayoutUI* pParent = static_cast<CHorizontalLayoutUI*>(m_pParent);
			//pParent->NotifyDrag(this);    // NotifyDrag函数是CModulePaneConfigUI容器的函数，和拖拽的效果本身没关系
			if(  m_pManager ) 
			{
				m_pManager->RemovePostPaint(this);
				m_pManager->Invalidate(m_rcNewPos);
			}
			NeedParentUpdate();
			//if( ::PtInRect(&m_rcItem, event.ptMouse) )
					Activate();
			if( m_pManager )
				m_pManager->SendNotify(this, DUI_DRAG_RELEASE);
			return;
		}
	}
	if( event.Type == UIEVENT_MOUSEMOVE )
	{
		if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) 
		{
			LONG cx = event.ptMouse.x - m_ptLastMouse.x;
			LONG cy = event.ptMouse.y - m_ptLastMouse.y;
 
			m_ptLastMouse = event.ptMouse;
 
			RECT rcCurPos = m_rcNewPos;
 
			rcCurPos.left += cx;
			rcCurPos.right += cx;
			rcCurPos.top += cy;
			rcCurPos.bottom += cy;			
 
			//将当前拖拽块的位置 和 当前拖拽块的前一时刻的位置，刷新
			CDuiRect rcInvalidate = m_rcNewPos;
			m_rcNewPos = rcCurPos;
			rcInvalidate.Join(m_rcNewPos);

			if( m_pManager ) 
				m_pManager->Invalidate(rcInvalidate);
			if( m_pManager )
				m_pManager->SendNotify(this, DUI_DRAG_MOVE);
 
			return;
		}
	}
	if( event.Type == UIEVENT_SETCURSOR )
	{
		if( IsEnabled() ) 
		{
			::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
			return;
		}
	}
 
	COptionUI::DoEvent(event);
}

void CDragOptionUI::DoPostPaint(HDC hDC, const RECT& rcPaint)
{
	if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
		CDuiRect rcParent = m_pParent->GetPos();
		RECT rcUpdate ={0};
		rcUpdate.left = m_rcNewPos.left;//m_rcNewPos.left < rcParent.left ? rcParent.left : m_rcNewPos.left;
		rcUpdate.top = m_rcNewPos.top;//m_rcNewPos.top < rcParent.top ? rcParent.top : m_rcNewPos.top;
		rcUpdate.right = m_rcNewPos.right;//m_rcNewPos.right > rcParent.right ? rcParent.right : m_rcNewPos.right;
		rcUpdate.bottom = m_rcNewPos.bottom;//m_rcNewPos.bottom > rcParent.bottom ? rcParent.bottom : m_rcNewPos.bottom;

		DWORD color = GetTextColor();
		int alpha = (color & 0xff000000) >> 24;
		int red   = (color & 0x00ff0000) >> 16;
		int green = (color & 0x0000ff00) >> 8;
		int blue  = (color & 0x000000ff);
		Color acolor = Color(0xAA, red, green, blue);

		CRenderEngine::DrawTextW(hDC, m_pManager, rcUpdate, GetText(), acolor.GetValue(), GetFont(), GetTextStyle());
		CRenderEngine::DrawColor(hDC, rcUpdate, 0xAA000000);
	}
}

RECT CDragOptionUI::GetMousePos()
{
	return m_rcNewPos;
}


void CDragOptionUI::Notify(TNotifyUI& msg)
{
}

