#include "stdafx.h"
#include <ShellAPI.h>
#include <locale>
#include "MFCUIDoc.h"
#include "MFCUIView.h"
#include "MainFrm.h"
#include "AfxDialogEx.h"
#include "bolt/file.h"
#include "bolt/win32sys.h"
#include "common/ConfigManager.h"
#include "lib/text_encoding_detect.h"
#include "DlgMain.h"
#include "DlgConfig.h"
#include "DlgAppInfo.h"

using namespace AutoIt::Common;

IMPLEMENT_DYNAMIC(CDlgMain, CDialogEx)

CDlgMain::CDlgMain(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgMain::IDD, pParent)
{
	m_hIcon			= AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	pMFCUIView		= (CMFCUIView*)pParent;
	m_pToolTip		= NULL;
	bConvert		= FALSE;
	bShowFullPath	= FALSE;
	convert_eol		= EolType::unknown;
	convert_type	= TextEncodingDetect::UTF8_NOBOM;
	nRow			= -1;
}

CDlgMain::~CDlgMain()
{
	if (m_pToolTip) delete m_pToolTip;
}

void CDlgMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MAIN, m_list_main);
	DDX_Control(pDX, IDC_COMBO_CONVERT_TO, m_combo_convert_to);
	DDX_Control(pDX, IDC_EDIT_TEXTVIEW, m_edit_textview);
	DDX_Control(pDX, IDC_STATIC_FILENAME, m_static_filename);
	DDX_Control(pDX, IDC_STATIC_FORMAT, m_static_format);
	DDX_Control(pDX, IDC_RF_NOT_CONVERT, m_rf_not_convert);
	DDX_Control(pDX, IDC_RF_DOS_WIN, m_rf_dos_win);
	DDX_Control(pDX, IDC_RF_UNIX, m_rf_unix);
	DDX_Control(pDX, IDC_RF_MAC, m_rf_mac);
}

BEGIN_MESSAGE_MAP(CDlgMain, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_NOTIFY(NM_RCLICK, IDC_LIST_MAIN, &CDlgMain::OnNMRClickListMain)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_MAIN, &CDlgMain::OnLvnItemchangedListMain)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST_MAIN, &CDlgMain::OnBegindragListMain)
	ON_WM_MOUSEMOVE()
	ON_COMMAND(IDC_CONFIG, &CDlgMain::Config)
	ON_COMMAND(IDC_ABOUT, &CDlgMain::About)
	ON_COMMAND(IDC_EXIT, &CDlgMain::Exit)
	ON_COMMAND(IDC_RELOAD, &CDlgMain::Reload)
	ON_COMMAND(IDC_DELETE, &CDlgMain::Delete)
	ON_STN_DBLCLK(IDC_STATIC_FILENAME, &CDlgMain::OnStnDblclickStaticFilename)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_CHECK_FULLPATH, &CDlgMain::OnBnClickedCheckFullpath)
	ON_CBN_SELCHANGE(IDC_COMBO_CONVERT_TO, &CDlgMain::OnCbnSelchangeComboConvertTo)
	ON_BN_CLICKED(IDC_RF_NOT_CONVERT, &CDlgMain::OnBnClickedRfNotConvert)
	ON_BN_CLICKED(IDC_RF_DOS_WIN, &CDlgMain::OnBnClickedRfDosWin)
	ON_BN_CLICKED(IDC_RF_UNIX, &CDlgMain::OnBnClickedRfUnix)
	ON_BN_CLICKED(IDC_RF_MAC, &CDlgMain::OnBnClickedRfMac)
END_MESSAGE_MAP()


BOOL CDlgMain::Create(CRect& wndRect, UINT nIDTemplate, CWnd* pParentWnd)
{
	m_WindowRect.CopyRect(&wndRect);
	return CDialogEx::Create(nIDTemplate, pParentWnd);
}

BOOL CDlgMain::PreTranslateMessage(MSG* pMsg) 
{ 
	if (m_hSearch != NULL)
	{
		if (TranslateAccelerator(m_hWnd, m_hSearch, pMsg))
			return TRUE;
	}

    switch (pMsg->wParam)
	{
    case VK_ESCAPE:
        return TRUE;

	case VK_RETURN:
		return TRUE;

    default:
        break;
    }

	switch(pMsg->message)
	{	
	case WM_MOUSEMOVE:
		m_pToolTip->RelayEvent(pMsg);        
		break;
	}

    return CDialogEx::PreTranslateMessage(pMsg);
}

