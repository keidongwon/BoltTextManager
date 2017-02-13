#pragma once
#include <string>
#include <vector>
#include <locale>
#include <cctype>
#include <algorithm>
#include <iostream>
//#include <boost/program_options/detail/utf8_codecvt_facet.hpp>

#define INVALID_SOURCE			-2
#define CANNOT_FIND_DELIMITER	-1
#define IsHangle(c) ((unsigned char)(c) > 0x7f)

using namespace std;

namespace bolt 
{

class StrUtil {
public:
	StrUtil() {}
	~StrUtil() {}

public:
	static void trim_right(string & value);
	static void trim_right(wstring & value);
	static void trim_left(string & value);
	static void trim_left(wstring & value);
	static void trim(string & value);
	static void trim(wstring & value);
	static string upper(const string & value);
	static wstring upper(const wstring & value);
	static string lower(const string & value);
	static wstring lower(const wstring & value);
	static int str_to_int(const char* value)		{ return atoi(value); }
	static int str_to_int(const wchar_t* value)		{ return _wtoi(value); }
	static int str_to_int(const string &value)		{ return atoi(value.c_str()); }
	static int str_to_int(const wstring &value)		{ return _wtoi(value.c_str()); }
	static long str_to_long(const char* value)		{ return atol(value); }
	static long str_to_long(const wchar_t* value)	{ return _wtol(value); }
	static long str_to_long(const string &value)	{ return atol(value.c_str()); }
	static long str_to_long(const wstring &value)	{ return _wtol(value.c_str()); }
	static string long_to_str(long value);
	static wstring long_to_wstr(long value);
	static string wcs_to_mbs(wstring const& str, locale const& loc = locale());
	static wstring mbs_to_wcs(string const& str, locale const& loc = locale());
	//string ansi_to_utf8(const string &ansi);
	//string utf8_to_ansi(const string &utf8);
	//wstring utf8_to_unicode(const string &utf8);
	//string unicode_to_utf8(const wstring &unicode);
	static int split(const string& str, vector<string>& vec, const string& delimiter);
	static int split(const wstring& str, vector<wstring>& vec, const wstring& delimiter);
	static int token(string& source, string& target, const string& delimiter);
	static int token(wstring& source, wstring& target, const wstring& delimiter);
	static string token(string& source, const string& delimiter);
	static wstring token(wstring& source, const wstring& delimiter);
	static string replaceall(const string& source, const string& pattern, const string& replace);
	static wstring replaceall(const wstring& source, const wstring& pattern, const wstring& replace);
	static bool compare_nocase(const string& s1, const string &s2)		{ return _stricmp(s1.c_str(), s2.c_str()) == 0 ? true : false; }
	static bool compare_nocase(const wstring& s1, const wstring &s2)	{ return _wcsicmp(s1.c_str(), s2.c_str()) == 0 ? true : false; }
	static int divide(string &strsur, string &strdest, char* cdelimiter);
	static int divide(string &strsur, string &strdest, char cdelimiter);
	static string divide(string &strsur, char* cdelimiter);
	static string divide(string &strsur, char cdelimiter);
	static wstring get_parse_string(wstring& value, wchar_t* pDeli);
	static string get_parse_string(string& value, char* pDeli);

	template <class T>
	void print_elements(const T& coll, const char* optcstr = "")
	{
		typename T::const_iterator pos;
		cout << optcstr << endl;
		for (pos = coll.begin(); pos != coll.end(); ++pos)
		{
			cout << *pos << endl;
		}
	}

	template <class T>
	void wprint_elements(const T& coll, const char* optcstr = "")
	{
		typename T::const_iterator pos;
		cout << optcstr << endl;
		for (pos = coll.begin(); pos != coll.end(); ++pos)
		{
			wcout << *pos << endl;
		}
	}
};

}