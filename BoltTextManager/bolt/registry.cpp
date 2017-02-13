#include "stdafx.h"
#include "registry.h"
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib") 

namespace bolt
{

Registry::Registry()
{
	m_hKey = NULL;
}

Registry::Registry(HKEY hKey, LPCWSTR keyName, REGSAM samDesired)
{
	m_hKey = NULL;
	open(hKey, keyName, samDesired);
}

Registry::Registry(HKEY hKey, LPCSTR keyName, REGSAM samDesired)
{
	m_hKey = NULL;
	open(hKey, keyName, samDesired);
}

Registry::~Registry()
{
	close();
}

LSTATUS Registry::create(HKEY hKey, LPCWSTR keyName, LPWSTR lpszClass, DWORD dwOptions, REGSAM samDesired, 
						 LPSECURITY_ATTRIBUTES lpSecAttr, LPDWORD lpdwDisposition)
{
	ASSERT(hKey != NULL);
	DWORD dwDisposition;
	LSTATUS lStatus = RegCreateKeyExW(hKey, keyName, 0, lpszClass, dwOptions, samDesired, lpSecAttr, &hKey, &dwDisposition);
	if (lpdwDisposition != NULL)
		*lpdwDisposition = dwDisposition;
	if (lStatus != ERROR_SUCCESS) close();
	m_hKey = hKey;
	return lStatus;
}

LSTATUS Registry::create(HKEY hKey, LPCSTR keyName, LPSTR lpszClass, DWORD dwOptions, REGSAM samDesired, 
						 LPSECURITY_ATTRIBUTES lpSecAttr, LPDWORD lpdwDisposition)
{
	ASSERT(hKey != NULL);
	DWORD dwDisposition;
	LSTATUS lStatus = RegCreateKeyExA(hKey, keyName, 0, lpszClass, dwOptions, samDesired, lpSecAttr, &hKey, &dwDisposition);
	if (lpdwDisposition != NULL)
		*lpdwDisposition = dwDisposition;
	if (lStatus != ERROR_SUCCESS) close();
	m_hKey = hKey;
	return lStatus;
}

LSTATUS Registry::open(HKEY hKey, LPCWSTR keyName, REGSAM samDesired)
{
	ASSERT(hKey != NULL);
	LSTATUS lStatus = RegOpenKeyExW(hKey, keyName, 0, samDesired, &hKey);
	if (lStatus != ERROR_SUCCESS) close();
	m_hKey = hKey;
	return lStatus;
}

LSTATUS Registry::open(HKEY hKey, LPCSTR keyName, REGSAM samDesired)
{
	ASSERT(hKey != NULL);
	LSTATUS lStatus = RegOpenKeyExA(hKey, keyName, 0, samDesired, &hKey);
	if (lStatus != ERROR_SUCCESS) close();
	m_hKey = hKey;
	return lStatus;
}

LSTATUS Registry::close()
{
	if (m_hKey) 
		return RegCloseKey(m_hKey);
	return -1; 
}

LSTATUS Registry::delete_key(LPCWSTR keyName) 
{
	return SHDeleteKeyW(m_hKey, keyName); //return RegDeleteKey(m_hKey, valueName);
}

LSTATUS Registry::delete_key(LPCSTR keyName)
{
	return SHDeleteKeyA(m_hKey, keyName);
}

DWORD Registry::query_int_value(LPCWSTR valueName)
{
	DWORD dwValue;
	ASSERT(m_hKey != NULL);
	DWORD dwType = NULL;
	DWORD dwCount = sizeof(DWORD);
	RegQueryValueExW(m_hKey, (LPWSTR)valueName, NULL, &dwType, (LPBYTE)&dwValue, &dwCount);
	return dwValue;
}

DWORD Registry::query_int_value(LPCSTR valueName)
{
	DWORD dwValue;
	ASSERT(m_hKey != NULL);
	DWORD dwType = NULL;
	DWORD dwCount = sizeof(DWORD);
	RegQueryValueExA(m_hKey, (LPSTR)valueName, NULL, &dwType, (LPBYTE)&dwValue, &dwCount);
	return dwValue;
}

LSTATUS Registry::query_value(LPCWSTR valueName, DWORD& dwValue)
{
	ASSERT(m_hKey != NULL);
	DWORD dwType = NULL;
	DWORD dwCount = sizeof(DWORD);
	return RegQueryValueExW(m_hKey, (LPWSTR)valueName, NULL, &dwType, (LPBYTE)&dwValue, &dwCount);
}

LSTATUS Registry::query_value(LPCSTR valueName, DWORD& dwValue)
{
	ASSERT(m_hKey != NULL);
	DWORD dwType = NULL;
	DWORD dwCount = sizeof(DWORD);
	return RegQueryValueExA(m_hKey, (LPSTR)valueName, NULL, &dwType, (LPBYTE)&dwValue, &dwCount);
}

LSTATUS Registry::query_value(LPCWSTR valueName, LPWSTR szValue, DWORD dwCount)
{
	ASSERT(m_hKey != NULL);
	DWORD dwType = NULL;
	return RegQueryValueExW(m_hKey, (LPWSTR)valueName, NULL, &dwType, (LPBYTE)szValue, &dwCount);
}

LSTATUS Registry::query_value(LPCSTR valueName, LPSTR szValue, DWORD dwCount)
{
	ASSERT(m_hKey != NULL);
	DWORD dwType = NULL;
	return RegQueryValueExA(m_hKey, (LPSTR)valueName, NULL, &dwType, (LPBYTE)szValue, &dwCount);
}

LSTATUS Registry::set_value(LPCWSTR valueName, DWORD dwValue)
{
	ASSERT(m_hKey != NULL);
	return RegSetValueExW(m_hKey, valueName, NULL, REG_DWORD, (BYTE * const)&dwValue, sizeof(DWORD));
}

LSTATUS Registry::set_value(LPCSTR valueName, DWORD dwValue)
{
	ASSERT(m_hKey != NULL);
	return RegSetValueExA(m_hKey, valueName, NULL, REG_DWORD, (BYTE * const)&dwValue, sizeof(DWORD));
}

LSTATUS Registry::set_value(LPCWSTR valueName, LPCWSTR lpszValue)
{
	ASSERT(m_hKey != NULL);
	return RegSetValueExW(m_hKey, valueName, NULL, REG_SZ, (BYTE * const)lpszValue, (wcslen(lpszValue)+1) * sizeof(wchar_t));
}

LSTATUS Registry::set_value(LPCSTR valueName, LPCSTR lpszValue)
{
	ASSERT(m_hKey != NULL);
	return RegSetValueExA(m_hKey, valueName, NULL, REG_SZ, (BYTE * const)lpszValue, (strlen(lpszValue)+1));
}

LSTATUS Registry::delete_value(LPCWSTR valueName)
{
	ASSERT(m_hKey != NULL);
	return RegDeleteValueW(m_hKey, valueName);
}

LSTATUS Registry::delete_value(LPCSTR valueName)
{
	ASSERT(m_hKey != NULL);
	return RegDeleteValueA(m_hKey, valueName);
}

DWORD Registry::get_enum_keys(vtRegWString &keys, DWORD dwSize)
{
	ASSERT(m_hKey != NULL);
	keys.clear();
	DWORD index = 0;
	DWORD dwReturn = 0;
	DWORD dwSubKeyLength;
	wchar_t *szSubKey = new wchar_t[dwSize];
	ZeroMemory(szSubKey, dwSize);
	while (dwReturn == ERROR_SUCCESS)
	{
		dwSubKeyLength = dwSize;
		dwReturn = RegEnumKeyExW(m_hKey, index, szSubKey, &dwSubKeyLength, NULL, NULL, NULL, NULL);
		if(dwReturn == ERROR_NO_MORE_ITEMS)	break;
		if (dwReturn == ERROR_SUCCESS) keys.push_back(szSubKey);
		ZeroMemory(szSubKey, dwSize);
		index++;
	}
	delete [] szSubKey;
	return (DWORD)keys.size();
}

DWORD Registry::get_enum_keys(vtRegString &keys, DWORD dwSize)
{
	ASSERT(m_hKey != NULL);
	keys.clear();
	DWORD index = 0;
	DWORD dwReturn = 0;
	DWORD dwSubKeyLength;
	char *szSubKey = new char[dwSize];
	while (dwReturn == ERROR_SUCCESS)
	{
		dwSubKeyLength = dwSize;
		dwReturn = RegEnumKeyExA(m_hKey, index, szSubKey, &dwSubKeyLength, NULL, NULL, NULL, NULL);
		if(dwReturn == ERROR_NO_MORE_ITEMS)	break;
		if (dwReturn == ERROR_SUCCESS) keys.push_back(szSubKey);
		ZeroMemory(szSubKey, dwSize);
		index++;
	}
	delete [] szSubKey;
	return (DWORD)keys.size();
}

DWORD Registry::get_enum_values(vtRegWString &values, DWORD dwSize)
{
	ASSERT(m_hKey != NULL);
	values.clear();
	DWORD index = 0;
	DWORD dwReturn = 0;
	DWORD dwValueLength;
	wchar_t *szValue = new wchar_t[dwSize];
	ZeroMemory(szValue, dwSize);
	while (dwReturn == ERROR_SUCCESS)
	{
		dwValueLength = dwSize;
		dwReturn = RegEnumValueW(m_hKey, index, szValue, &dwValueLength, NULL, NULL, NULL, NULL);
		if(dwReturn == ERROR_NO_MORE_ITEMS)	break;
		if (dwReturn == ERROR_SUCCESS) values.push_back(szValue);
		ZeroMemory(szValue, dwSize);
		index++;
	}
	delete [] szValue;
	return (DWORD)values.size();
}

DWORD Registry::get_enum_values(vtRegString &values, DWORD dwSize)
{
	ASSERT(m_hKey != NULL);
	values.clear();
	DWORD index = 0;
	DWORD dwReturn = 0;
	DWORD dwValueLength;
	char *szValue = new char[dwSize];
	ZeroMemory(szValue, dwSize);
	while (dwReturn == ERROR_SUCCESS)
	{
		dwValueLength = dwSize;
		dwReturn = RegEnumValueA(m_hKey, index, szValue, &dwValueLength, NULL, NULL, NULL, NULL);
		if(dwReturn == ERROR_NO_MORE_ITEMS)	break;
		if (dwReturn == ERROR_SUCCESS) values.push_back(szValue);
		ZeroMemory(szValue, dwSize);
		index++;
	}
	delete [] szValue;
	return (DWORD)values.size();
}


// static 
LSTATUS WINAPI Registry::delete_key(HKEY hKey, LPCWSTR keyName)
{
	Registry reg;
	LSTATUS lStatus = reg.open(hKey, keyName);
	if (lStatus == ERROR_SUCCESS)
		return reg.delete_key(keyName);
	return lStatus;
}

LSTATUS WINAPI Registry::delete_key(HKEY hKey, LPCSTR keyName)
{
	Registry reg;
	LSTATUS lStatus = reg.open(hKey, keyName);
	if (lStatus == ERROR_SUCCESS)
		return reg.delete_key(keyName);
	return lStatus;
}

LSTATUS WINAPI Registry::query_value(HKEY hKey, LPCWSTR keyName, LPCWSTR valueName, DWORD &dwValue)
{
	Registry reg;
	LSTATUS lStatus = reg.open(hKey, keyName, KEY_READ);
	if (lStatus == ERROR_SUCCESS)
		return reg.query_value(valueName, dwValue);
	return lStatus;
}

LSTATUS WINAPI Registry::query_value(HKEY hKey, LPCSTR keyName, LPCSTR valueName, DWORD &dwValue)
{
	Registry reg;
	LSTATUS lStatus = reg.open(hKey, keyName, KEY_READ);
	if (lStatus == ERROR_SUCCESS)
		return reg.query_value(valueName, dwValue);
	return lStatus;
}

LSTATUS WINAPI Registry::query_value(HKEY hKey, LPCWSTR keyName, LPCWSTR valueName, LPWSTR szValue, DWORD dwCount)
{
	Registry reg;
	LSTATUS lStatus = reg.open(hKey, keyName, KEY_READ);
	if (lStatus == ERROR_SUCCESS)
		return reg.query_value(valueName, szValue, dwCount);
	return lStatus;
}

LSTATUS WINAPI Registry::query_value(HKEY hKey, LPCSTR keyName, LPCSTR valueName, LPSTR szValue, DWORD dwCount)
{
	Registry reg;
	LSTATUS lStatus = reg.open(hKey, keyName, KEY_READ);
	if (lStatus == ERROR_SUCCESS)
		return reg.query_value(valueName, szValue, dwCount);
	return lStatus;
}
LSTATUS WINAPI Registry::set_value(HKEY hKey, LPCWSTR keyName, LPCWSTR valueName, DWORD dwValue)
{
	Registry reg;
	LSTATUS lStatus = reg.create(hKey, keyName);
	if (lStatus == ERROR_SUCCESS)
		return reg.set_value(valueName, dwValue);
	return lStatus;
}

LSTATUS WINAPI Registry::set_value(HKEY hKey, LPCSTR keyName, LPCSTR valueName, DWORD dwValue)
{
	Registry reg;
	LSTATUS lStatus = reg.create(hKey, keyName);
	if (lStatus == ERROR_SUCCESS)
		return reg.set_value(valueName, dwValue);
	return lStatus;
}

LSTATUS WINAPI Registry::set_value(HKEY hKey, LPCWSTR keyName, LPCWSTR valueName, LPCWSTR lpszValue)
{
	Registry reg;
	LSTATUS lStatus = reg.create(hKey, keyName);
	if (lStatus == ERROR_SUCCESS)
		return reg.set_value(valueName, lpszValue);
	return lStatus;
}

LSTATUS WINAPI Registry::set_value(HKEY hKey, LPCSTR keyName, LPCSTR valueName, LPCSTR lpszValue)
{
	Registry reg;
	LSTATUS lStatus = reg.create(hKey, keyName);
	if (lStatus == ERROR_SUCCESS)
		return reg.set_value(valueName, lpszValue);
	return lStatus;
}

LSTATUS WINAPI Registry::delete_value(HKEY hKey, LPCWSTR keyName, LPCWSTR valueName)
{
	Registry reg;
	LSTATUS lStatus = reg.create(hKey, keyName);
	if (lStatus == ERROR_SUCCESS)
		return reg.delete_value(valueName);
	return lStatus;
}

LSTATUS WINAPI Registry::delete_value(HKEY hKey, LPCSTR keyName, LPCSTR valueName)
{
	Registry reg;
	LSTATUS lStatus = reg.create(hKey, keyName);
	if (lStatus == ERROR_SUCCESS)
		return reg.delete_value(valueName);
	return lStatus;
}

}