HCURSOR CDlgMain::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CDlgMain::ComponentResize()
{
	CRect rect;
	GetClientRect(&rect);
	nListWidth = rect.right - 250;
	nListHeight = rect.bottom - 85;
	nListX = 30;
	nListY = 20;
	m_edit_textview.MoveWindow(nListX, nListY, nListWidth, rect.bottom - 90, TRUE);
	m_list_main.MoveWindow(nListX, nListY, nListWidth, rect.bottom - 103, TRUE);
	m_list_main.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	SetListStyle();
		
	m_combo_convert_to.MoveWindow(rect.right - 205, rect.top + 190, 170, 20);
	GetDlgItem(IDC_CHECK_FULLPATH)->MoveWindow(rect.left + 40, rect.bottom - 50, 300, 20);

	m_static_filename.MoveWindow(nListX + 10, nListY + nListHeight + 20, nListX + nListWidth - (32 * 6 + 70), 20);

	m_static_format.MoveWindow(nListWidth + 45, rect.bottom - 210, 170, 120);
	m_rf_not_convert.MoveWindow(nListWidth + 55, rect.bottom - 185, 120, 20);
	m_rf_dos_win.MoveWindow(nListWidth + 55, rect.bottom - 165, 120, 20);
	m_rf_unix.MoveWindow(nListWidth + 55, rect.bottom - 145, 120, 20);
	m_rf_mac.MoveWindow(nListWidth + 55, rect.bottom - 125, 120, 20);
}

void CDlgMain::UncheckRadioButtons()
{
	m_rf_not_convert.SetCheck(TRUE);
	m_rf_dos_win.SetCheck(FALSE);
	m_rf_unix.SetCheck(FALSE);
	m_rf_mac.SetCheck(FALSE);
}

void CDlgMain::SetRadioButtons(BOOL flag)
{
	m_rf_dos_win.EnableWindow(flag);
	m_rf_unix.EnableWindow(flag);
	m_rf_mac.EnableWindow(flag);
}

BOOL CDlgMain::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	DragAcceptFiles(TRUE);
	ChangeWindowMessageFilter(WM_DROPFILES, MSGFLT_ADD);
	ChangeWindowMessageFilter(WM_COPYDATA, MSGFLT_ADD);
	ChangeWindowMessageFilter(0x0049, MSGFLT_ADD);

	MoveWindow(m_WindowRect.left, m_WindowRect.top,	m_WindowRect.right-m_WindowRect.left, m_WindowRect.bottom-m_WindowRect.top,	TRUE);

	m_combo_convert_to.AddString(_T("UTF-8"));
	m_combo_convert_to.AddString(_T("UTF-8 BOM"));
	m_combo_convert_to.AddString(_T("UTF-16 LE"));
	m_combo_convert_to.AddString(_T("UTF-16 BE"));
	m_combo_convert_to.AddString(_T("CP949"));
	m_combo_convert_to.AddString(_T("EUC-KR"));
	m_combo_convert_to.AddString(_T("SHIFT-JS"));
	m_combo_convert_to.AddString(_T("GB2312"));
	m_combo_convert_to.SetCurSel(0);

	m_static_filename.ShowWindow(SW_HIDE);

	if (!m_pToolTip) 
	{
		m_pToolTip = new CToolTipCtrl;
		m_pToolTip->Create(this, TTS_ALWAYSTIP|TTS_NOPREFIX);
		m_pToolTip->SetDelayTime(50);
		m_pToolTip->Activate(TRUE);
	}

	m_list_main.InsertColumn(0, _T("File"), LVCFMT_LEFT, 0);
	m_list_main.InsertColumn(1, _T("Path"), LVCFMT_LEFT, 0);
	m_list_main.InsertColumn(2, _T("EOL"), LVCFMT_LEFT, 80);
	m_list_main.InsertColumn(3, _T("Encoding"), LVCFMT_LEFT, 100);
	m_list_main.InsertColumn(4, _T("encoding"), LVCFMT_LEFT, 0);
	m_list_main.InsertColumn(5, _T("charset"), LVCFMT_LEFT, 0);
	m_list_main.ShowWindow(SW_SHOW);

	UncheckRadioButtons();

	ComponentResize();
	m_hSearch = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));

	setlocale(LC_ALL, "");

	return TRUE;
}

