#pragma once
#include <string>
#include <vector>
#include <set>
#include "common/CommonData.h"
#include "lib/EncodingUtil.h"

class CMFCUIView;
class CDlgMain : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMain)

public:
	CDlgMain(CWnd* pParent = NULL);
	virtual ~CDlgMain();
	BOOL Create(CRect& rcWnd, UINT nIDTemplate, CWnd* pParentWnd);
	enum { IDD = IDD_DLG_MAIN };

public:
	void Resize(CRect& rect);
	void Reset();
	void ComponentResize();
	int GetSelectedCount();
	int GetSelectedRow();

	void GetFiles(CString dir);
	void AddFile(CString path, bool ignore = false);
	void RemoveFile(CString path);
	void RemoveFiles();
	void EncodingFile(CString path, CString eol, CString encoding, CString charset);
	void EncodingFiles();
	void ExecuteFile(CString path);
	void ExecuteFiles();

	BOOL SearchList(CString path);
	void UpdateList(CString path, CString eol, CString encoding, CString charset);
	void SetListStyle();
	CString GetEncodingType(std::wstring encoding, std::wstring charset);
	CString GetEolType(EolType eol);

	void UncheckRadioButtons();
	void SetRadioButtons(BOOL flag);

	BOOL StartConvert();
	void About();
	void Exit();
	void Config();
	void Reload();
	void Delete();
	void OpenFile();
	void OpenFolder();
	void ShowList();
	void ShowInfo();

private:
	CMFCUIView *pMFCUIView;

	CToolTipCtrl* m_pToolTip;
	CListCtrl m_list_main;
	CComboBox m_combo_convert_to;
	CEdit m_edit_textview;
	CStatic m_static_filename;
	CRect m_WindowRect;
	CImageList* m_pDragImage;
	CStatic m_static_format;
	CButton m_rf_not_convert;
	CButton m_rf_dos_win;
	CButton m_rf_unix;
	CButton m_rf_mac;

	HACCEL m_hSearch;
	HICON m_hIcon;
	HDROP hDrop;

	std::set<int> m_set_selected;

	BOOL bShowFullPath;
	int nDragIndex;
	int nDropIndex;
	int nRow;
	UINT nListWidth;
	UINT nListHeight;
	UINT nListX;
	UINT nListY;

	BOOL bConvert;
	EolType convert_eol;
	int convert_type;

	CRect rcIconStart;
	CRect rcIconDocument;
	CRect rcIconFolderDoc;
	CRect rcIconTrash;
	CRect rcRefresh;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnNMRClickListMain(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBegindragListMain(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLvnItemchangedListMain(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnStnDblclickStaticFilename();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedCheckFullpath();
	afx_msg void OnCbnSelchangeComboConvertTo();
	afx_msg void OnBnClickedRfNotConvert();
	afx_msg void OnBnClickedRfDosWin();
	afx_msg void OnBnClickedRfUnix();
	afx_msg void OnBnClickedRfMac();
};

