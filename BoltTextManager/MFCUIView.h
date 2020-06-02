#pragma once

#include "resource.h"

class CMainFrame;
class CMFCUIView : public CFormView
{
protected:
    CMFCUIView();
    DECLARE_DYNCREATE(CMFCUIView)

public:
    enum { IDD = IDD_MFCUI_FORM };

public:
    CMFCUIDoc* GetDocument() const;

public:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual void OnInitialUpdate();

public:
    virtual ~CMFCUIView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

public:
    BOOL bMaximize;
    void SetStatus(CString status);
    void Exit();

private:
    CMainFrame *m_pMainFrame;

    CRect rcIconSmall;
    CRect rcIconMax;
    CRect rcIconExit;
    CRect rcImageResize;
    CRect rcEncodingTool;

    CRect rcIconConfig;
    CRect rcIconInfo;

protected:

protected:
    afx_msg void OnFilePrintPreview();
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG
inline CMFCUIDoc* CMFCUIView::GetDocument() const
{
    return reinterpret_cast<CMFCUIDoc*>(m_pDocument);
}
#endif

