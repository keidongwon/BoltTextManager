#include "stdafx.h"
#include "MFCUI.h"
#include "DlgConfigDefault.h"
#include "afxdialogex.h"
#include "common/CommonData.h"
#include "common/ConfigManager.h"
#include "MainFrm.h"

IMPLEMENT_DYNAMIC(CDlgConfigDefault, CDialogEx)

CDlgConfigDefault::CDlgConfigDefault(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgConfigDefault::IDD, pParent)
{
}

CDlgConfigDefault::~CDlgConfigDefault()
{
}

void CDlgConfigDefault::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_OPTION_NAME, m_edit_option_name);
	DDX_Control(pDX, IDC_EDIT_EDITOR_PATH, m_edit_editor_path);
	DDX_Control(pDX, IDC_EDIT_DEFAULT_ENCODING, m_edit_default_encoding);
}


BEGIN_MESSAGE_MAP(CDlgConfigDefault, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BTN_CONFIG_RESET, &CDlgConfigDefault::OnBnClickedBtnConfigReset)
	ON_BN_CLICKED(IDC_CHECK_OVERWRITE, &CDlgConfigDefault::OnBnClickedCheckOverwrite)
	ON_BN_CLICKED(IDC_CHECK_READONLY, &CDlgConfigDefault::OnBnClickedCheckReadonly)
	ON_BN_CLICKED(IDC_CHECK_CUSTOM_EDITOR, &CDlgConfigDefault::OnBnClickedCheckCustomEditor)
END_MESSAGE_MAP()


BOOL CDlgConfigDefault::PreTranslateMessage(MSG* pMsg) 
{ 
    switch (pMsg->wParam)
	{
    case VK_ESCAPE:
        return TRUE;

	case VK_RETURN:
		return TRUE;
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CDlgConfigDefault::Create(CRect& wndRect, UINT nIDTemplate, CWnd* pParentWnd)
{
	m_WindowRect.CopyRect(&wndRect);
	return CDialogEx::Create(nIDTemplate, pParentWnd);
}

BOOL CDlgConfigDefault::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED);
	MoveWindow(m_WindowRect.left, m_WindowRect.top,	m_WindowRect.right-m_WindowRect.left, m_WindowRect.bottom-m_WindowRect.top,	TRUE);

	EditTextControl(m_edit_option_name);
	EditTextControl(m_edit_editor_path);
	EditTextControl(m_edit_default_encoding);

	CheckDlgButton(IDC_CHECK_SUB_FOLDER, config.is_include_subfolder());

	CheckDlgButton(IDC_CHECK_READONLY, config.is_overwrite_readonly());
	CheckDlgButton(IDC_CHECK_OVERWRITE, config.is_overwrite_file());
	if (config.is_option_name_prefix())
	{
		CheckDlgButton(IDC_RADIO_HEAD, TRUE);
		CheckDlgButton(IDC_RADIO_TAIL, FALSE);
	}
	else
	{
		CheckDlgButton(IDC_RADIO_HEAD, FALSE);
		CheckDlgButton(IDC_RADIO_TAIL, TRUE);
	}
	GetDlgItem(IDC_EDIT_OPTION_NAME)->SetWindowTextW(config.get_option_name());

	CheckDlgButton(IDC_CHECK_CUSTOM_EDITOR, config.is_custom_editor());
	GetDlgItem(IDC_EDIT_EDITOR_PATH)->SetWindowTextW(config.get_editor_path());

	GetDlgItem(IDC_EDIT_DEFAULT_ENCODING)->SetWindowTextW(config.get_default_encoding());

	// Enable / Disable

	if (config.is_overwrite_file() == TRUE)
	{
		GetDlgItem(IDC_CHECK_READONLY)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_HEAD)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_TAIL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_OPTION_NAME)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_CHECK_READONLY)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_HEAD)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_TAIL)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_OPTION_NAME)->EnableWindow(TRUE);
	}

	if (config.is_custom_editor() == TRUE)
	{
		GetDlgItem(IDC_EDIT_EDITOR_PATH)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_EDITOR_PATH)->EnableWindow(FALSE);
	}

	return TRUE;
}

HBRUSH CDlgConfigDefault::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	COLORREF mainbg = RGB(237, 248, 252);
	COLORREF editbg = RGB(248, 251, 254);
	HBRUSH brush = CreateSolidBrush(mainbg);

	switch(nCtlColor)
	{
	case CTLCOLOR_EDIT:
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkColor(editbg);
		return CreateSolidBrush(editbg);
		break;
	case CTLCOLOR_DLG:
	case CTLCOLOR_BTN:
	case CTLCOLOR_LISTBOX:
	case CTLCOLOR_STATIC:
		pDC->SetTextColor(RGB(64, 64, 64));
		pDC->SetBkColor(mainbg);
		return brush;
		break;
	default:
		return brush;
		break;
	}
	return brush;
}

