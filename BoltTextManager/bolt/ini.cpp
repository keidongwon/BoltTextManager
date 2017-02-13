#include "stdafx.h"
#include "ini.h"

namespace bolt
{

Ini::Ini()
{
	ZeroMemory(m_filename, MAX_PATH * sizeof(wchar_t));
}

Ini::Ini(LPCWSTR filename, BOOL bModule)
{
	set_filename(filename, bModule);
}

Ini::Ini(LPCSTR filename, BOOL bModule)
{
	set_filename(filename, bModule);
}

Ini::~Ini()
{
}

void Ini::set_filename(LPCWSTR filename, BOOL bModule)
{
	ZeroMemory(m_filename, MAX_PATH * sizeof(wchar_t));
	if (bModule == TRUE)
	{
		wchar_t drive[MAX_PATH] = {0,};
		wchar_t dir[MAX_PATH] = {0,};
		wchar_t name[MAX_PATH] = {0,};
		_wsplitpath(filename, drive, dir, name, NULL);
		swprintf(m_filename, L"%s%s%s.ini", drive, dir, name);
	}
	else
		wcsncpy(m_filename, filename, wcslen(filename));
}

void Ini::set_filename(LPCSTR filename, BOOL bModule)
{
	ZeroMemory(m_filename, MAX_PATH * sizeof(wchar_t));
	USES_CONVERSION;
	if (bModule == TRUE)
	{
		wchar_t drive[MAX_PATH] = {0,};
		wchar_t dir[MAX_PATH] = {0,};
		wchar_t name[MAX_PATH] = {0,};
		_wsplitpath(A2W(filename), drive, dir, name, NULL);
		swprintf(m_filename, L"%s%s%s.ini", drive, dir, name);
	}
	else
		wcsncpy(m_filename, A2W(filename), wcslen(A2W(filename)));
}

BOOL Ini::get_filename(LPWSTR filename)
{
	if ( (m_filename == NULL) || (wcslen(m_filename) == 0) ) return FALSE;
	wcsncpy(filename, m_filename, wcslen(m_filename));
	return TRUE;
}

BOOL Ini::get_filename(LPSTR filename)
{
	if ( (m_filename == NULL) || (wcslen(m_filename) == 0) ) return FALSE;
	USES_CONVERSION;
	strncpy(filename, W2A(m_filename), strlen(W2A(m_filename)));
	return TRUE;
}

BOOL Ini::set_value(LPCWSTR _section, LPCWSTR szKey, LPCWSTR szValue)
{
	return WritePrivateProfileStringW(_section, szKey, szValue, m_filename);
}

BOOL Ini::set_value(LPCSTR _section, LPCSTR szKey, LPCSTR szValue)
{
	USES_CONVERSION;
	return WritePrivateProfileStringA(_section, szKey, szValue, W2A(m_filename));
}

BOOL Ini::set_value(LPCWSTR _section, LPCWSTR szKey, const DWORD dwValue)
{
	wchar_t temp[16] = {0,};
	_ltow(dwValue, temp, 10);
	return WritePrivateProfileStringW(_section, szKey, temp, m_filename);
}

BOOL Ini::set_value(LPCSTR _section, LPCSTR szKey, const DWORD dwValue)
{
	USES_CONVERSION;
	char temp[16] = {0,};
	_ltoa(dwValue, temp, 10);
	return WritePrivateProfileStringA(_section, szKey, temp, W2A(m_filename));
}

DWORD Ini::get_value(LPCWSTR _section, LPCWSTR szKey, LPWSTR szValue, const DWORD dwSize)
{
	return GetPrivateProfileStringW(_section, szKey, L"", szValue, dwSize, m_filename);
}

DWORD Ini::get_value(LPCSTR _section, LPCSTR szKey, LPSTR szValue, const DWORD dwSize)
{
	USES_CONVERSION;
	return GetPrivateProfileStringA(_section, szKey, "", szValue, dwSize, W2A(m_filename));
}

DWORD Ini::get_value(const wstring _section, const wstring szKey, wstring &szValue, const DWORD dwSize)
{
	wchar_t *szText = new wchar_t[dwSize];
	ZeroMemory(szText, dwSize * sizeof(wchar_t));
	DWORD result = GetPrivateProfileStringW(_section.c_str(), szKey.c_str(), L"", szText, dwSize, m_filename);
	szValue = szText;
	delete [] szText;
	return result;
}

DWORD Ini::get_value(const string _section, const string szKey, string &szValue, const DWORD dwSize)
{
	USES_CONVERSION;
	char *szText = new char[dwSize];
	ZeroMemory(szText, dwSize);
	DWORD result = GetPrivateProfileStringA(_section.c_str(), szKey.c_str(), "", szText, dwSize, W2A(m_filename));
	szValue = szText;
	delete [] szText;
	return result;
}

wstring Ini::get_wstring_value(const wstring _section, const wstring szKey, const DWORD dwSize)
{
	wchar_t *szText = new wchar_t[dwSize];
	ZeroMemory(szText, dwSize * sizeof(wchar_t));
	GetPrivateProfileStringW(_section.c_str(), szKey.c_str(), L"", szText, dwSize, m_filename);
	wstring szValue = szText;
	delete [] szText;
	return szValue;
}

string	Ini::get_string_value(const string _section, const string szKey, const DWORD dwSize)
{
	USES_CONVERSION;
	char *szText = new char[dwSize];
	ZeroMemory(szText, dwSize);
	GetPrivateProfileStringA(_section.c_str(), szKey.c_str(), "", szText, dwSize, W2A(m_filename));
	string szValue = szText;
	delete [] szText;
	return szValue;
}

UINT Ini::get_int_value(LPCWSTR _section, LPCWSTR szKey)
{
	return GetPrivateProfileIntW(_section, szKey, 0, m_filename);
}

UINT Ini::get_int_value(LPCSTR _section, LPCSTR szKey)
{
	USES_CONVERSION;
	return GetPrivateProfileIntA(_section, szKey, 0, W2A(m_filename));
}

BOOL Ini::get_bool_value(LPCWSTR _section, LPCWSTR szKey)
{
	wchar_t temp[16] = {0,};
	get_value(_section, szKey, temp, 16);
	if ( (wcscmp(_wcsupr(temp), L"TRUE") == 0) || (wcscmp(_wcsupr(temp), L"YES") == 0) )
		return TRUE;
	return FALSE;
}

BOOL Ini::get_bool_value(LPCSTR _section, LPCSTR szKey)
{
	char temp[16] = {0,};
	get_value(_section, szKey, temp, 16);
	if ( (strcmp(_strlwr(temp), "TRUE") == 0) || (strcmp(_strlwr(temp), "YES") == 0) )
		return TRUE;
	return FALSE;
}

DWORD Ini::get_array_value(LPCWSTR _section, vector< pair<wstring, wstring> > &result, const DWORD dwSize)
{
	result.clear();
	wchar_t *key = new wchar_t[dwSize];
	wchar_t *value = new wchar_t[dwSize];

	pair<wstring, wstring> pairString;

	GetPrivateProfileStringW(_section, NULL, L"", key, dwSize, m_filename);
	for (wchar_t *i = key; *i; i+= wcslen(i)+1)
	{
		ZeroMemory(value, dwSize * sizeof(wchar_t));
		GetPrivateProfileStringW(_section, i, L"", value, dwSize, m_filename);
		pairString.first = i;
		pairString.second = value;
		result.push_back(pairString);
	}
	delete [] key;
	delete [] value;

	return result.size();
}

DWORD Ini::get_array_value(LPCSTR _section, vector< pair<string, string> > &result, const DWORD dwSize)
{
	USES_CONVERSION;
	result.clear();
	char *key = new char[dwSize];
	char *value = new char[dwSize];

	pair<string, string> pairString;

	GetPrivateProfileStringA(_section, NULL, "", key, dwSize, W2A(m_filename));
	for (char *i = key; *i; i+= strlen(i)+1)
	{
		ZeroMemory(value, dwSize);
		GetPrivateProfileStringA(_section, i, "", value, dwSize, W2A(m_filename));
		pairString.first = i;
		pairString.second = value;
		result.push_back(pairString);
	}
	delete [] key;
	delete [] value;

	return result.size();
}

DWORD Ini::getIncreaseValue(LPCWSTR _section, LPCWSTR szKey, vector<wstring> &result, const DWORD dwSize)
{
	result.clear();
	wchar_t *key = new wchar_t[dwSize];
	wchar_t *value = new wchar_t[dwSize];

	int i = 1;
	while(TRUE)
	{
		ZeroMemory(value, dwSize * sizeof(wchar_t));
		swprintf(key, L"%s%d", szKey, i);
		GetPrivateProfileStringW(_section, key, L"", value, dwSize, m_filename);
		if (*value)
			result.push_back(value);
		else
			break;
		i++;
	}
	delete [] key;
	delete [] value;
	return result.size();
}

DWORD Ini::getIncreaseValue(LPCSTR _section, LPCSTR szKey, vector<string> &result, const DWORD dwSize)
{
	USES_CONVERSION;
	result.clear();
	char *key = new char[dwSize];
	char *value = new char[dwSize];

	int i = 1;
	while(TRUE)
	{
		ZeroMemory(value, dwSize);
		sprintf(key, "%s%d", szKey, i);
		GetPrivateProfileStringA(_section, key, "", value, dwSize, W2A(m_filename));
		if (*value)
			result.push_back(value);
		else
			break;
		i++;
	}
	delete [] key;
	delete [] value;
	return result.size();
}

}