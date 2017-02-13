#pragma once
#include <fstream>
#include <string>
#include <map>
#include "system.h"

#define BUFFERSIZE					8192

#define TYPE_LOG					L"LOG    "
#define TYPE_ERROR					L"ERROR  "
#define TYPE_INFO					L"INFO   "
#define TYPE_DEBUG					L"DEBUG  "
#define TYPE_TRACE					L"TRACE  "
#define TYPE_VERBOSE				L"VERBOSE"

#define LOG_DM						L"|"
#define LOG_SPACE					L" "

#ifdef _WIN32
#define PREFIX_DIR					L"log\\"
#define DIR_CHAR					L'\\'
#define DIR_LETTER					L"\\"
#else
#define PREFIX_DIR					L"log/"
#define DIR_CHAR					L'/'
#define DIR_LETTER					L"/"
#endif

#define DEV_LOG

enum LogLevel { LV_LOG = 0, LV_ERROR, LV_INFO, LV_DEBUG, LV_TRACE, LV_VERBOSE, LV_MAX }; 

using namespace std;

namespace bolt 
{

class Log
{
public:
	Log()	{ init(); }
	~Log()	{ close(); }
	void init();
	bool create();	// 호출하지 않아도 write 에서 호출됨
	bool create(string path, string filename = "");
	bool create(wstring path, wstring filename = L"");
	void close();	// 호출하지 않아도 소멸자에서 호출됨

public:
	void write();
	void write(const char *fmtstr, ...);
	void write(const wchar_t *fmtstr, ...);
	void write(unsigned int level, const char *fmtstr, ...);
	void write(unsigned int level, const wchar_t *fmtstr, ...);

	void write_line(const char * filename, int line, const char *fmtstr, ...);
	void write_line(const char * filename, int line, const wchar_t *fmtstr, ...);
	void write_line(const char * filename, int line, unsigned int level, const char *fmtstr, ...);
	void write_line(const char * filename, int line, unsigned int level, const wchar_t *fmtstr, ...);
	
	// 디버그 메서드는 운영체제에 따라 다르게 동작한다.
	// stderror는 공통으로 출력하고, 
	// Windows의 경우 OutputDebugString을 출력해준다.
	void debug(const char *fmtstr, ...);
	void debug(const wchar_t *fmtstr, ...);

	void set_writefile(bool flag = true)				{ writefile = flag; }
	void set_outputdebug(bool flag = true)				{ outputdebug = flag; }
	void set_stderror(bool flag = true)					{ stderror = flag; }

	// 지정된 path가 존재하지 않을 경우 make 값(true)에 의해 자동 생성
	// 이 함수를 실행할 경우 uac_safe_folder는 무시된다.
	bool set_path(string path, bool make = true);
	bool set_path(wstring path, bool make = true);
	bool make_path(wstring path, bool make = true);

	// 지정된 path(설정하지 않으면 실행 파일의 current directory) 밑으로 log 폴더를 생성해서 저장
	void set_make_log_folder(bool flag = true)			{ make_log_folder = flag; }
	
	// 이 함수를 사용할 경우 UAC 환경에서 권한 상승 없이 사용할 수 있는 폴더에 로그를 저장
	void set_uac_safe_folder(bool flag = false)			{ uac_safe_folder = flag; }

	// 저장할 파일 이름에 날짜, 시간값을 넣거나 뺀다.
	void set_filename_date(bool flag = true)			{ filename_date = flag; }
	void set_filename_time(bool flag = false)			{ filename_time = flag; }

	wstring get_datetime()								{ DATETIMEW(L"%m-%d %H:%M:%S") }
	wstring get_date()									{ DATETIMEW(L"%Y%m%d") }
	wstring get_time()									{ DATETIMEW(L"%H%M%S") }

	void set_codeline(bool flag = true)					{ codeline = flag; }
	bool show_codeline()								{ return codeline; }
	bool get_logflag()									{ return logflag; }
	unsigned int get_loglevel()							{ return loglevel; }
	void set_loglevel(unsigned int level) 
	{
		if (level <= 0)
			loglevel = LV_ERROR;
		else if (level >= LV_MAX) 
			loglevel = LV_MAX;
		loglevel = level; 
	}

private:
	wofstream fout_log;
	map<unsigned int, wstring> LogPrefix;
	