void CDlgConfigDefault::OnPaint()
{
	CPaintDC dc(this);
	
	CRect rcWindow;
	GetClientRect(&rcWindow);

	CRect rcBox;
	m_edit_option_name.GetWindowRect(&rcBox);
	ScreenToClient(&rcBox);
	DrawComponentRect(dc, rcBox, RGB(154, 193, 241));

	m_edit_editor_path.GetWindowRect(&rcBox);
	ScreenToClient(&rcBox);
	DrawComponentRect(dc, rcBox, RGB(154, 193, 241));

	Gdiplus::Graphics graphics(dc);
	graphics.DrawImage(commonData.m_icon_browse, rcBox.right + 7, rcBox.bottom - 19, commonData.m_icon_browse.m_pBitmap->GetWidth(), commonData.m_icon_browse.m_pBitmap->GetHeight());
	rcBrowseEditor.SetRect(rcBox.right + 7, rcBox.bottom - 19, rcBox.right + 7 + commonData.m_icon_browse.m_pBitmap->GetWidth(), rcBox.bottom - 19 + commonData.m_icon_browse.m_pBitmap->GetHeight());

	m_edit_default_encoding.GetWindowRect(&rcBox);
	ScreenToClient(&rcBox);
	DrawComponentRect(dc, rcBox, RGB(154, 193, 241));

}

void CDlgConfigDefault::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (IsPosition(rcBrowseEditor, point))
	{
		CFileDialog fileDlg(TRUE, _T("*.exe"), _T("*.exe"), OFN_FILEMUSTEXIST, _T(""), this);
		if (fileDlg.DoModal() == IDOK)
		{
			CString path = fileDlg.GetFolderPath() + _T("\\") + fileDlg.GetFileName();
			m_edit_editor_path.SetWindowText(path);
		}
	}
	else
		GetParent()->PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CDlgConfigDefault::OnBnClickedBtnConfigReset()
{
	if (MessageBox(_T("All settings are reseting.\nWould you like to process?"), _T("Bolt Text Manager"), MB_YESNO) == IDYES)
	{
		MessageBox(_T("Reset finished"), _T("Bolt Text Manager"));

		CMainFrame* m_pMainFrame = (CMainFrame*) AfxGetMainWnd();
		m_pMainFrame->ResetDlgMain();
	}
}

BOOL CDlgConfigDefault::Update()
{
	config.set_include_subfolder(IsDlgButtonChecked(IDC_CHECK_SUB_FOLDER));
	config.set_overwrite_readonly(IsDlgButtonChecked(IDC_CHECK_READONLY));
	config.set_overwrite_file(IsDlgButtonChecked(IDC_CHECK_OVERWRITE));
	config.set_custom_editor(IsDlgButtonChecked(IDC_CHECK_CUSTOM_EDITOR));

	if (IsDlgButtonChecked(IDC_RADIO_HEAD))
		config.set_option_name_prefix(TRUE);
	else
		config.set_option_name_prefix(FALSE);
	CString value = _T("");
	GetDlgItemText(IDC_EDIT_OPTION_NAME, value);
	config.set_option_name(value);
	value = _T("");
	GetDlgItemText(IDC_EDIT_EDITOR_PATH, value);
	config.set_editor_path(value);

	return TRUE;
}

void CDlgConfigDefault::OnBnClickedCheckOverwrite()
{
	if (IsDlgButtonChecked(IDC_CHECK_OVERWRITE) == TRUE)
	{
		GetDlgItem(IDC_CHECK_READONLY)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_HEAD)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_TAIL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_OPTION_NAME)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_CHECK_READONLY)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_HEAD)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_TAIL)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_OPTION_NAME)->EnableWindow(TRUE);
	}
}

void CDlgConfigDefault::OnBnClickedCheckReadonly()
{
}

void CDlgConfigDefault::OnBnClickedCheckCustomEditor()
{
	if (IsDlgButtonChecked(IDC_CHECK_CUSTOM_EDITOR) == TRUE)
	{
		GetDlgItem(IDC_EDIT_EDITOR_PATH)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_EDITOR_PATH)->EnableWindow(FALSE);
	}
}

