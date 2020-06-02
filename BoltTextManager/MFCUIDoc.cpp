#include "stdafx.h"
#include "MFCUI.h"
#include "MFCUIDoc.h"
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CMFCUIDoc, CDocument)

BEGIN_MESSAGE_MAP(CMFCUIDoc, CDocument)
END_MESSAGE_MAP()

CMFCUIDoc::CMFCUIDoc()
{
}

CMFCUIDoc::~CMFCUIDoc()
{
}

BOOL CMFCUIDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    return TRUE;
}

void CMFCUIDoc::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
    }
    else
    {
    }
}

#ifdef _DEBUG
void CMFCUIDoc::AssertValid() const
{
    CDocument::AssertValid();
}

void CMFCUIDoc::Dump(CDumpContext& dc) const
{
    CDocument::Dump(dc);
}
#endif 
