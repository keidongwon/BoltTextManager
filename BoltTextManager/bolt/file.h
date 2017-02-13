#pragma once
#include <windows.h>
#include <string>
using namespace std;

namespace bolt 
{

class File
{
public:
	File() {}
	File(LPCSTR filename);
	File(LPCWSTR filename);
	~File()	{ close(); }

	void	init();
	void	set_filename(LPCSTR filename);
	void	set_filename(LPCWSTR filename);
	void	get_filename(LPSTR filename);
	void	get_filename(LPWSTR filename);
	void	sex_max_buffersize(DWORD size);

	DWORD	create(DWORD dwCreation = OPEN_ALWAYS);
	DWORD	create(DWORD dwAccess, DWORD dwShared, DWORD dwCreation);
	BOOL	remove();
	void	close();
	void	set_file_pointer(__int64 pos, DWORD method = FILE_BEGIN);
	BOOL	read(char* buffer, DWORD bytesToRead, LPDWORD bytesRead);
	DWORD	read(char* buffer, DWORD bytesToRead);
	DWORD	read_line(char* buffer);
	DWORD	write(const char* buffer);
	DWORD	write_bytes(const char* buffer, DWORD bytesToWrite);
	DWORD	write_line(const char* buffer);

	HANDLE	get_handle() { return m_hfile; }
	string	get_drive();
	string	get_dir();
	string	get_name();
	string	get_ext();
	string	get_name_with_ext();

	__int64	get_filesize();

	static BOOL		is_exist_file(LPCSTR filename);
	static BOOL		is_exist_file(LPCWSTR filename);
	static BOOL		is_file(LPCSTR filename);
	static BOOL		is_file(LPCWSTR filename);
	static string	get_drive(LPCSTR path);
	static wstring	get_drive(LPCWSTR path);
	static string	get_dir(LPCSTR path);
	static wstring	get_dir(LPCWSTR path);
	static string	get_name(LPCSTR path);
	static wstring	get_name(LPCWSTR path);
	static string	get_ext(LPCSTR path);
	static wstring	get_ext(LPCWSTR path);
	static string	get_name_with_ext(LPCSTR path);
	static wstring	get_name_with_ext(LPCWSTR path);
	static __int64	get_filesize(LPCSTR filename);
	static __int64	get_filesize(LPCWSTR filename);

protected:
	HANDLE m_hfile;
	DWORD dwMaxBufferSize;
	char m_filename[MAX_PATH];
	__int64 filepos;
};

}