void CDlgMain::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_ESCAPE:
		break;

	case VK_RETURN:
		break;
	}
	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}

HBRUSH CDlgMain::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	switch(nCtlColor)
	{
	case CTLCOLOR_DLG:
		return (HBRUSH)GetStockObject(WHITE_BRUSH);
		break;
	case CTLCOLOR_STATIC:
		if (pWnd->m_hWnd == m_static_filename.m_hWnd)
		{
			SetTextColor(pDC->m_hDC, RGB(35, 129, 189));
		}
		SetBkColor(pDC->m_hDC, RGB(255, 255, 255));
		return (HBRUSH)GetStockObject(WHITE_BRUSH);
		break;
	}
	return hbr;
}

void CDlgMain::Reset()
{
	commonData.m_icon_sh_start.Load(IDB_ICON_START, _T("PNG"), AfxGetApp()->m_hInstance);
	InvalidateRect(&rcIconStart);
}

void CDlgMain::Resize(CRect& rect)
{
	m_WindowRect.CopyRect(&rect);
	MoveWindow(m_WindowRect.left, m_WindowRect.top,	m_WindowRect.right-m_WindowRect.left, m_WindowRect.bottom-m_WindowRect.top,	TRUE);
	ComponentResize();
	Invalidate();
}

void CDlgMain::OnPaint()
{
	CPaintDC dc(this);
	CRect rcWindow;
	GetClientRect(&rcWindow);

	CPen pen,*pOldPen;
	pen.CreatePen(PS_SOLID, 1, RGB(153, 210, 230));
	pOldPen = (CPen *)dc.SelectObject(&pen);
	dc.RoundRect(rcWindow.left, rcWindow.top, rcWindow.right, rcWindow.bottom, 20, 20);
	dc.RoundRect(rcWindow.left + 1, rcWindow.top + 1, rcWindow.right - 1, rcWindow.bottom - 1, 20, 20);
	dc.SelectObject(pOldPen);
	DeleteObject(pen);

	Gdiplus::Graphics graphics(dc);

	// convert
	int nServiceX = rcWindow.right - (40 * 1) - 90;
	int nServiceY = rcWindow.bottom - 65;
	int nServiceSpace = 20;

	if (bConvert == FALSE)
	{
		commonData.m_icon_sh_start.Load(IDB_ICON_START, _T("PNG"), AfxGetApp()->m_hInstance);
	}
	else
	{
		commonData.m_icon_sh_start.Load(IDB_ICON_START_GRAY, _T("PNG"), AfxGetApp()->m_hInstance);
	}
	CommonData::draw(graphics, commonData.m_icon_sh_start, nServiceX, nServiceY, nServiceSpace, rcIconStart);

	// function
	int nFunctionX = nListX + nListWidth - (32 * 4) - 60;
	int nFunctionY = nListY + nListHeight + 10;
	int nFunctionSpace = 20;

	CommonData::draw(graphics, commonData.m_icon_document, nFunctionX, nFunctionY, nFunctionSpace, rcIconDocument);
	CommonData::draw(graphics, commonData.m_icon_folderdoc, nFunctionX, nFunctionY, nFunctionSpace, rcIconFolderDoc);
	CommonData::draw(graphics, commonData.m_icon_trash, nFunctionX, nFunctionY, nFunctionSpace, rcIconTrash);
	CommonData::draw(graphics, commonData.m_icon_refresh, nFunctionX, nFunctionY, nFunctionSpace, rcRefresh);
	
	// etc
	if (config.get_main_image() == 1)
		commonData.m_image_body.Load(IDB_PNG_EBOOKS, _T("PNG"), AfxGetApp()->m_hInstance);
	else
		commonData.m_image_body.Load(IDB_PNG_TEXT, _T("PNG"), AfxGetApp()->m_hInstance);
	graphics.DrawImage(commonData.m_image_body, 
		rcWindow.right - (commonData.m_image_body.m_pBitmap->GetWidth() + 10), 
		rcWindow.top + 20, 
		commonData.m_image_body.m_pBitmap->GetWidth(), commonData.m_image_body.m_pBitmap->GetHeight());
}

