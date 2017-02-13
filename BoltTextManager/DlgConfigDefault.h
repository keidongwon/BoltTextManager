#pragma once

class CDlgConfigDefault : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgConfigDefault)

public:
	CDlgConfigDefault(CWnd* pParent = NULL);
	virtual ~CDlgConfigDefault();
	BOOL Create(CRect& rcWnd, UINT nIDTemplate, CWnd* pParentWnd);
	BOOL Update();

	enum { IDD = IDD_DLG_CONFIG_DEFAULT };

private:
	CRect m_WindowRect;
	CEdit m_edit_option_name;
	CEdit m_edit_editor_path;
	CEdit m_edit_default_encoding;
	CRect rcBrowseEditor;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()

public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedBtnConfigReset();
	afx_msg void OnBnClickedCheckOverwrite();
	afx_msg void OnBnClickedCheckCustomEditor();
	afx_msg void OnBnClickedCheckReadonly();
};
