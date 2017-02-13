#pragma once

class CDlgMain;
class CMainFrame : public CFrameWnd
{
protected:
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CDlgMain *dlgMain;
	HICON hIcon;

public:
	void CreateDlgMain(CWnd *wnd);
	void ShowDlgMain();
	void ResetDlgMain();
	int GetSelectedCount_ListMain();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnQueryEndSession();

public:
	afx_msg void OnListMainEncoding();
	afx_msg void OnListMainEncodingUpdate(CCmdUI* pCmdUI);
	afx_msg void OnListMainRemove();
	afx_msg void OnListMainRemoveUpdate(CCmdUI* pCmdUI);
	afx_msg void OnListMainOpenEditor();
	afx_msg void OnListMainOpenEditorUpdate(CCmdUI* pCmdUI);
};