void CDlgMain::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (IsPosition(rcIconStart, point))
	{
		if (bConvert == FALSE)
		{
			bConvert = TRUE;
			if (StartConvert() == TRUE)
			{
				commonData.m_icon_sh_start.Load(IDB_ICON_START_GRAY, _T("PNG"), AfxGetApp()->m_hInstance);
				InvalidateRect(&rcIconStart);
			}
			else
				bConvert = FALSE;
		}
	}
	else if (IsPosition(rcIconDocument, point))
	{
		OpenFile();
	}
	else if (IsPosition(rcIconFolderDoc, point))
	{
		OpenFolder();
	}
	else if (IsPosition(rcIconTrash, point))
	{
		if (m_list_main.GetItemCount() == 0) return;
		if (MessageBox(_T("All lists are deleting.\nWould you like to process?"), _T("Bolt Text Manager"), MB_YESNO) == IDYES)
		{
			m_list_main.DeleteAllItems();
		}
	}
	else if (IsPosition(rcRefresh, point))
	{
		Reload();
	}
	else
	{
		GetParentFrame()->PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CDlgMain::OnLButtonUp(UINT nFlags, CPoint point)
{
	CDialogEx::OnLButtonUp(nFlags, point);
}

void CDlgMain::OnNMRClickListMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	CMenu menu, *pSubMenu;
	CWnd*  pMainWindow = AfxGetMainWnd();

	if (!menu.LoadMenu(IDR_MENU_LIST_MAIN)) return;
	if(!(pSubMenu = menu.GetSubMenu(0))) return;

	CPoint pos = GetCurrentMessage()->pt;
	GetCursorPos(&pos);
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, pos.x, pos.y, pMainWindow);

	*pResult = 0;
}

int CDlgMain::GetSelectedCount()
{
	m_set_selected.clear();
	POSITION pos = m_list_main.GetFirstSelectedItemPosition();
	while (pos)
	{
		int sel = m_list_main.GetNextSelectedItem(pos);
		m_set_selected.insert(sel);
	}
	return m_list_main.GetSelectedCount();
}

int CDlgMain::GetSelectedRow()
{
	return nRow;
}

void CDlgMain::OnLvnItemchangedListMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if (pNMLV->uNewState == 3)
	{
		nRow = pNMLV->iItem;
	}
	*pResult = 0;
}

void CDlgMain::OnMouseMove(UINT nFlags, CPoint point)
{
	// service
	if (IsPosition(rcIconStart, point))
	{
		SetCursor(LoadCursor(NULL, IDC_HAND));
		m_pToolTip->AddTool(this, _T("Convert"), CRect(point.x-1,point.y-1,point.x+1,point.y+1), TRUE);
	}

	// function
	else if (IsPosition(rcIconDocument, point))
	{
		SetCursor(LoadCursor(NULL, IDC_HAND));
		m_pToolTip->AddTool(this, _T("Open file"), CRect(point.x-1,point.y-1,point.x+1,point.y+1), TRUE);
	}
	else if (IsPosition(rcIconFolderDoc, point))
	{
		SetCursor(LoadCursor(NULL, IDC_HAND));
		m_pToolTip->AddTool(this, _T("Open folder"), CRect(point.x-1,point.y-1,point.x+1,point.y+1), TRUE);
	}
	else if (IsPosition(rcIconTrash, point))
	{
		SetCursor(LoadCursor(NULL, IDC_HAND));
		m_pToolTip->AddTool(this, _T("Clear list"), CRect(point.x-1,point.y-1,point.x+1,point.y+1), TRUE);
	}
	else if (IsPosition(rcRefresh, point))
	{
		SetCursor(LoadCursor(NULL, IDC_HAND));
		m_pToolTip->AddTool(this, _T("Refresh"), CRect(point.x-1,point.y-1,point.x+1,point.y+1), TRUE);
	}
	else
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
	}
	CDialogEx::OnMouseMove(nFlags, point);
}

// service
BOOL CDlgMain::StartConvert()
{
	if (m_list_main.GetItemCount() == 0) 
	{
		MessageBox(_T("목록이 비어있습니다. 변환을 실행할 수 없습니다"), _T("Bolt Text Manager"));
		return FALSE;
	}
	bConvert = TRUE;
	CString path;
	CString eol;
	CString encoding;
	CString charset;
	for (int i=0; i<m_list_main.GetItemCount(); i++)
	{
		path = m_list_main.GetItemText(i, 1);
		eol = m_list_main.GetItemText(i, 2);
		encoding = m_list_main.GetItemText(i, 4);
		charset = m_list_main.GetItemText(i, 5);
		EncodingFile(path, eol, encoding, charset);
	}
	bConvert = FALSE;
	return TRUE;
}

