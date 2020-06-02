#include "stdafx.h"
#include "MFCUI.h"
#include "MFCUIDoc.h"
#include "MFCUIView.h"
#include "MainFrm.h"
#include "common/ConfigManager.h"
#include "common/CommonData.h"
#include "DlgAppInfo.h"
#include "DlgConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CommonData commonData;
ConfigManager config;


IMPLEMENT_DYNCREATE(CMFCUIView, CFormView)

BEGIN_MESSAGE_MAP(CMFCUIView, CFormView)
    ON_WM_CONTEXTMENU()
    ON_WM_RBUTTONUP()
    ON_WM_LBUTTONDOWN()
    ON_WM_CTLCOLOR()
    ON_WM_PAINT()
    ON_WM_SIZE()
    ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

CMFCUIView::CMFCUIView()
    : CFormView(CMFCUIView::IDD)
{
    bMaximize = FALSE;
    m_pMainFrame = NULL;
}

CMFCUIView::~CMFCUIView()
{
}

#ifdef _DEBUG
void CMFCUIView::AssertValid() const
{
    CFormView::AssertValid();
}

void CMFCUIView::Dump(CDumpContext& dc) const
{
    CFormView::Dump(dc);
}

CMFCUIDoc* CMFCUIView::GetDocument() const
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCUIDoc)));
    return (CMFCUIDoc*)m_pDocument;
}
#endif

void CMFCUIView::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
}

BOOL CMFCUIView::PreCreateWindow(CREATESTRUCT& cs)
{
    return CFormView::PreCreateWindow(cs);
}

BOOL CMFCUIView::PreTranslateMessage(MSG* pMsg)
{
    return CFormView::PreTranslateMessage(pMsg);
}

void CMFCUIView::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();
    ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED);
    GetParentFrame()->CenterWindow();

    SetWindowTextW(L"Bolt Text Manager 2.0");

    config.Load();

    GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T(""));

    m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
    m_pMainFrame->CreateDlgMain(this);
    m_pMainFrame->ShowDlgMain();
}

void CMFCUIView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
    ClientToScreen(&point);
    OnContextMenu(this, point);
}

void CMFCUIView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
}

HBRUSH CMFCUIView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

    switch (nCtlColor)
    {
    case CTLCOLOR_DLG:
        return (HBRUSH)GetStockObject(WHITE_BRUSH);
        break;
    case CTLCOLOR_STATIC:
        if (pWnd->m_hWnd == GetDlgItem(IDC_STATIC_STATUS)->m_hWnd)
        {
            SetTextColor(pDC->m_hDC, RGB(190, 45, 45));
        }
        SetBkColor(pDC->m_hDC, RGB(255, 255, 255));
        return (HBRUSH)GetStockObject(WHITE_BRUSH);
        break;
    }
    return hbr;
}

void CMFCUIView::OnSize(UINT nType, int cx, int cy)
{
    CFormView::OnSize(nType, cx, cy);
}

