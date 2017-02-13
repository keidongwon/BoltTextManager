#pragma once

#include "DlgConfigDefault.h"

class CDlgConfig : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgConfig)

public:
	CDlgConfig(CWnd* pParent = NULL);
	virtual ~CDlgConfig();

	enum { IDD = IDD_DLG_CONFIG };

private:
	CListCtrl m_list_config;
	CFont m_fedit;
	CDlgConfigDefault *dlgConfigDefault;

public:
	void SetDialog(CString item);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnNMClickListConfig(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListConfig(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
};

