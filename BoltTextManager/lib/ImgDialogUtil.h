#pragma once

#define WS_EX_LAYERED           0x00080000
#define LWA_COLORKEY            0x00000001
#define LWA_ALPHA               0x00000002
#define ULW_COLORKEY            0x00000001
#define ULW_ALPHA               0x00000002
#define ULW_OPAQUE              0x00000004
#define BUFFERSIZE				8192
typedef BOOL(WINAPI *SLWA)(HWND, COLORREF, BYTE, DWORD);

using namespace Gdiplus;

class CImgDialogUtil
{
public:
	static CString GetModulePath(HMODULE hModule = NULL);
	static BOOL IsFileExist(LPCTSTR lpszFilePath);
	static BOOL ExtractResourceToFile( LPCTSTR lpszType
									, UINT nResID
									, LPCTSTR lpszFilename
									, HMODULE hModule
									);

	static Image * LoadImage( UINT nID, LPCTSTR lpszType, HINSTANCE hInstance = NULL);
};
