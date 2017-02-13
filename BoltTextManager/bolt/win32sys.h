#pragma once
#include <atlbase.h>
#include <algorithm>
#include <string>

using namespace std;

namespace bolt
{
	wstring get_module_name(HMODULE hModule = NULL);
	wstring get_module_path(HMODULE hModule = NULL);
	wstring get_current_path();
	wstring get_environment(wstring type);
	string get_lasterror_msg();
	CString get_window_path();
	void run_process(CString path, CString args, CString option = NULL);
}
