#include "stdafx.h"
#include "log.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <locale>
#include <ShlObj.h>
//#include <boost/program_options/detail/utf8_codecvt_facet.hpp>
//#include <boost/filesystem.hpp>
#include "win32sys.h"
#include "strutil.h"
#include "file.h"

bolt::Log boltlog;

namespace bolt 
{

void Log::init()
{
	logfullpath				= L"";
	logfile					= L"";
	logpath					= L"";
	logext					= L".log";
	logdate					= L"";
	logtime					= L"";
	loglevel				= 1;

	logflag					= true;
	filename_date			= true;
	filename_time			= false;
	writefile				= true;
	outputdebug				= true;
	stderror				= true;
	make_log_folder			= true;
	uac_safe_folder			= false;
	codeline				= false;

	LogPrefix.clear();
	LogPrefix[LV_LOG]		= TYPE_LOG;
	LogPrefix[LV_ERROR]		= TYPE_ERROR;
	LogPrefix[LV_INFO]		= TYPE_INFO;
	LogPrefix[LV_DEBUG]		= TYPE_DEBUG;
	LogPrefix[LV_TRACE]		= TYPE_TRACE;
	LogPrefix[LV_VERBOSE]	= TYPE_VERBOSE;
}

bool Log::create()
{
	return create(L"", L"");
}

bool Log::create(string path, string filename)
{
	return create(StrUtil::mbs_to_wcs(path), StrUtil::mbs_to_wcs(filename));
}

bool Log::create(wstring path, wstring filename)
{
	if (writefile == false) return false;

	wstring m_filename = filename;
	wstring m_path = path;
	logfile = filename;

	// 파일 이름이 설정되지 않은 경우, 모듈의 이름을 가져온다.
	if (filename.empty() == true) m_filename = get_module_name(NULL);

	// 로그 폴더가 지정되지 않은 경우, 
	// UAC 설정에 따라 사용자\모듈 경로를 가져오거나, 모듈의 경로를 가져온다. 
	if (m_path.empty() == true)
	{
		if (uac_safe_folder == true)
		{
			m_path = get_environment(L"APPDATA") + DIR_LETTER + m_filename;
		}
		else
		{
			m_path = get_module_path();
		}
		// 로그 폴더의 존재 여부를 확인한 후 없으면 생성한다. 
		if (make_path(m_path) == false) return false;
	}
	else
	{
		// 로그 폴더의 존재 여부를 확인한 후 없으면 생성하고, logpath를 설정한다.
		if (set_path(m_path) == false) return false;
	}

	// path의 마지막 부분에 \ 이 없을 경우 추가해준다.
	if (m_path[m_path.length() - 1] != DIR_CHAR) m_path.append(DIR_LETTER);

	// 파일 이름에 날짜, 시간을 추가하는 부분
	if (filename_date == true) m_filename = m_filename + L"_" + get_date();
	if (filename_time == true) m_filename = m_filename + L"_" + get_time();

	logdate = get_date();

	wstring m_log_path = m_path;
	if (make_log_folder == true) m_log_path += PREFIX_DIR;
	m_log_path = m_log_path + m_filename + logext;

	if (fout_log.is_open() == true) fout_log.close();
	fout_log.open(m_log_path, ios_base::out | ios_base::app);
	//std::locale nlog_loc_utf8(std::locale(""), new boost::program_options::detail::utf8_codecvt_facet);
	//fout_log.imbue(nlog_loc_utf8);
	//fout_log << wchar_t(0xFEFF); // UTF-8 BOM 설정
	return true;
}
	//bool utf8_set = false;
	//if (boost::filesystem::exists(m_log_path) == false) utf8_set = true;
	//if (utf8_set == true) fout << wchar_t(0xFEFF);

void Log::close()
{
	if (fout_log.is_open() == true) 
	{
		fout_log.flush();
		fout_log.close();
	}
}

void Log::write()
{
	if (stderror == true) wcout << endl;
	if (writefile == true) 
	{
		if (logdate != get_date()) create(logpath, logfile);
		fout_log << get_datetime() << endl;
	}
}

void Log::write(const char *fmtstr, ...)
{
	WRITEA(fmtstr, LV_LOG, NULL, 0)
}

void Log::write(const wchar_t *fmtstr, ...)
{
	WRITEW(fmtstr, LV_LOG, NULL, 0)
}

void Log::write(unsigned int level, const char *fmtstr, ...)
{
	if (level > loglevel) return;
	WRITEA(fmtstr, level, NULL, 0)
}

void Log::write(unsigned int level, const wchar_t *fmtstr, ...)
{
	if (level > loglevel) return;
	WRITEW(fmtstr, level, NULL, 0)
}

void Log::write_line(const char * filename, int line, const char *fmtstr, ...)
{
	if (codeline == true)
		WRITEA(fmtstr, LV_LOG, filename, line)
	else
		WRITEA(fmtstr, LV_LOG, NULL, 0)
}

void Log::write_line(const char * filename, int line, const wchar_t *fmtstr, ...)
{
	if (codeline == true)
		WRITEW(fmtstr, LV_LOG, filename, line)
	else
		WRITEW(fmtstr, LV_LOG, NULL, 0)
}

void Log::write_line(const char * filename, int line, unsigned int level, const char *fmtstr, ...)
{
	if (codeline == true)
		WRITEA(fmtstr, level, filename, line)
	else
		WRITEA(fmtstr, level, NULL, 0)
}

void Log::write_line(const char * filename, int line, unsigned int level, const wchar_t *fmtstr, ...)
{
	if (codeline == true)
		WRITEW(fmtstr, level, filename, line)
	else
		WRITEW(fmtstr, level, NULL, 0)
}

void Log::debug(const char *fmtstr, ...)
{
	char buffer[BUFFERSIZE] = {0,};
	va_list vl;
	va_start(vl, fmtstr);
	_vsnprintf_s(buffer, BUFFERSIZE, fmtstr, vl);
	va_end(vl);
	if (outputdebug == true) OutputDebugStringA(buffer);
	if (stderror == true) cout << buffer << endl;
}

void Log::debug(const wchar_t *fmtstr, ...)
{
	wchar_t widebuff[BUFFERSIZE] = {0,};
	va_list vl;
	va_start(vl, fmtstr);
	_vsnwprintf_s(widebuff, BUFFERSIZE, fmtstr, vl);
	va_end(vl);
	if (outputdebug == true) OutputDebugStringW(widebuff);
	if (stderror == true) wcout << widebuff << endl;
}

bool Log::set_path(string path, bool make)
{
	return set_path(StrUtil::mbs_to_wcs(path), make); 
}

bool Log::set_path(wstring path, bool make)
{
	if (path.empty() == true) return false;
	if (make_path(path, make) == false) return false;
	logpath = path;
	set_uac_safe_folder(false);
	return true;
}

inline bool Log::make_path(wstring path, bool make)
{
	wstring makepath = path;
	if (makepath[makepath.length() - 1] != DIR_CHAR) makepath.append(DIR_LETTER);
	if (make_log_folder == true) makepath += PREFIX_DIR;
	//if (boost::filesystem::exists(makepath) == false)
	if (File::is_exist_file(makepath.c_str()) == FALSE)
	{
		if (make == false) return false;
		//	boost::filesystem::create_directories(makepath);
		SHCreateDirectoryExW( NULL, makepath.c_str(), NULL );
	}
	return true;
}

}