#pragma once
#include "../lib/CGdiPlusBitmap.h"
#include "../MFCUI.h"

using namespace Gdiplus;

class CommonData
{
public:
	void Load()
	{
		m_bg_top.Load(IDB_BG_TOP, _T("PNG"), AfxGetApp()->m_hInstance);
		m_bg_bottom.Load(IDB_BG_BOTTOM, _T("PNG"), AfxGetApp()->m_hInstance);
		m_image_resize.Load(IDB_IMAGE_RESIZE, _T("PNG"), AfxGetApp()->m_hInstance);
		m_icon_small.Load(IDB_ICON_SMALL, _T("PNG"), AfxGetApp()->m_hInstance);
		m_icon_max.Load(IDB_ICON_MAX, _T("PNG"), AfxGetApp()->m_hInstance);
		m_icon_exit.Load(IDB_ICON_EXIT, _T("PNG"), AfxGetApp()->m_hInstance);
		m_image_title.Load(IDB_IMAGE_TITLE, _T("PNG"), AfxGetApp()->m_hInstance);

		m_menu_encoding.Load(IDB_PNG_ENCODING_SEL, _T("PNG"), AfxGetApp()->m_hInstance);
		m_icon_config.Load(IDB_ICON_CONFIG, _T("PNG"), AfxGetApp()->m_hInstance);
		m_icon_info.Load(IDB_ICON_INFO, _T("PNG"), AfxGetApp()->m_hInstance);
		m_icon_sh_start.Load(IDB_ICON_START, _T("PNG"), AfxGetApp()->m_hInstance);
		m_icon_document.Load(IDB_ICON_DOC, _T("PNG"), AfxGetApp()->m_hInstance);
		m_icon_folderdoc.Load(IDB_ICON_FOLDER_DOC, _T("PNG"), AfxGetApp()->m_hInstance);
		m_icon_trash.Load(IDB_ICON_TRASH, _T("PNG"), AfxGetApp()->m_hInstance);
		m_icon_refresh.Load(IDB_ICON_REFRESH, _T("PNG"), AfxGetApp()->m_hInstance);
		m_icon_browse.Load(IDB_ICON_BROWSE, _T("PNG"), AfxGetApp()->m_hInstance);
	}

	static void draw(Graphics &graphics, CGdiPlusBitmapResource &bm, int &x, int &y, int space, CRect &rect)
	{
		graphics.DrawImage(bm, x, y, bm.m_pBitmap->GetWidth(), bm.m_pBitmap->GetHeight());
		rect.SetRect(x, y, x + bm.m_pBitmap->GetWidth(), y + bm.m_pBitmap->GetHeight());
		x = x + space + bm.m_pBitmap->GetWidth();
	}
	
	CGdiPlusBitmapResource m_image_title;
	CGdiPlusBitmapResource m_bg_top;
	CGdiPlusBitmapResource m_bg_bottom;;
	CGdiPlusBitmapResource m_image_resize;
	CGdiPlusBitmapResource m_icon_small;
	CGdiPlusBitmapResource m_icon_max;
	CGdiPlusBitmapResource m_icon_exit;
	CGdiPlusBitmapResource m_menu_encoding;
	CGdiPlusBitmapResource m_icon_config;
	CGdiPlusBitmapResource m_icon_info;
	CGdiPlusBitmapResource m_icon_sh_start;
	CGdiPlusBitmapResource m_icon_document;
	CGdiPlusBitmapResource m_icon_folderdoc;
	CGdiPlusBitmapResource m_icon_trash;
	CGdiPlusBitmapResource m_icon_refresh;
	CGdiPlusBitmapResource m_image_body;
	CGdiPlusBitmapResource m_icon_browse;
};

extern CommonData commonData;