	wstring logfullpath;
	wstring logpath;
	wstring logfile;
	wstring logext;

	wstring logdate;
	wstring logtime;

	bool logflag;
	bool codeline;

	bool writefile;
	bool outputdebug;
	bool stderror;

	bool make_log_folder;
	bool uac_safe_folder;
	bool filename_date;
	bool filename_time;

	unsigned int loglevel; // loglevel을 사용할 경우 loglevel 에 해당되지 않으면 저장하지 않음
};

}

extern bolt::Log boltlog;

#define NLOG(LV, ...) LV > nLog.get_loglevel() ? 0 : nLog.write_line(__FILE__, __LINE__, LV, __VA_ARGS__)
#define NLOG0(...) LV > nLog.get_loglevel() ? 0 : nLog.write_line(__FILE__, __LINE__, LV_LOG, __VA_ARGS__)
#define NLOG1(...) LV > nLog.get_loglevel() ? 0 : nLog.write_line(__FILE__, __LINE__, LV_ERROR, __VA_ARGS__)
#define NLOG2(...) LV > nLog.get_loglevel() ? 0 : nLog.write_line(__FILE__, __LINE__, LV_INFO, __VA_ARGS__)
#define NLOG3(...) LV > nLog.get_loglevel() ? 0 : nLog.write_line(__FILE__, __LINE__, LV_DEBUG, __VA_ARGS__)
#define NLOG4(...) LV > nLog.get_loglevel() ? 0 : nLog.write_line(__FILE__, __LINE__, LV_TRACE, __VA_ARGS__)
#define NLOG5(...) LV > nLog.get_loglevel() ? 0 : nLog.write_line(__FILE__, __LINE__, LV_VERBOSE, __VA_ARGS__)

#define WRITEA(FMTSTR, LV, FILE, LINE) {\
	char buffer[BUFFERSIZE] = {0,}; \
	va_list vl; \
	va_start(vl, FMTSTR); \
	_vsnprintf_s(buffer, BUFFERSIZE, FMTSTR, vl); \
	va_end(vl); \
	if (outputdebug == true) OutputDebugStringA(buffer); \
	if (stderror == true) cout << buffer << endl; \
	if (writefile == true) \
	{ \
		USES_CONVERSION; \
		if (logdate != get_date()) create(logpath, logfile); \
		if (FILE != NULL) \
			fout_log << get_datetime() << LOG_DM << LogPrefix[LV] << LOG_DM << FILE << L":" << LINE << LOG_DM << LOG_SPACE<< A2W(buffer) << endl; \
		else \
			fout_log << get_datetime() << LOG_DM << LogPrefix[LV] << LOG_DM << LOG_DM << LOG_SPACE << A2W(buffer) << endl; \
	}}

#define WRITEW(FMTSTR, LV, FILE, LINE) {\
	wchar_t buffer[BUFFERSIZE] = {0,}; \
	va_list vl; \
	va_start(vl, FMTSTR); \
	_vsnwprintf_s(buffer, BUFFERSIZE, fmtstr, vl); \
	va_end(vl); \
	if (outputdebug == true) OutputDebugStringW(buffer); \
	if (stderror == true) wcout << buffer << endl; \
	if (writefile == true) \
	{ \
		if (logdate != get_date()) create(logpath, logfile); \
		if (FILE != NULL) \
			fout_log << get_datetime() << LOG_DM << LogPrefix[LV] << LOG_DM << FILE << L":" << LINE << LOG_DM << LOG_SPACE << buffer << endl; \
		else \
			fout_log << get_datetime() << LOG_DM << LogPrefix[LV] << LOG_DM << LOG_DM << LOG_SPACE << buffer << endl; \
	}}

#ifdef DEV_LOG
#define NOUT(LOGMSG) nLog.debug(LOGMSG);
#else
#define NOUT(LOGMSG) 
#endif
