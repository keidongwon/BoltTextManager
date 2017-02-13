#include "stdafx.h"
#include "MFCUI.h"
#include "MainFrm.h"
#include "DlgMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_LISTMAIN_ENCDOING, OnListMainEncoding)
	ON_COMMAND(ID_LISTMAIN_REMOVE, OnListMainRemove)
	ON_COMMAND(ID_LISTMAIN_OPEN_EDITOR, OnListMainOpenEditor)
	ON_UPDATE_COMMAND_UI(ID_LISTMAIN_ENCDOING, OnListMainEncodingUpdate)
	ON_UPDATE_COMMAND_UI(ID_LISTMAIN_REMOVE, OnListMainRemoveUpdate)
	ON_UPDATE_COMMAND_UI(ID_LISTMAIN_OPEN_EDITOR, OnListMainOpenEditorUpdate)
END_MESSAGE_MAP()

CMainFrame::CMainFrame()
{
	dlgMain = NULL;
}

CMainFrame::~CMainFrame()
{
	if (dlgMain) delete dlgMain;
}

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.cx = 800;
	cs.cy = 600;
	cs.style = WS_POPUP | WS_VISIBLE | WS_EX_TOOLWINDOW;
	cs.lpszName = _T("Bolt Text Manager");

	if (cs.hMenu != NULL) 
	{
		::DestroyMenu(cs.hMenu);
		cs.hMenu = NULL;
	}
	return TRUE;
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);
	CRect rcChildWindow;
	GetClientRect(&rcChildWindow);
	rcChildWindow.left = rcChildWindow.left + 10;
	rcChildWindow.top = rcChildWindow.top + 55 + 34;
	rcChildWindow.right = rcChildWindow.right - 10;
	rcChildWindow.bottom = rcChildWindow.bottom - 60;
	if (dlgMain) dlgMain->Resize(rcChildWindow);

	CRgn m_rgnWnd;
	CRect rect;
    GetWindowRect(&rect);
    CRgn  region;
    if ( (HRGN)m_rgnWnd != NULL )
       m_rgnWnd.DeleteObject();
    m_rgnWnd.CreateRoundRectRgn(0, 0, rect.Width()+1, rect.Height()+1, 5, 5);
    SetWindowRgn((HRGN)m_rgnWnd, TRUE); 
}

void CMainFrame::CreateDlgMain(CWnd *wnd)
{
	CRect rcChildWindow;
	GetClientRect(&rcChildWindow);
	rcChildWindow.left = rcChildWindow.left + 10;
	rcChildWindow.top = rcChildWindow.top + 55 + 34;
	rcChildWindow.right = rcChildWindow.right - 10;
	rcChildWindow.bottom = rcChildWindow.bottom - 60;
	dlgMain = new CDlgMain(wnd);
	dlgMain->Create(rcChildWindow, IDD_DLG_MAIN, wnd);
}

void CMainFrame::ShowDlgMain()
{
	if (dlgMain) dlgMain->ShowWindow(SW_SHOW);
}

int CMainFrame::GetSelectedCount_ListMain()
{
	if (dlgMain)
		return dlgMain->GetSelectedCount();
	return 0;
}

void CMainFrame::OnListMainEncodingUpdate(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetSelectedCount_ListMain() > 0);
}

void CMainFrame::OnListMainEncoding()
{
	if (GetSelectedCount_ListMain() > 0)
		dlgMain->EncodingFiles();
}

void CMainFrame::OnListMainRemoveUpdate(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetSelectedCount_ListMain() > 0);
}

void CMainFrame::OnListMainRemove()
{
	if (GetSelectedCount_ListMain() > 0)
		dlgMain->RemoveFiles();
}

void CMainFrame::OnListMainOpenEditorUpdate(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetSelectedCount_ListMain() > 0);
}

void CMainFrame::OnListMainOpenEditor()
{
	if (GetSelectedCount_ListMain() > 0)
		dlgMain->ExecuteFiles();
}

void CMainFrame::ResetDlgMain()
{
	if (dlgMain) dlgMain->Reset();
}

BOOL CMainFrame::OnQueryEndSession()
{
	PostQuitMessage(WM_QUIT);
	return TRUE;
}