// menu
int CALLBACK CallbackForFolder(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	switch(uMsg)
	{
	case BFFM_INITIALIZED:
		SendMessage(hWnd, BFFM_SETSELECTION, TRUE, lpData);
		break;
	}
	return 0;
}

void CDlgMain::Config()
{
	CDlgConfig dlgConfig;
	dlgConfig.DoModal();
}

void CDlgMain::About()
{
	CDlgAppInfo dlgAppInfo;
	dlgAppInfo.DoModal();
}

void CDlgMain::Exit()
{
	pMFCUIView->Exit();
}

void CDlgMain::Delete()
{
	if (GetSelectedCount() > 0) RemoveFiles();
}

void CDlgMain::Reload()
{
	if (bConvert) return;

	EncodingUtil encutil;
	CString path;
	for (int i=0; i<m_list_main.GetItemCount(); i++)
	{
		path = m_list_main.GetItemText(i, 1);
		EolType eol;
		wstring encoding;
		wstring charset;
		encutil.check(path, eol, encoding, charset);
		UpdateList(path, GetEolType(eol), encoding.c_str(), charset.c_str());
	}
}

// function
void CDlgMain::ShowList()
{
	m_list_main.ShowWindow(SW_SHOW);
	m_edit_textview.ShowWindow(SW_HIDE);
}

void CDlgMain::ShowInfo()
{
	m_list_main.ShowWindow(SW_HIDE);
	m_edit_textview.ShowWindow(SW_SHOW);
}

void CDlgMain::OnStnDblclickStaticFilename()
{
}

void CDlgMain::OnClose()
{
	CDialogEx::OnClose();
}

void CDlgMain::OnDestroy()
{
	CDialogEx::OnDestroy();
}

void CDlgMain::OnBegindragListMain(NMHDR* pNMHDR, LRESULT* pResult)
{
}

void CDlgMain::OnDropFiles(HDROP hDropInfo)
{
	UINT nCnt = DragQueryFile(hDropInfo, 0xffffffff, NULL, 0);
	if (nCnt == 0) return;

	for(UINT i = 0; i < nCnt; i++)
	{
		wchar_t wzItem[MAX_PATH] = {0};
		int nCount = DragQueryFile(hDropInfo, i, wzItem, MAX_PATH);
		if( nCount == 0 )
			continue ;

		AddFile(wzItem, true);
	}
	DragFinish(hDropInfo);
	CDialogEx::OnDropFiles(hDropInfo);
}

BOOL CDlgMain::SearchList(CString path)
{
	CString listFile = _T("");

	for (int i=0; i<m_list_main.GetItemCount(); i++)
	{
		listFile = m_list_main.GetItemText(i, 1);
		listFile.Trim();
		if (listFile.Find(path) >= 0) return TRUE;
	}
	return FALSE;
}

CString CDlgMain::GetEncodingType(wstring encoding, wstring charset)
{
	if (encoding == L"") return L"";
	int _encoding = std::stoi(encoding);
	CString text;
	switch (_encoding)
	{
	case TextEncodingDetect::None:
		text = _T("Unknown");
		break;
	case TextEncodingDetect::ANSI:
	case TextEncodingDetect::ASCII:
		if (charset.empty())
			text = _T("ASCII");
		else
			text = charset.c_str();
		break;
	case TextEncodingDetect::UTF8_BOM:
		text = _T("UTF8 BOM");
		break;
	case TextEncodingDetect::UTF8_NOBOM:
		text = _T("UTF8");
		break;
	case TextEncodingDetect::UTF16_LE_BOM:
		text = _T("UTF16LE BOM");
		break;
	case TextEncodingDetect::UTF16_LE_NOBOM:
		text = _T("UTF16LE");
		break;
	case TextEncodingDetect::UTF16_BE_BOM:
		text = _T("UTF16BE BOM");
		break;
	case TextEncodingDetect::UTF16_BE_NOBOM:
		text = _T("UTF16BE");
		break;
	}
	return text;
}

