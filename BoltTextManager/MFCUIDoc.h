#pragma once

class CMFCUIDoc : public CDocument
{
protected:
	CMFCUIDoc();
	DECLARE_DYNCREATE(CMFCUIDoc)

public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

public:
	virtual ~CMFCUIDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

protected:
	DECLARE_MESSAGE_MAP()
};

