#pragma once

class CDragOptionUI: public COptionUI
{
public:
	CDragOptionUI(CPaintManagerUI& paint_manager);
	~CDragOptionUI();

	virtual LPCTSTR GetClass() const;

	virtual void Notify(TNotifyUI& msg);

	virtual void DoEvent(TEventUI& event);

	virtual void DoPostPaint(HDC hDC, const RECT& rcPaint);

	RECT GetMousePos();

protected:
	CPaintManagerUI& paint_manager_;
	POINT m_ptLastMouse;
	RECT  m_rcNewPos;
};