#pragma once
#include <windows.h>
#include <string>
#include <vector>

using namespace std;

namespace bolt
{
class Ini
{
public:
	Ini();
	Ini(LPCWSTR _filename, BOOL bModule = FALSE);
	Ini(LPCSTR _filename, BOOL bModule = FALSE);
	~Ini();

	void	set_filename(LPCWSTR _filename, BOOL bModule = FALSE);
	void	set_filename(LPCSTR _filename, BOOL bModule = FALSE);
	BOOL	get_filename(LPWSTR _filename);
	BOOL	get_filename(LPSTR _filename);
	BOOL	set_value(LPCWSTR _section, LPCWSTR _key, LPCWSTR _value);
	BOOL	set_value(LPCSTR _section, LPCSTR _key, LPCSTR _value);
	BOOL	set_value(LPCWSTR _section, LPCWSTR _key, const DWORD dwValue);
	BOOL	set_value(LPCSTR _section, LPCSTR _key, const DWORD dwValue);
	DWORD	get_value(LPCWSTR _section, LPCWSTR _key, LPWSTR _value, const DWORD dwsize = 1024);
	DWORD	get_value(LPCSTR _section, LPCSTR _key, LPSTR _value, const DWORD dwsize = 1024);
	DWORD	get_value(const wstring _section, const wstring _key, wstring &_value, const DWORD dwsize = 1024);
	DWORD	get_value(const string _section, const string _key, string &_value, const DWORD dwsize = 1024);
	string	get_string_value(const string _section, const string _key, const DWORD dwsize = 1024);
	wstring get_wstring_value(const wstring _section, const wstring _key, const DWORD dwsize = 1024);
	UINT	get_int_value(LPCWSTR _section, LPCWSTR _key);
	UINT	get_int_value(LPCSTR _section, LPCSTR _key);
	BOOL	get_bool_value(LPCWSTR _section, LPCWSTR _key);
	BOOL	get_bool_value(LPCSTR _section, LPCSTR _key);
	DWORD	get_array_value(LPCWSTR _section, vector< pair<wstring, wstring> > &result, const DWORD dwsize = 1024);
	DWORD	get_array_value(LPCSTR _section, vector< pair<string, string> > &result, const DWORD dwsize = 1024);
	DWORD	getIncreaseValue(LPCWSTR _section, LPCWSTR _key, vector<wstring> &result, const DWORD dwsize = 1024);
	DWORD	getIncreaseValue(LPCSTR _section, LPCSTR _key, vector<string> &result, const DWORD dwsize = 1024);

protected:
	wchar_t m_filename[MAX_PATH];

};

}