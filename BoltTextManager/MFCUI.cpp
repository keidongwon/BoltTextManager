#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "MFCUI.h"
#include "MainFrm.h"
#include "MFCUIDoc.h"
#include "MFCUIView.h"
#include "common/CommonData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CMFCUIApp, CWinAppEx)
    ON_COMMAND(ID_APP_ABOUT, &CMFCUIApp::OnAppAbout)
    ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
    ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
END_MESSAGE_MAP()


CMFCUIApp::CMFCUIApp()
{
    m_bHiColorIcons = TRUE;

    // 다시 시작 관리자 지원
    m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
    // 응용 프로그램을 공용 언어 런타임 지원을 사용하여 빌드한 경우(/clr):
    //     1) 이 추가 설정은 다시 시작 관리자 지원이 제대로 작동하는 데 필요합니다.
    //     2) 프로젝트에서 빌드하려면 System.Windows.Forms에 대한 참조를 추가해야 합니다.
    System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

    // CompanyName.ProductName.SubProduct.VersionInformation
    SetAppID(_T("Archesoft.BoltTextManager.2"));
}

CMFCUIApp theApp;

BOOL CMFCUIApp::InitInstance()
{
    // 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
    // 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다. 
    // InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    // 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
    // 이 항목을 설정하십시오.
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinAppEx::InitInstance();


    HANDLE hMutexOneInstance = ::CreateMutex(NULL, TRUE, L"BoltTextManager");
    BOOL bFound = FALSE;
    if (::GetLastError() == ERROR_ALREADY_EXISTS) bFound = TRUE;
    if (hMutexOneInstance) ::ReleaseMutex(hMutexOneInstance);
    if (bFound)
    {
        CloseHandle(hMutexOneInstance);
        return FALSE;
    }

    GdiplusStartupInput gdiplusStartupInput;
    if (::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL) != Ok)
    {
        ::OutputDebugString(_T("ERROR : Failed to initialize GDI+"));
        return FALSE;
    }

    if (!AfxOleInit())
    {
        AfxMessageBox(IDP_OLE_INIT_FAILED);
        return FALSE;
    }

    AfxEnableControlContainer();

    EnableTaskbarInteraction(FALSE);

    // AfxInitRichEdit2();

    //SetRegistryKey(_T("BoltTextManager"));
    LoadStdProfileSettings(4);  // MRU를 포함하여 표준 INI 파일 옵션을 로드합니다.

    InitContextMenuManager();
    //InitKeyboardManager();

    InitTooltipManager();
    CMFCToolTipInfo ttParams;
    ttParams.m_bVislManagerTheme = TRUE;
    theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
        RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);


    CSingleDocTemplate* pDocTemplate;
    pDocTemplate = new CSingleDocTemplate(
        IDR_MAINFRAME,
        RUNTIME_CLASS(CMFCUIDoc),
        RUNTIME_CLASS(CMainFrame),
        RUNTIME_CLASS(CMFCUIView));
    if (!pDocTemplate)
        return FALSE;
    AddDocTemplate(pDocTemplate);


    CCommandLineInfo cmdInfo;
    ParseCommandLine(cmdInfo);

    //EnableShellOpen();
    //RegisterShellFileTypes(TRUE);

    //if (cmdInfo.m_nShellCommand != CCommandLineInfo::FileNew)
    //{
    //    if (!ProcessShellCommand(cmdInfo))
    //        return FALSE;
    //}

    commonData.Load();

    if (!ProcessShellCommand(cmdInfo))
        return FALSE;

    m_pMainWnd->DragAcceptFiles(TRUE);
    m_pMainWnd->ShowWindow(SW_SHOW);
    m_pMainWnd->UpdateWindow();

    // 접미사가 있을 경우에만 DragAcceptFiles를 호출합니다.
    //  SDI 응용 프로그램에서는 ProcessShellCommand 후에 이러한 호출이 발생해야 합니다.
    return TRUE;
}

int CMFCUIApp::ExitInstance()
{
    AfxOleTerm(FALSE);
    return CWinAppEx::ExitInstance();
}

class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();
    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

void CMFCUIApp::OnAppAbout()
{
    CAboutDlg aboutDlg;
    aboutDlg.DoModal();
}

void CMFCUIApp::PreLoadState()
{
    BOOL bNameValid;
    CString strName;
    bNameValid = strName.LoadString(IDS_EDIT_MENU);
    ASSERT(bNameValid);
    GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CMFCUIApp::LoadCustomState()
{
}

void CMFCUIApp::SaveCustomState()
{
}