CString CDlgMain::GetEolType(EolType eol)
{
	CString text;
	switch (eol)
	{
	case EolType::windows:
		text = _T("Win");
		break;
	case EolType::macos:
		text = _T("Mac");
		break;
	case EolType::unix:
		text = _T("Unix");
		break;
	case EolType::unknown:
		text = _T("Unknown");
		break;
	}

	return text;
}

void CDlgMain::SetListStyle()
{
	int width_file = 0;
	int width_path = 0;
	if (bShowFullPath == TRUE)
		width_path = (nListWidth - 22) - 150;
	else
		width_file = (nListWidth - 22) - 150;
	m_list_main.SetColumnWidth(0, width_file);
	m_list_main.SetColumnWidth(1, width_path);
	m_list_main.SetColumnWidth(2, 50);
	m_list_main.SetColumnWidth(3, 100);
	m_list_main.SetColumnWidth(4, 0);
	m_list_main.SetColumnWidth(5, 0);
}

void CDlgMain::UpdateList(CString path, CString eol, CString encoding, CString charset)
{
	for (int i=0; i<m_list_main.GetItemCount(); i++)
	{
		if (path == m_list_main.GetItemText(i, 1))
		{
			m_list_main.SetItemText(i, 2, eol);
			m_list_main.SetItemText(i, 3, GetEncodingType((LPCTSTR)encoding, (LPCTSTR)charset));
			m_list_main.SetItemText(i, 4, encoding);
			m_list_main.SetItemText(i, 5, charset);
		}
	}
}

void CDlgMain::OnBnClickedCheckFullpath()
{
	bShowFullPath = IsDlgButtonChecked(IDC_CHECK_FULLPATH);
	SetListStyle();
}

void CDlgMain::OpenFile()
{
	CString extensions = config.get_allow_extensions();
	extensions = L"," + extensions;
	extensions.Replace(L",", L";*.");
	CFileDialog fileDlg(TRUE, _T("txt"), extensions, OFN_FILEMUSTEXIST, _T(""), this);
	if (fileDlg.DoModal() == IDOK)
	{
		CString path = fileDlg.GetFolderPath() + _T("\\") + fileDlg.GetFileName();
		AddFile(path.GetBuffer());
	}
}

void CDlgMain::OpenFolder()
{
	ITEMIDLIST*  pildBrowse;
	TCHAR pszPathname[MAX_PATH];
	BROWSEINFO  bInfo;
	memset(&bInfo, 0, sizeof(bInfo));
	bInfo.hwndOwner   = GetSafeHwnd();
	bInfo.pidlRoot   = NULL;
	bInfo.pszDisplayName = pszPathname;
	bInfo.lpszTitle   = _T("디렉토리를 선택하세요");
	bInfo.ulFlags   = BIF_RETURNONLYFSDIRS; 
	bInfo.lpfn    = NULL;
	bInfo.lParam  = (LPARAM)(LPCTSTR)"C:\\";
	bInfo.lParam  = (LPARAM)NULL;
	pildBrowse    = ::SHBrowseForFolder(&bInfo);
	if (!pildBrowse) return;

	SHGetPathFromIDList(pildBrowse, pszPathname);
	GetFiles(pszPathname);
}

void CDlgMain::GetFiles(CString dir)
{
	dir += L"\\*.*";
	CFileFind find;
	BOOL bResult = find.FindFile(dir);

	if (bResult == FALSE) return;

	while (bResult)
	{
		bResult = find.FindNextFileW();
		if (find.IsDots()) continue;
		if (find.IsDirectory())
		{
			if (config.is_include_subfolder() == FALSE) continue;
			GetFiles(find.GetFilePath());
		}
		else
		{
			CString name = find.GetFilePath();
			AddFile(name);
		}
	}
}

void CDlgMain::AddFile(CString path, bool ignore)
{
	if (bolt::File::is_file(path) == FALSE) return;
	if (SearchList(path) == FALSE)
	{
		wstring ext = bolt::File::get_ext(path);
		if (ext.length() == 0) return;
		size_t pos = ext.find(L".");
		ext.erase(pos, 1);
		if (ignore == false && config.is_include_extension(ext.c_str()) == FALSE) return;

		int row = m_list_main.GetItemCount();
		EncodingUtil encutil;
		wstring filename = bolt::File::get_name_with_ext(path);
		EolType eol;
		wstring charset;
		wstring encoding;
		encutil.check(path, eol, encoding, charset);
		m_list_main.InsertItem(row, filename.c_str());
		m_list_main.SetItemText(row, 1, path);
		m_list_main.SetItemText(row, 2, GetEolType(eol));
		m_list_main.SetItemText(row, 3, GetEncodingType(encoding, charset));
		m_list_main.SetItemText(row, 4, encoding.c_str());
		m_list_main.SetItemText(row, 5, charset.c_str());
	}
}

