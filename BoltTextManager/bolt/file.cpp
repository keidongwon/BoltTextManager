#include "stdafx.h"
#include "file.h"

namespace bolt 
{

File::File(LPCSTR filename)
{
	init();
	strncpy(m_filename, filename, MAX_PATH);
}

File::File(LPCWSTR filename)
{
	init();
	USES_CONVERSION;
	strncpy(m_filename, W2A(filename), MAX_PATH);
}

void File::init()
{
	filepos = 0;
	dwMaxBufferSize = 8192;
	m_hfile = NULL;
	ZeroMemory(m_filename, MAX_PATH);
}

void File::set_filename(LPCSTR filename)
{
	if (filename == NULL || strlen(filename) == 0) return;
	strncpy(m_filename, filename, MAX_PATH);
}

void File::set_filename(LPCWSTR filename)
{
	if (filename == NULL || wcslen(filename) == 0) return;
	USES_CONVERSION;
	set_filename(W2A(filename));
}

void File::get_filename(LPSTR filename)
{
	if (m_filename == NULL || strlen(m_filename) == 0) return;
	strncpy(filename, m_filename, MAX_PATH);
}

void File::get_filename(LPWSTR filename)
{
	if (m_filename == NULL || strlen(m_filename) == 0) return;
	USES_CONVERSION;
	wcsncpy(filename, A2W(m_filename), MAX_PATH);
}

void File::sex_max_buffersize(DWORD size)
{
	dwMaxBufferSize = size;
}

DWORD File::create(DWORD dwCreation)
{
	m_hfile = ::CreateFileA(m_filename,
		GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_WRITE | FILE_SHARE_READ,
		NULL,
		dwCreation,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (m_hfile == INVALID_HANDLE_VALUE) 
	{
		DWORD error = GetLastError();
		return error;
	}
	return ERROR_SUCCESS;
}

DWORD File::create(DWORD dwAccess, DWORD dwShared, DWORD dwCreation)
{
	m_hfile = ::CreateFileA(m_filename,
		dwAccess, 
		dwShared,
		NULL,
		dwCreation,
		FILE_ATTRIBUTE_NORMAL,
		NULL);


	if (m_hfile == INVALID_HANDLE_VALUE) 
	{
		DWORD error = GetLastError();
		return error;
	}
	return ERROR_SUCCESS;
}

BOOL File::remove()
{
	close();
	return ::DeleteFileA(m_filename);
}

void File::close()
{
	if (m_hfile)
	{
		CloseHandle(m_hfile);
		m_hfile = NULL;
	}
}

void File::set_file_pointer(__int64 pos, DWORD method)
{
	if (m_hfile == INVALID_HANDLE_VALUE) return;

	LARGE_INTEGER li;
	li.QuadPart = pos;
	SetFilePointer(m_hfile, li.LowPart, &li.HighPart, method);
}

DWORD File::read(char* buffer, DWORD bytesToRead)
{
	DWORD bytesRead;
	ReadFile(m_hfile, buffer, bytesToRead, &bytesRead, NULL);
	return bytesRead;
}

BOOL File::read(char* buffer, DWORD bytesToRead, LPDWORD bytesRead)
{
	if (ReadFile(m_hfile, buffer, bytesToRead, bytesRead, NULL) == 0) return FALSE;
	if (*bytesRead == 0) return FALSE;
	return TRUE;
}

DWORD File::read_line(char* buffer)
{
	char *data = new char[dwMaxBufferSize];
	ZeroMemory(data, dwMaxBufferSize);
	char mem[8];
	ZeroMemory(mem, 8);
	DWORD bytesRead = 0;
	DWORD total = 0;
	strcpy(data, mem);
	DWORD result = 0;

	for (int i=0; i<(int)dwMaxBufferSize; i++)
	{
		result = ReadFile(m_hfile, mem, 1, &bytesRead, NULL);
		if ((result == 0) || ((result != 0) && (bytesRead == 0)))
			break;

		if (strcmp(mem, "\n") == 0)
			break;

		if (strcmp(mem, "\n") == 0)
		{
			if (strlen(buffer) == 0) 
				continue;

			break;
		}

		strcat(data, mem);
		ZeroMemory(mem, 8);
		total++;
	}
	strcpy(buffer, data);
	delete [] data;

	return total;
}

DWORD File::write(const char* buffer)
{
	DWORD bytesWritten = 0;
	size_t bytesToWrite = strlen(buffer);
	WriteFile(m_hfile, buffer, bytesToWrite, &bytesWritten, NULL);
	return bytesWritten;
}

DWORD File::write_bytes(const char* buffer, DWORD bytesToWrite)
{
	DWORD bytesWritten = 0;
	WriteFile(m_hfile, buffer, bytesToWrite, &bytesWritten, NULL);
	return bytesWritten;
}

DWORD File::write_line(const char* buffer)
{
	DWORD bytesWritten = 0;
	size_t bytesToWrite = strlen(buffer);
	char *data = new char[bytesToWrite+2];
	ZeroMemory(data, bytesToWrite+2);
	strncpy(data, buffer, bytesToWrite);
	data[bytesToWrite] = '\r';
	data[bytesToWrite+1] = '\n';
	WriteFile(m_hfile, data, bytesToWrite+2, &bytesWritten, NULL);
	delete [] data;
	return bytesWritten;
}

string File::get_drive()
{
	if (m_filename == NULL || strlen(m_filename) == 0) return "";
	char text[MAX_PATH] = {0,};
	_splitpath(m_filename, text, NULL, NULL, NULL);
	return text;
}

string File::get_dir()
{
	if (m_filename == NULL || strlen(m_filename) == 0) return "";
	char text[MAX_PATH] = {0,};
	_splitpath(m_filename, NULL, text, NULL, NULL);
	return text;
}

string File::get_name()
{
	if (m_filename == NULL || strlen(m_filename) == 0) return "";
	char text[MAX_PATH] = {0,};
	_splitpath(m_filename, NULL, NULL, text, NULL);
	return text;
}

string File::get_ext()
{
	if (m_filename == NULL || strlen(m_filename) == 0) return "";
	char text[MAX_PATH] = {0,};
	_splitpath(m_filename, NULL, NULL, NULL, text);
	return text;
}

string File::get_name_with_ext()
{
	if (m_filename == NULL || strlen(m_filename) == 0) return "";
	char text[MAX_PATH] = {0,};
	char filename[MAX_PATH];
	char ext[MAX_PATH];
	_splitpath(m_filename, NULL, NULL, filename, ext);
	sprintf(text, "%s%s", filename, ext);
	return text;
}

__int64 File::get_filesize()
{
	if (m_hfile == INVALID_HANDLE_VALUE) return -1;

	unsigned long nSizeHigh = 0;
	unsigned long nSizeLow = 0;
	__int64 filesize = 0;
	nSizeLow = ::GetFileSize(m_hfile, &nSizeHigh);
	filesize = nSizeHigh;
	filesize <<= 32;
	filesize |= nSizeLow;
	return filesize;
}


// static

BOOL File::is_exist_file(LPCSTR filename)
{
	USES_CONVERSION;
	return is_exist_file(A2W(filename));
}

BOOL File::is_exist_file(LPCWSTR filename)
{
	HANDLE hSearch;
	WIN32_FIND_DATAW wfd;
	hSearch = FindFirstFileW(filename, &wfd);
	return hSearch == INVALID_HANDLE_VALUE ? FALSE : TRUE;
}

BOOL File::is_file(LPCSTR filename)
{
	USES_CONVERSION;
	return is_file(A2W(filename));
}

BOOL File::is_file(LPCWSTR filename)
{
	CFileFind finder;
	if (finder.FindFile(filename) == FALSE) return FALSE;
	finder.FindNextFile();
	//if (finder.FindNextFile() == FALSE) return FALSE;
	if (finder.IsDirectory() == TRUE) return FALSE;
	return TRUE;
}

string File::get_drive(LPCSTR path)
{
	if (!path) return "";
	char text[MAX_PATH] = {0,};
	_splitpath(path, text, NULL, NULL, NULL);
	return text;
}

wstring File::get_drive(LPCWSTR path)
{
	if (!path) return L"";
	wchar_t text[MAX_PATH] = {0,};
	_wsplitpath(path, text, NULL, NULL, NULL);
	return text;
}

string File::get_dir(LPCSTR path)
{
	if (!path) return "";
	char text[MAX_PATH] = {0,};
	_splitpath(path, NULL, text, NULL, NULL);
	return text;
}

wstring File::get_dir(LPCWSTR path)
{
	if (!path) return L"";
	wchar_t text[MAX_PATH] = {0,};
	_wsplitpath(path, NULL, text, NULL, NULL);
	return text;
}

string File::get_name(LPCSTR path)
{
	if (!path) return "";
	char text[MAX_PATH] = {0,};
	_splitpath(path, NULL, NULL, text, NULL);
	return text;
}

wstring File::get_name(LPCWSTR path)
{
	if (!path) return L"";
	wchar_t text[MAX_PATH] = {0,};
	_wsplitpath(path, NULL, NULL, text, NULL);
	return text;
}

string File::get_ext(LPCSTR path)
{
	if (!path) return "";
	char text[MAX_PATH] = {0,};
	_splitpath(path, NULL, NULL, NULL, text);
	return text;
}

wstring File::get_ext(LPCWSTR path)
{
	if (!path) return L"";
	wchar_t text[MAX_PATH] = {0,};
	_wsplitpath(path, NULL, NULL, NULL, text);
	return text;
}

string File::get_name_with_ext(LPCSTR path)
{
	if (!path) return "";
	char filename[MAX_PATH];
	char ext[MAX_PATH];
	char text[MAX_PATH] = {0,};
	_splitpath(path, NULL, NULL, filename, ext);
	sprintf(text, "%s%s", filename, ext);
	return text;
}

wstring File::get_name_with_ext(LPCWSTR path)
{
	if (!path) return L"";
	wchar_t filename[MAX_PATH];
	wchar_t ext[MAX_PATH];
	wchar_t text[MAX_PATH] = {0,};
	_wsplitpath(path, NULL, NULL, filename, ext);
	swprintf(text, L"%s%s", filename, ext);
	return text;
}

__int64 File::get_filesize(LPCSTR filename)
{
	USES_CONVERSION;
	return get_filesize(A2W(filename));
}

__int64 File::get_filesize(LPCWSTR filename)
{
	unsigned long nSizeHigh = 0;
	unsigned long nSizeLow = 0;
	__int64 filesize = 0;

	HANDLE hFileSize = CreateFileW(filename, 
		GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_WRITE | FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFileSize == INVALID_HANDLE_VALUE) return -1;
	nSizeLow = ::GetFileSize(hFileSize, &nSizeHigh);
	filesize = nSizeHigh;
	filesize <<= 32;
	filesize |= nSizeLow;

	CloseHandle(hFileSize);
	return filesize;
}

}