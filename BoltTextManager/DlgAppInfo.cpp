#include "stdafx.h"
#include "MFCUI.h"
#include "DlgAppInfo.h"
#include "afxdialogex.h"
//#include "common/ConfigManager.h"

IMPLEMENT_DYNAMIC(CDlgAppInfo, CDialog)

CDlgAppInfo::CDlgAppInfo(CWnd* pParent /*=NULL*/)
	: CImgDialogBase(CDlgAppInfo::IDD
	, IDB_IMAGE_INFO
	, _T("PNG")
	, AfxGetResourceHandle()
	, pParent)	
{
}

CDlgAppInfo::~CDlgAppInfo()
{
}

void CDlgAppInfo::DoDataExchange(CDataExchange* pDX)
{
	CImgDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_INFO, m_static_info);
}


BEGIN_MESSAGE_MAP(CDlgAppInfo, CImgDialogBase)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


BOOL CDlgAppInfo::OnInitDialog()
{
	CImgDialogBase::OnInitDialog();
	CenterWindow();
	CRect rcWindow;
	GetClientRect(&rcWindow);
	m_fedit.CreateFont(17, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("맑은 고딕"));

	m_static_info.MoveWindow(rcWindow.left + 50, rcWindow.top + 55, rcWindow.right - 85, rcWindow.bottom - 140);
	m_static_info.SetFont(&m_fedit);
	
	CString desc = _T("Bolt Text Manager 2\n\nauthor :\n  keidongwon@gmail.com");
	
	m_static_info.SetWindowText(desc);

	rcConfirm.SetRect(rcWindow.right-65, rcWindow.bottom-60, rcWindow.right-35, rcWindow.bottom-30);

	return TRUE;
}

void CDlgAppInfo::OnPaint() 
{
	CPaintDC dc(this);
}

HBRUSH CDlgAppInfo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	COLORREF mainbg = RGB(0, 0, 0);
	HBRUSH brush = CreateSolidBrush(mainbg);

	switch(nCtlColor)
	{
	case CTLCOLOR_EDIT:
	case CTLCOLOR_DLG:
	case CTLCOLOR_BTN:
	case CTLCOLOR_LISTBOX:
	case CTLCOLOR_STATIC:
		pDC->SetTextColor(RGB(192, 192, 192));
		pDC->SetBkColor(mainbg);
		return brush;
		break;
	default:
		return brush;
		break;
	}
	return brush;
}

void CDlgAppInfo::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (IsPosition(rcConfirm, point))
	{
		OnOK();
	}
	CImgDialogBase::OnLButtonDown(nFlags, point);
}