void CDlgMain::RemoveFiles()
{
	std::set<int>::iterator itr = m_set_selected.begin();
	std::vector<CString> remove_lists;
	for (itr; itr != m_set_selected.end(); itr++)
	{
		CString filename = m_list_main.GetItemText(*itr, 1);
		remove_lists.push_back(filename);
	}
	for (int i=0; i<(int)remove_lists.size(); i++)
	{
		RemoveFile(remove_lists[i]);
	}
}

void CDlgMain::RemoveFile(CString path)
{
	for (int i=0; i<m_list_main.GetItemCount(); i++)
	{
		if (path == m_list_main.GetItemText(i, 1))
			m_list_main.DeleteItem(i);
	}
}

void CDlgMain::EncodingFile(CString path, CString eol, CString encoding, CString charset)
{
	EncodingUtil encutil;

	CString newpath;
	if (config.is_overwrite_file() == TRUE)
	{
		DWORD attr = ::GetFileAttributes(path);
		if ((attr && FILE_ATTRIBUTE_SYSTEM == TRUE) ||
			(attr && FILE_ATTRIBUTE_COMPRESSED == TRUE))
		{
			return;
		}
		else if (attr && FILE_ATTRIBUTE_READONLY == TRUE)
		{
			if (!config.is_overwrite_readonly()) return;
			::SetFileAttributes(path, FILE_ATTRIBUTE_NORMAL);
		}
		newpath = path;
	}
	else
	{
		TCHAR szpath[255] = {0,};

		if (config.is_option_name_prefix() == TRUE)
		{
			wsprintf(szpath, _T("%s%s%s%s%s"), 
				bolt::File::get_drive(path).c_str(), 
				bolt::File::get_dir(path).c_str(),
				config.get_option_name(),
				bolt::File::get_name(path).c_str(),
				bolt::File::get_ext(path).c_str());
		}
		else
		{
			wsprintf(szpath, _T("%s%s%s%s%s"), 
				bolt::File::get_drive(path).c_str(), 
				bolt::File::get_dir(path).c_str(),
				bolt::File::get_name(path).c_str(),
				config.get_option_name(),
				bolt::File::get_ext(path).c_str());
		}
		newpath = szpath;
	}

	CString tocode = L"";
	bool tobom = false;
	switch (convert_type)
	{
	case TextEncodingDetect::UTF8_BOM:
		tocode = L"UTF-8";
		tobom = true;
		break;
	case TextEncodingDetect::UTF8_NOBOM:
		tocode = L"UTF-8";
		break;
	case TextEncodingDetect::UTF16_LE_BOM:
		tocode = L"UTF-16LE";
		tobom = true;
		break;
	case TextEncodingDetect::UTF16_LE_NOBOM:
		tocode = L"UTF-16LE";
		break;
	case TextEncodingDetect::UTF16_BE_BOM:
		tocode = L"UTF-16BE";
		tobom = true;
		break;
	case TextEncodingDetect::UTF16_BE_NOBOM:
		tocode = L"UTF-16BE";
		break;
	case TextEncodingDetect::ANSI_CP949:
		tocode = L"CP949";
		break;
	case TextEncodingDetect::ANSI_EUC_KR:
		tocode = L"EUC-KR";
		break;
	case TextEncodingDetect::ANSI_SHIFT_JS:
		tocode = L"SHIFT-JS";
		break;
	case TextEncodingDetect::ANSI_GB2312:
		tocode = L"GB2312";
		break;
	}

	int _encoding = _wtoi(encoding);
	bool frombom = false;
	CString fromcode;
	switch (_encoding)
	{
	case TextEncodingDetect::ANSI:
	case TextEncodingDetect::ASCII:
		if (charset != "")
			fromcode = charset;
		else
			config.get_default_encoding();
		break;
	case TextEncodingDetect::UTF8_BOM:
		fromcode = L"UTF-8";
		frombom = true;
		break;
	case TextEncodingDetect::UTF8_NOBOM:
		fromcode = L"UTF-8";
		break;
	case TextEncodingDetect::UTF16_LE_BOM:
		fromcode = L"UTF-16LE";
		frombom = true;
		break;
	case TextEncodingDetect::UTF16_LE_NOBOM:
		fromcode = L"UTF-16LE";
		break;
	case TextEncodingDetect::UTF16_BE_BOM:
		fromcode = L"UTF-16BE";
		frombom = true;
		break;
	case TextEncodingDetect::UTF16_BE_NOBOM:
		fromcode = L"UTF-16BE";
		break;
	}

	EolType fromeol = EolType::unknown;
	if (eol == "Win")
		fromeol = EolType::windows;
	else if (eol == "Mac")
		fromeol = EolType::macos;
	else if (eol == "Unix")
		fromeol = EolType::unix;

	encutil.convert(path, newpath, tocode, fromcode, tobom, frombom, convert_eol, fromeol);

	EolType m_eol;
	wstring m_encoding;
	wstring m_charset;
	encutil.check(path, m_eol, m_encoding, m_charset);

	if (config.is_overwrite_file() == TRUE)
	{
		UpdateList(newpath, GetEolType(m_eol), m_encoding.c_str(), m_charset.c_str());
	}
}

