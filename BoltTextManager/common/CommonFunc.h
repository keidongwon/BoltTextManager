#pragma once

typedef BOOL(WINAPI *SLWA)(HWND, COLORREF, BYTE, DWORD);

inline BOOL IsPosition(CRect rect, CPoint point)
{
	if ((point.x >= rect.left) && (point.x <= rect.right) && (point.y >= rect.top) && (point.y <= rect.bottom))
		return TRUE;

	return FALSE;
}

inline void DrawWindowRect(CDC &dc, CRect &rect, COLORREF color)
{
	CPen pen,*pOldPen;
	pen.CreatePen(PS_SOLID, 1, color);
	pOldPen = (CPen *)dc.SelectObject(&pen);

	dc.MoveTo(rect.left, rect.top);
	dc.LineTo(rect.left, rect.bottom-1);
	dc.LineTo(rect.right-1, rect.bottom-1);
	dc.LineTo(rect.right-1, rect.top);
	dc.LineTo(rect.left, rect.top);

	dc.SelectObject(pOldPen);
	DeleteObject(pen);
}

inline void DrawComponentRect(CDC &dc, CRect &rect, COLORREF color)
{
	CPen pen,*pOldPen;
	pen.CreatePen(PS_SOLID, 1, color);
	pOldPen = (CPen *)dc.SelectObject(&pen);

	dc.MoveTo(rect.left-1, rect.top-1);
	dc.LineTo(rect.left-1, rect.bottom+1);
	dc.LineTo(rect.right+1, rect.bottom+1);
	dc.LineTo(rect.right+1, rect.top-1);
	dc.LineTo(rect.left-1, rect.top-1);

	dc.SelectObject(pOldPen);
	DeleteObject(pen);
}

inline void EditTextRect(CEdit &edit, CRect &rect)
{
	CRect rcEdit;
	edit.GetClientRect(&rcEdit);
	edit.GetRect(&rect);
	int y1 = rcEdit.Height() / 2;
	int y2 = rect.Height() / 2;
	int y3 = y1 - y2;
	rect.MoveToY(y3);
}

inline void EditTextControl(CEdit &edit)
{
	CRect rcEdit, rcText;
	edit.GetClientRect(&rcEdit);
	edit.GetRect(&rcText);
	int y1 = rcEdit.Height() / 2;
	int y2 = rcText.Height() / 2;
	int y3 = y1 - y2;
	rcText.MoveToY(y3);
	edit.SetRect(&rcText);
	edit.SetMargins(2, 2);
}

inline void EditTextControl(CEdit &edit, CRect &rect)
{
	edit.SetRect(rect);
	edit.SetMargins(2, 2);
}

inline void SetWindowTransparent(CWnd *wnd, UINT alpha)
{
	SLWA pSetLayeredWindowAttributes = NULL;
	HINSTANCE hmodUSER32 = LoadLibraryW(L"USER32.DLL");
	pSetLayeredWindowAttributes=(SLWA)GetProcAddress(hmodUSER32,("SetLayeredWindowAttributes"));

	HWND hwnd = wnd->m_hWnd;
	SetWindowLong(hwnd, GWL_EXSTYLE,GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	pSetLayeredWindowAttributes(hwnd, 0, (255 * alpha) / 100, LWA_ALPHA);
}
