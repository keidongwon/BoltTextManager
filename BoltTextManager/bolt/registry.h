#pragma once

#include <string>
#include <vector>
using namespace std;

#ifndef HKCU
#define HKCU HKEY_CURRENT_USER
#endif

#ifndef HKLM
#define HKLM HKEY_LOCAL_MACHINE
#endif

#ifndef LSTATUS
#define LSTATUS LONG
#endif

namespace bolt
{

class Registry
{
private:

public:
	Registry();
	Registry(HKEY hKey, LPCWSTR keyName, REGSAM samDesired = KEY_ALL_ACCESS);
	Registry(HKEY hKey, LPCSTR keyName, REGSAM samDesired = KEY_ALL_ACCESS);
	~Registry();

	operator HKEY() const {	return m_hKey; }
	
	typedef vector<wstring> vtRegWString;	
	typedef vector<string> vtRegString;	

	LSTATUS create(HKEY hKey, LPCWSTR keyName, LPWSTR lpszClass = REG_NONE, DWORD dwOptions = REG_OPTION_NON_VOLATILE,
		REGSAM samDesired = KEY_ALL_ACCESS,	LPSECURITY_ATTRIBUTES lpSecAttr = NULL,	LPDWORD lpdwDisposition = NULL);
	LSTATUS create(HKEY hKey, LPCSTR keyName, LPSTR lpszClass = REG_NONE, DWORD dwOptions = REG_OPTION_NON_VOLATILE,
		REGSAM samDesired = KEY_ALL_ACCESS,	LPSECURITY_ATTRIBUTES lpSecAttr = NULL,	LPDWORD lpdwDisposition = NULL);

	LSTATUS open(HKEY hKey, LPCWSTR keyName, REGSAM samDesired = KEY_ALL_ACCESS);
	LSTATUS open(HKEY hKey, LPCSTR keyName, REGSAM samDesired = KEY_ALL_ACCESS);
	LSTATUS close();

	LSTATUS delete_key(LPCWSTR keyName);
	LSTATUS delete_key(LPCSTR keyName);

	DWORD query_int_value(LPCWSTR valueName);
	DWORD query_int_value(LPCSTR valueName);

	LSTATUS query_value(LPCWSTR valueName, DWORD& dwValue);
	LSTATUS query_value(LPCSTR valueName, DWORD& dwValue);
	LSTATUS query_value(LPCWSTR valueName, LPWSTR szValue, DWORD dwCount = 512);
	LSTATUS query_value(LPCSTR valueName, LPSTR szValue, DWORD dwCount = 512);

	LSTATUS set_value(LPCWSTR valueName, DWORD dwValue);
	LSTATUS set_value(LPCSTR valueName, DWORD dwValue);
	LSTATUS set_value(LPCWSTR valueName, LPCWSTR lpszValue);
	LSTATUS set_value(LPCSTR valueName, LPCSTR lpszValue);

	LSTATUS delete_value(LPCWSTR valueName);
	LSTATUS delete_value(LPCSTR valueName);

	DWORD	get_enum_keys(vtRegWString &keys, DWORD dwSize = 512);
	DWORD	get_enum_keys(vtRegString &keys, DWORD dwSize = 512);
	DWORD	get_enum_values(vtRegWString &values, DWORD dwSize = 512);
	DWORD	get_enum_values(vtRegString &values, DWORD dwSize = 512);

	static LSTATUS WINAPI delete_key(HKEY hKey, LPCWSTR keyName);
	static LSTATUS WINAPI delete_key(HKEY hKey, LPCSTR keyName);
	static LSTATUS WINAPI query_value(HKEY hKey, LPCWSTR keyName, LPCWSTR valueName, DWORD &dwValue);
	static LSTATUS WINAPI query_value(HKEY hKey, LPCSTR keyName, LPCSTR valueName, DWORD &dwValue);
	static LSTATUS WINAPI query_value(HKEY hKey, LPCWSTR keyName, LPCWSTR valueName, LPWSTR szValue, DWORD dwCount = 512);
	static LSTATUS WINAPI query_value(HKEY hKey, LPCSTR keyName, LPCSTR valueName, LPSTR szValue, DWORD dwCount = 512);
	static LSTATUS WINAPI set_value(HKEY hKey, LPCWSTR keyName, LPCWSTR valueName, DWORD dwValue);
	static LSTATUS WINAPI set_value(HKEY hKey, LPCSTR keyName, LPCSTR valueName, DWORD dwValue);
	static LSTATUS WINAPI set_value(HKEY hKey, LPCWSTR keyName, LPCWSTR valueName, LPCWSTR lpszValue);
	static LSTATUS WINAPI set_value(HKEY hKey, LPCSTR keyName, LPCSTR valueName, LPCSTR lpszValue);
	static LSTATUS WINAPI delete_value(HKEY hKey, LPCWSTR keyName, LPCWSTR valueName);
	static LSTATUS WINAPI delete_value(HKEY hKey, LPCSTR keyName, LPCSTR valueName);

protected:
	HKEY m_hKey;
};

}
