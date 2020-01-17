#ifndef WIN_IMPL_BASE_HPP
#define WIN_IMPL_BASE_HPP

namespace DuiLib {
	class CWindowWnd;
	class INotifyUI;
	class IMessageFilterUI;
	class IDialogBuilderCallback;
}

class CWindowBase : public CWindowWnd, public INotifyUI, public IMessageFilterUI, public IDialogBuilderCallback
{
public:
	CWindowBase();
	virtual ~CWindowBase();

	virtual void OnFinalMessage(HWND hWnd);

	virtual UINT GetClassStyle() const;

	virtual void Init();

	virtual CControlUI* CreateControl(LPCTSTR pstrClass);

	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);	

	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

protected:
	virtual CString GetSkinFolder();
	virtual CString GetSkinFile() = 0;

	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

protected:
	CPaintManagerUI		paint_manager_;
};


//////////////////////////////////////////////////////////////////////////
/// CDialogUI base class
class CDialogUI : public CWindowWnd, public INotifyUI, public IMessageFilterUI
{
public:
	CDialogUI() { };

	UINT	GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };

	virtual	LPCTSTR	GetWindowClassName() const  = 0;
	virtual LPCTSTR	GetSkinFile() const = 0;

	virtual	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual	void	Init(){ };
	virtual	void	OnFinalMessage(HWND hWnd);

	virtual	void	Notify(TNotifyUI& msg){ };	

	virtual	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {	return 0;};
	virtual	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled){	return 0;};

	virtual	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

	CPaintManagerUI m_pm;
};




#endif // WIN_IMPL_BASE_HPP