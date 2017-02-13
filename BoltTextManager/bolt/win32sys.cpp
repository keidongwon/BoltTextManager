#include "stdafx.h"
#include "win32sys.h"
#include "file.h"

namespace bolt {

wstring get_module_name(HMODULE hModule)
{
	wchar_t fullpath[MAX_PATH] = { 0, };
	wchar_t target[MAX_PATH] = { 0, };
	wstring filename;
	::GetModuleFileNameW(hModule, fullpath, MAX_PATH);
	_wsplitpath_s(fullpath, NULL, 0, NULL, 0, target, MAX_PATH, NULL, 0);
	filename = target;
	return filename;
}

wstring get_module_path(HMODULE hModule)
{
	wchar_t buf[MAX_PATH] = { '\0' };
	wstring path;
	::GetModuleFileNameW(hModule, buf, MAX_PATH);
	path = buf;
	int index = path.find_last_of(L'\\');
	path.erase(index, (path.size() - index));
	return path;
}

wstring get_current_path()
{
	wchar_t path[MAX_PATH] = { 0, };
	GetCurrentDirectoryW(MAX_PATH, path);
	return path;
}

wstring get_environment(wstring type)
{
	wchar_t value[MAX_PATH] = { 0, };
	GetEnvironmentVariableW(type.c_str(), value, MAX_PATH);
	return value;
}

string get_lasterror_msg()
{
	int nErr = GetLastError();
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		nErr,
		0,
		(LPSTR)&lpMsgBuf,
		0,
		NULL);
	return string((LPSTR)lpMsgBuf);
}

CString get_window_path()
{
	TCHAR szWinPath[MAX_PATH];
	::GetWindowsDirectory(szWinPath, MAX_PATH);
	CString path = szWinPath;
	return path;
}

void run_process(CString path, CString args, CString option)
{
	if (bolt::File::is_exist_file(path) == FALSE) return;
	SHELLEXECUTEINFO shExecInfo;
	shExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	shExecInfo.fMask = NULL;
	shExecInfo.hwnd = NULL;
	shExecInfo.lpVerb = option;
	shExecInfo.lpFile = path;
	shExecInfo.lpParameters = args;
	shExecInfo.lpDirectory = NULL;
	shExecInfo.nShow = SW_NORMAL;
	shExecInfo.hInstApp = NULL;
	ShellExecuteEx(&shExecInfo);
}

}
