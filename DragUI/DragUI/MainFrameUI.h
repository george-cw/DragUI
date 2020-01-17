#include <shlobj.h>
#include <string>
#include <map>
using namespace std;

class CWindowBase;
class CMainFrameUI: public CWindowBase
{
public:
	CMainFrameUI();
	~CMainFrameUI();

	LPCTSTR GetWindowClassName() const;	

	virtual void Init();

	virtual CString GetSkinFile();

	virtual CString GetSkinFolder();

	virtual CControlUI* CreateControl(LPCTSTR pstrClass);

	bool InBox(RECT parent, RECT child);

protected:	

	void	Notify(TNotifyUI& msg);

private:
	CVerticalLayoutUI *m_pMainDragLayout;
};