#pragma once
#include <map>

using namespace Gdiplus;

class CImgDialogBase;

typedef struct _tagCtrlInfo
{
	WNDPROC				pWndProc;
	CImgDialogBase *	pImgDlg;
} tagCtrlInfo, *LPCtrlInfo;

class CImgDialogBase : public CDialog
{
	DECLARE_DYNAMIC(CImgDialogBase)

public:
	CImgDialogBase( UINT nIDD
		, LPCTSTR lpszFile
		, CWnd* pParent = NULL
		);

	CImgDialogBase( UINT nIDD
		, UINT nImgID
		, LPCTSTR lpszType
		, HINSTANCE hResourceModule = NULL
		, CWnd* pParent = NULL
		);

	virtual ~CImgDialogBase();
	void Refresh();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnCancel();
	virtual void OnOK();
	virtual void PostNcDestroy();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnClose();

	virtual BOOL OnInitDialog();
	virtual void OnDraw(Graphics & graphics);

	void CreateFakeWnd(void);
	void DestoryFakeWnd(void);

	void DrawCtrl( Graphics & graphics, HDC hDC, HWND hWnd);
	void DrawCaret(Graphics & graph);
	HBITMAP GetWindowBitmap(HWND hWnd, HDC hDC);

	void EnableDrag(BOOL bEnabled);
	void HookControlUpdate(HWND hWnd);
	void UnhookControlUpdate(HWND hWnd);

	Image *					m_pImage;
	int						m_nWidth;
	int						m_nHeigh;
	int						m_nAlpha;
	HWND					m_hFakeWnd;
	BOOL					m_bEnableDrag;
	BOOL					m_bIsRefreshing;
	BOOL					m_bShown;
	CString					m_strWndClassName;

	static std::map<HWND, tagCtrlInfo>	s_mpCtrl;
	static UINT __cdecl ShowMotionThread( LPVOID pParam );
	static LRESULT CALLBACK CtrlWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
};