void CDlgMain::EncodingFiles()
{
	std::set<int>::iterator itr = m_set_selected.begin();
	for (itr; itr != m_set_selected.end(); itr++)
	{
		CString filename = m_list_main.GetItemText(*itr, 1);
		CString eol = m_list_main.GetItemText(*itr, 2);
		CString encoding = m_list_main.GetItemText(*itr, 4);
		CString charset = m_list_main.GetItemText(*itr, 5);
		EncodingFile(filename, eol, encoding, charset);
	}
}

void CDlgMain::ExecuteFile(CString path)
{
	if (config.is_custom_editor() == TRUE && config.get_editor_path().GetLength() > 4)
	{
		bolt::run_process(config.get_editor_path(), path);
	}
	else
	{
		CString notepad = bolt::get_window_path() + "\\notepad.exe";
		bolt::run_process(notepad, path);
	}
}

void CDlgMain::ExecuteFiles()
{
	std::set<int>::iterator itr = m_set_selected.begin();
	std::vector<CString> excute_lists;
	for (itr; itr != m_set_selected.end(); itr++)
	{
		CString filename = m_list_main.GetItemText(*itr, 1);
		excute_lists.push_back(filename);
	}
	for (int i=0; i<(int)excute_lists.size(); i++)
	{
		ExecuteFile(excute_lists[i]);
	}
}

void CDlgMain::OnCbnSelchangeComboConvertTo()
{
	switch(m_combo_convert_to.GetCurSel())
	{
	case 0:
		convert_type = TextEncodingDetect::UTF8_NOBOM;
		SetRadioButtons(TRUE);
		break;

	case 1:
		convert_type = TextEncodingDetect::UTF8_BOM;
		SetRadioButtons(TRUE);
		break;

	case 2:
		convert_type = TextEncodingDetect::UTF16_LE_BOM;
		SetRadioButtons(FALSE);
		break;

	case 3:
		convert_type = TextEncodingDetect::UTF16_BE_BOM;
		SetRadioButtons(FALSE);
		break;

	case 4:
		convert_type = TextEncodingDetect::ANSI_CP949;
		SetRadioButtons(TRUE);
		break;

	case 5:
		convert_type = TextEncodingDetect::ANSI_EUC_KR;
		SetRadioButtons(TRUE);
		break;

	case 6:
		convert_type = TextEncodingDetect::ANSI_SHIFT_JS;
		SetRadioButtons(TRUE);
		break;

	case 7:
		convert_type = TextEncodingDetect::ANSI_GB2312;
		SetRadioButtons(TRUE);
		break;
	}
	UncheckRadioButtons();
}

void CDlgMain::OnBnClickedRfNotConvert()
{
	convert_eol = EolType::unknown;
}

void CDlgMain::OnBnClickedRfDosWin()
{
	convert_eol = EolType::windows;
}

void CDlgMain::OnBnClickedRfUnix()
{
	convert_eol = EolType::unix;
}

void CDlgMain::OnBnClickedRfMac()
{
	convert_eol = EolType::macos;
}

