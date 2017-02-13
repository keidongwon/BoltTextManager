#include "stdafx.h"
#include "MFCUI.h"
#include "DlgConfig.h"
#include "afxdialogex.h"
#include "common/ConfigManager.h"

IMPLEMENT_DYNAMIC(CDlgConfig, CDialogEx)

CDlgConfig::CDlgConfig(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgConfig::IDD, pParent)
{
	dlgConfigDefault = NULL;
}

CDlgConfig::~CDlgConfig()
{
	if (dlgConfigDefault) delete dlgConfigDefault;
}

void CDlgConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CONFIG, m_list_config);
}

BEGIN_MESSAGE_MAP(CDlgConfig, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_NOTIFY(NM_CLICK, IDC_LIST_CONFIG, &CDlgConfig::OnNMClickListConfig)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CONFIG, &CDlgConfig::OnLvnItemchangedListConfig)
	ON_BN_CLICKED(IDOK, &CDlgConfig::OnBnClickedOk)
END_MESSAGE_MAP()


BOOL CDlgConfig::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED);

	MoveWindow(0, 0, 650, 500, TRUE);
	CenterWindow();

	SetWindowTransparent(this, config.GetTransparentDlgConfig());

	m_fedit.CreateFont(17, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Tahoma"));

	CImageList gapImage;
    gapImage.Create(1, 23, ILC_COLORDDB, 1, 0);
    m_list_config.SetImageList(&gapImage, LVSIL_SMALL);
	m_list_config.SetFont(&m_fedit);

	config.Load();

	CRect rcChildWindow;
	GetClientRect(&rcChildWindow);
	rcChildWindow.left = rcChildWindow.left + 210;
	rcChildWindow.top = rcChildWindow.top + 30;
	rcChildWindow.right = rcChildWindow.right - 30;
	rcChildWindow.bottom = rcChildWindow.bottom - 60;
	dlgConfigDefault = new CDlgConfigDefault(this);
	dlgConfigDefault->Create(rcChildWindow, IDD_DLG_CONFIG_DEFAULT, this);
	dlgConfigDefault->ShowWindow(SW_SHOW);

	int row = 0;
	m_list_config.MoveWindow(40, 35, 130, 400);
	m_list_config.InsertItem(row, _T("Default")); row++;

	m_list_config.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
	m_list_config.EnsureVisible(0, false);
	m_list_config.SetFocus();

	CRgn m_rgnWnd;
	CRect rect;
    GetWindowRect(&rect);
    CRgn  region;
    if ( (HRGN)m_rgnWnd != NULL )
       m_rgnWnd.DeleteObject();
    m_rgnWnd.CreateRoundRectRgn(0, 0, rect.Width()+1, rect.Height()+1, 15, 15);
    SetWindowRgn((HRGN)m_rgnWnd, TRUE); 

	return TRUE;
}

HBRUSH CDlgConfig::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	COLORREF mainbg = RGB(225, 237, 251);
	COLORREF editbg = RGB(193, 219, 247);
	HBRUSH brush = CreateSolidBrush(mainbg);

	switch(nCtlColor)
	{
	case CTLCOLOR_EDIT:
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkColor(editbg);
		return CreateSolidBrush(editbg);
		break;
	case CTLCOLOR_STATIC:
		pDC->SetTextColor(RGB(64, 64, 64));
		pDC->SetBkColor(mainbg);
		return brush;
		break;
	case CTLCOLOR_DLG:
	case CTLCOLOR_BTN:
	default:
		return brush;
		break;
	}
	return brush;
}

void CDlgConfig::OnPaint()
{
	CPaintDC dc(this);
	
	CRect rcWindow;
	GetClientRect(&rcWindow);
	DrawWindowRect(dc, rcWindow, RGB(154, 193, 241));

	HBRUSH *pOldBrush;
	HBRUSH brush = CreateSolidBrush(RGB(237, 248, 252));
	CPen pen,*pOldPen;
	pen.CreatePen(PS_SOLID, 1, RGB(154, 193, 241));
	pOldBrush = (HBRUSH*)dc.SelectObject(brush);
	pOldPen = (CPen*)dc.SelectObject(&pen);
	dc.RoundRect(rcWindow.left+200, rcWindow.top+20, rcWindow.right-20, rcWindow.bottom-50, 20, 20);
	dc.SelectObject(pOldBrush);
	DeleteObject(brush);
	dc.RoundRect(rcWindow.left+20, rcWindow.top+20, rcWindow.left+180, rcWindow.bottom-50, 20, 20);

	dc.MoveTo(0, 6);
	dc.LineTo(6, 0);
	dc.MoveTo(0, rcWindow.bottom - 7);
	dc.LineTo(7, rcWindow.bottom);
	dc.MoveTo(rcWindow.right - 7, 0);
	dc.LineTo(rcWindow.right, 7);
	dc.MoveTo(rcWindow.right - 8, rcWindow.bottom);
	dc.LineTo(rcWindow.right, rcWindow.bottom - 8);

	dc.SelectObject(pOldPen);
    DeleteObject(pen);
}

void CDlgConfig::OnLButtonDown(UINT nFlags, CPoint point)
{
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CDlgConfig::OnMouseMove(UINT nFlags, CPoint point)
{
	CDialogEx::OnMouseMove(nFlags, point);
}

inline void CDlgConfig::SetDialog(CString item)
{
	if (item == _T("Default"))
	{
		if (dlgConfigDefault) dlgConfigDefault->ShowWindow(SW_SHOW);
	}
}

void CDlgConfig::OnNMClickListConfig(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CString item = m_list_config.GetItemText(pNMItemActivate->iItem, 0);
	SetDialog(item);
	*pResult = 0;
}

void CDlgConfig::OnLvnItemchangedListConfig(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if (pNMLV->uNewState == 3) 
	{
		CString item = m_list_config.GetItemText(pNMLV->iItem, 0);
		SetDialog(item);
	}
	*pResult = 0;
}

void CDlgConfig::OnBnClickedOk()
{
	dlgConfigDefault->Update();
	config.Save();

	CDialogEx::OnOK();
}