void CMFCUIView::OnPaint()
{
    CPaintDC dc(this);

    CRect rcWindow;
    GetClientRect(&rcWindow);
    Gdiplus::Graphics graphics(dc);

    // default
    graphics.DrawImage(commonData.m_bg_top, 0, 0, rcWindow.right + 3, commonData.m_bg_top.m_pBitmap->GetHeight());
    graphics.DrawImage(commonData.m_bg_bottom, 0, rcWindow.bottom - commonData.m_bg_bottom.m_pBitmap->GetHeight(), rcWindow.right + 3, commonData.m_bg_bottom.m_pBitmap->GetHeight());
    graphics.DrawImage(commonData.m_image_title, 10, 10, commonData.m_image_title.m_pBitmap->GetWidth(), commonData.m_image_title.m_pBitmap->GetHeight());

    // system menu
    int nSysMenuX = rcWindow.right - (25 * 3 + 10);
    int nSysMenuY = 5;
    int nSysMenuSpace = 5;
    CommonData::draw(graphics, commonData.m_icon_small, nSysMenuX, nSysMenuY, nSysMenuSpace, rcIconSmall);
    CommonData::draw(graphics, commonData.m_icon_max, nSysMenuX, nSysMenuY, nSysMenuSpace, rcIconMax);
    CommonData::draw(graphics, commonData.m_icon_exit, nSysMenuX, nSysMenuY, nSysMenuSpace, rcIconExit);

    int height = commonData.m_image_resize.m_pBitmap->GetHeight();
    graphics.DrawImage(commonData.m_image_resize,
        rcWindow.right - commonData.m_image_resize.m_pBitmap->GetWidth() - 3,
        rcWindow.bottom - height - 3,
        commonData.m_image_resize.m_pBitmap->GetWidth(), commonData.m_image_resize.m_pBitmap->GetHeight());
    rcImageResize.SetRect(rcWindow.right - commonData.m_image_resize.m_pBitmap->GetWidth() - 3,
        rcWindow.bottom - height - 3,
        rcWindow.right - commonData.m_image_resize.m_pBitmap->GetWidth() - 3 + commonData.m_image_resize.m_pBitmap->GetWidth(),
        rcWindow.bottom - height - 3 + commonData.m_image_resize.m_pBitmap->GetHeight());

    CPen pen, *pOldPen;
    pen.CreatePen(PS_SOLID, 1, RGB(180, 179, 179));
    pOldPen = (CPen *)dc.SelectObject(&pen);

    dc.MoveTo(0, 4);
    dc.LineTo(0, rcWindow.bottom - 4);
    dc.MoveTo(rcWindow.right - 1, 4);
    dc.LineTo(rcWindow.right - 1, rcWindow.bottom - 4);

    dc.MoveTo(3, 0);
    dc.LineTo(0, 3);
    dc.MoveTo(rcWindow.right - 3, 0);
    dc.LineTo(rcWindow.right, 4);
    dc.MoveTo(0, rcWindow.bottom - 3);
    dc.LineTo(3, rcWindow.bottom);
    dc.MoveTo(rcWindow.right - 3, rcWindow.bottom);
    dc.LineTo(rcWindow.right, rcWindow.bottom - 4);

    SelectObject(dc.m_hDC, pOldPen);
    DeleteObject(pen);

    // program 
    int nProgramX = 40;
    int nProgramY = 55;
    int nProgramSpace = -1;
    CommonData::draw(graphics, commonData.m_menu_encoding, nProgramX, nProgramY, nProgramSpace, rcEncodingTool);
    GetDlgItem(IDC_STATIC_STATUS)->MoveWindow(nProgramX + 20, nProgramY + 12, 200, 20);

    // menu
    int nLinkX = rcWindow.right - (32 * 2 + 35);
    int nLinkY = 50;
    int nLinkSpace = 10;
    CommonData::draw(graphics, commonData.m_icon_config, nLinkX, nLinkY, nLinkSpace, rcIconConfig);
    CommonData::draw(graphics, commonData.m_icon_info, nLinkX, nLinkY, nLinkSpace, rcIconInfo);
}

void CMFCUIView::OnMouseMove(UINT nFlags, CPoint point)
{
    if (IsPosition(rcIconSmall, point)) { SetCursor(LoadCursor(NULL, IDC_HAND)); }
    else if (IsPosition(rcIconMax, point)) { SetCursor(LoadCursor(NULL, IDC_HAND)); }
    else if (IsPosition(rcIconExit, point)) { SetCursor(LoadCursor(NULL, IDC_HAND)); }
    else if (IsPosition(rcImageResize, point)) { SetCursor(LoadCursor(NULL, IDC_HAND)); }
    else if (IsPosition(rcEncodingTool, point)) { SetCursor(LoadCursor(NULL, IDC_HAND)); }
    else if (IsPosition(rcIconConfig, point)) { SetCursor(LoadCursor(NULL, IDC_HAND)); }
    else if (IsPosition(rcIconInfo, point)) { SetCursor(LoadCursor(NULL, IDC_HAND)); }
    else { SetCursor(LoadCursor(NULL, IDC_ARROW)); }
    CFormView::OnMouseMove(nFlags, point);
}

void CMFCUIView::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (IsPosition(rcIconSmall, point))
    {
        GetParent()->ShowWindow(SW_MINIMIZE);
    }
    else if (IsPosition(rcIconMax, point))
    {
        if (bMaximize == FALSE)
        {
            bMaximize = TRUE;
            GetParent()->ShowWindow(SW_MAXIMIZE);
        }
        else
        {
            bMaximize = FALSE;
            GetParent()->ShowWindow(SW_NORMAL);
        }
        Invalidate();
    }
    else if (IsPosition(rcIconExit, point))
    {
        GetParent()->PostMessage(WM_CLOSE);
    }
    else if (IsPosition(rcImageResize, point))
    {
    }
    else if (IsPosition(rcIconConfig, point))
    {
        CDlgConfig dlgConfig;
        dlgConfig.DoModal();
    }
    else if (IsPosition(rcIconInfo, point))
    {
        CDlgAppInfo dlgAppInfo;
        dlgAppInfo.DoModal();
    }
    else
    {
        GetParentFrame()->PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
    }

    CFormView::OnLButtonDown(nFlags, point);
}

void CMFCUIView::SetStatus(CString status)
{
    GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(status);
}

void CMFCUIView::Exit()
{
    GetParent()->PostMessage(WM_CLOSE);
}
