#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"
using namespace Gdiplus;

class CMFCUIApp : public CWinAppEx
{
public:
	CMFCUIApp();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

private:
	ULONG_PTR gdiplusToken;
};

extern CMFCUIApp theApp;
