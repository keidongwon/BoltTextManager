#pragma once

#include "lib/ImgDialogBase.h"

class CDlgAppInfo : public CImgDialogBase
{
	DECLARE_DYNAMIC(CDlgAppInfo)

public:
	CDlgAppInfo(CWnd* pParent = NULL);
	virtual ~CDlgAppInfo();

	enum { IDD = IDD_DLG_MFCUI };

private:
	CStatic m_static_info;
	CFont m_fedit;
	CRect rcConfirm;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
