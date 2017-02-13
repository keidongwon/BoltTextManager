#include "stdafx.h"
#include "strutil.h"
#include <windows.h>

namespace bolt 
{

//std::locale loc_utf8(std::locale(""), new boost::program_options::detail::utf8_codecvt_facet);
std::locale loc(std::locale(""));

void StrUtil::trim_right(string & value)
{
	if (value.length() == 0) return;
	size_t n = value.find_last_not_of(" \t\v\n"); 
	if (n != string::npos) value = value.substr(0, n + 1); 
}

void StrUtil::trim_right(wstring & value)
{
	if (value.length() == 0) return;
	size_t n = value.find_last_not_of(L" \t\v\n"); 
	if (n != wstring::npos) value = value.substr(0, n + 1); 
}

void StrUtil::trim_left(string & value)
{
	if (value.length() == 0) return;
	size_t n = value.find_first_not_of(" \t\v\n"); 
	if (n != string::npos) value = value.substr(n, value.length());
}

void StrUtil::trim_left(wstring & value)
{
	if (value.length() == 0) return;
	size_t n = value.find_first_not_of(L" \t\v\n"); 
	if (n != wstring::npos) value = value.substr(n, value.length()); 
}

void StrUtil::trim(string & value)
{
	if (value.length() == 0) return;
	std::size_t beg = value.find_first_not_of(" \a\b\f\n\r\t\v");
	std::size_t end = value.find_last_not_of(" \a\b\f\n\r\t\v");
	if (beg == std::string::npos) return; // No non-spaces   
	value = std::string(value, beg, end - beg + 1);
}

void StrUtil::trim(wstring & value)
{
	if (value.length() == 0) return;
	std::size_t beg = value.find_first_not_of(L" \a\b\f\n\r\t\v");
	std::size_t end = value.find_last_not_of(L" \a\b\f\n\r\t\v");
	if (beg == std::wstring::npos) return; // No non-spaces   
	value = std::wstring(value, beg, end - beg + 1); 
}

string StrUtil::upper(const string & value)
{
	string text = value;
	std::transform(text.begin(), text.end(), text.begin(), toupper);
	return text;
}

wstring StrUtil::upper(const wstring & value)
{
	wstring text = value;
	std::transform(text.begin(), text.end(), text.begin(), toupper);
	return text;
}

string StrUtil::lower(const string & value)
{
	string text = value;
	std::transform(text.begin(), text.end(), text.begin(), tolower);
	return text;
}

wstring StrUtil::lower(const wstring & value)
{
	wstring text = value;
	std::transform(text.begin(), text.end(), text.begin(), tolower);
	return text;
}

string StrUtil::long_to_str(long value)
{
	char buf[256] = {0};
	_ltoa_s(value, buf, 10);
	return buf;
}

wstring StrUtil::long_to_wstr(long value)
{
	wchar_t buf[256] = {0};
	_ltow_s(value, buf, 10);
	return buf;
}

string StrUtil::wcs_to_mbs(wstring const& str, locale const& loc)
{
	typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_t;
	codecvt_t const& codecvt = std::use_facet<codecvt_t>(loc);
	std::mbstate_t state = std::mbstate_t();
	std::vector<char> buf((str.size() + 1) * codecvt.max_length());
	//std::vector<char> buf;
	//buf.reserve((str.size() + 1) * codecvt.max_length());
	wchar_t const* in_next = str.c_str();
	char* out_next = &buf[0];
	std::codecvt_base::result r = codecvt.out(state, 
		str.c_str(), str.c_str() + str.size(), in_next, 
		&buf[0], &buf[0] + buf.size(), out_next);
	if (r == std::codecvt_base::error)
		throw std::runtime_error("can't convert wstring to string");
	return std::string(&buf[0]);
}

wstring StrUtil::mbs_to_wcs(string const& str, locale const& loc)
{
	typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_t;
	codecvt_t const& codecvt = std::use_facet<codecvt_t>(loc);
	std::mbstate_t state = std::mbstate_t();
	std::vector<wchar_t> buf(str.size() + 1);
	//std::vector<wchar_t> buf;
	//buf.reserve(str.size() + 1);
	char const* in_next = str.c_str();
	wchar_t* out_next = &buf[0];
	std::codecvt_base::result r = codecvt.in(state, 
		str.c_str(), str.c_str() + str.size(), in_next, 
		&buf[0], &buf[0] + buf.size(), out_next);
	if (r == std::codecvt_base::error)
		throw std::runtime_error("can't convert string to wstring");
	return std::wstring(&buf[0]);
}

//string StrUtil::utf8_to_ansi(const string& utf8)
//{
//	return wcs_to_mbs(mbs_to_wcs(utf8, loc_utf8), loc);
//}
//
//string StrUtil::ansi_to_utf8(const string& ansi)
//{
//	return wcs_to_mbs(mbs_to_wcs(ansi, loc), loc_utf8);
//}
//
//wstring StrUtil::utf8_to_unicode(const string &utf8)
//{
//	return mbs_to_wcs(utf8, loc_utf8);
//}
//
//string StrUtil::unicode_to_utf8(const wstring &unicode)
//{
//	return wcs_to_mbs(unicode, loc_utf8);
//}

int StrUtil::split(const string& str, vector<string>& vec, const string& delimiter)
{
	string::size_type lastPos = str.find_first_not_of(delimiter, 0);
	string::size_type pos = str.find_first_of(delimiter, lastPos);

	while (string::npos != pos || string::npos != lastPos)
	{
		vec.push_back(str.substr(lastPos, pos - lastPos));
		lastPos = str.find_first_not_of(delimiter, pos);
		pos = str.find_first_of(delimiter, lastPos);
	}
	return (int)vec.size();
}

int StrUtil::split(const wstring& str, vector<wstring>& vec, const wstring& delimiter)
{
	wstring::size_type lastPos = str.find_first_not_of(delimiter, 0);
	wstring::size_type pos = str.find_first_of(delimiter, lastPos);

	while (wstring::npos != pos || wstring::npos != lastPos)
	{
		vec.push_back(str.substr(lastPos, pos - lastPos));
		lastPos = str.find_first_not_of(delimiter, pos);
		pos = str.find_first_of(delimiter, lastPos);
	}
	return (int)vec.size();
}

int StrUtil::token(string &source, string &target, const string& delimeter)
{
	if (source.length() == 0) return INVALID_SOURCE;
	string::size_type idx;
	idx = source.find(delimeter);
	if (idx == string::npos)
	{
		target.assign(source);
		return CANNOT_FIND_DELIMITER;
	}
	else
	{
		target = source.substr(0, idx-1);
		StrUtil::trim_right(target);
		StrUtil::trim_left(target);
		source = source.substr(idx+1, source.length()-idx);
	}
	return (int)idx;
}

int StrUtil::token(wstring &source, wstring &target, const wstring& delimeter)
{
	if (source.length() == 0) return INVALID_SOURCE;
	wstring::size_type idx;
	idx = source.find(delimeter);
	if (idx == wstring::npos)
	{
		target.assign(source);
		return CANNOT_FIND_DELIMITER;
	}
	else
	{
		target = source.substr(0, idx-1);
		StrUtil::trim_right(target);
		StrUtil::trim_left(target);
		source = source.substr(idx+1, source.length()-idx);
	}
	return (int)idx;
}

string StrUtil::token(string& source, const string& delimiter)
{
	string ret;
	int idx = source.find_first_of(delimiter);
	if (idx == (int)string::npos)
	{
		ret.assign(source.c_str(), source.length());
		source.clear();
	}
	else if (idx == 0)
	{
		source.erase(source.begin(), source.begin() + idx + 1);
		ret.clear();
	}
	else if (idx > 0)
	{
		ret.assign(source.c_str(), idx);
		source.erase(source.begin(), source.begin() + idx + 1);
	}
	else
	{
		ret.assign(source.c_str(), source.length());
	}
	return ret;
}

wstring StrUtil::token(wstring& source, const wstring& delimiter)
{
	wstring ret;
	int idx = source.find_first_of(delimiter);
	if (idx == (int)string::npos)
	{
		ret.assign(source.c_str(), source.length());
		source.clear();
	}
	else if (idx == 0)
	{
		source.erase(source.begin(), source.begin() + idx + 1);
		ret.clear();
	}
	else if (idx > 0)
	{
		ret.assign(source.c_str(), idx);
		source.erase(source.begin(), source.begin() + idx + 1);
	}
	else
	{
		ret.assign(source.c_str(), source.length());
	}
	return ret;
}

string StrUtil::replaceall(const string& source, const string& pattern, const string& replace)
{
	string result = source;
	string::size_type pos = 0;
	string::size_type offset = 0;
	string::size_type pattern_len = pattern.size();
	string::size_type replace_len = replace.size();

	while ((pos = result.find(pattern, offset)) != string::npos)
	{
		result.replace(result.begin() + pos, result.begin() + pos + pattern_len, replace);
		offset = pos + replace_len;
	}
	return result;
}

wstring StrUtil::replaceall(const wstring& source, const wstring& pattern, const wstring& replace)
{
	wstring result = source;
	wstring::size_type pos = 0;
	wstring::size_type offset = 0;
	wstring::size_type pattern_len = pattern.size();
	wstring::size_type replace_len = replace.size();

	while ((pos = result.find(pattern, offset)) != wstring::npos)
	{
		result.replace(result.begin() + pos, result.begin() + pos + pattern_len, replace);
		offset = pos + replace_len;
	}
	return result;
}

int StrUtil::divide(string &strsur, string &strdest, char* cdelimiter)
{
	string strswap;
	strswap = strsur;
	int nlen = (int)strsur.find(cdelimiter);	
	if (nlen == -1)
	{
		strdest.assign(strsur);
		return 0;
	}
	strdest.assign(strsur.substr(0,nlen));
	strsur.erase(strsur.begin(),strsur.begin()+ nlen + 3);
	return nlen+3;
}

int StrUtil::divide(string &strsur, string &strdest, char cdelimiter)
{
	string strswap(strsur);
	strswap.assign(strsur);
	int nlen = (int)strsur.find(cdelimiter);
	if (nlen == -1)
	{
		nlen = (int)strsur.find('\0');
		if (nlen == -1)
		{
			strdest.assign(strsur);
			return 0;
		}
	}
	strdest.assign(strsur.substr(0,nlen));
	strsur.erase(strsur.begin(),strsur.begin()+ nlen + 1);
	return nlen+1;
}

string StrUtil::divide(string &strsur, char* cdelimiter)
{
	string strswap;
	string strdest;
	strswap = strsur;
	int nlen = (int)strsur.find(cdelimiter);	
	if (nlen == -1)
	{
		strdest.assign(strsur);
		return 0;
	}
	strdest.assign(strsur.substr(0,nlen));
	strsur.erase(strsur.begin(),strsur.begin()+ nlen + 3);
	return strdest;
}

string StrUtil::divide(string &strsur, char cdelimiter)
{
	string strswap(strsur);
	string strdest;
	strswap.assign(strsur);
	int nlen = (int)strsur.find(cdelimiter);
	if (nlen == -1)
	{
		nlen = (int)strsur.find('\0');
		if (nlen == -1)
		{
			strdest.assign(strsur);
			return 0;
		}
	}
	strdest.assign(strsur.substr(0,nlen));
	strsur.erase(strsur.begin(),strsur.begin()+ nlen + 1);
	return strdest;
}

string StrUtil::get_parse_string(string& value, char* pDeli)
{
	string strRet;
	string strSwap;
	int nDest = value.find_first_of(pDeli);
	if (nDest == (int)string::npos)
	{
		strRet.assign(value.c_str(), value.length());
		value.clear();
	}
	else if (nDest == 0)
	{
		value.erase(value.begin(), value.begin() + nDest+1);
		strRet.clear();
	}
	else if (nDest > 0)
	{
		strRet.assign(value.c_str(), nDest);
		value.erase(value.begin(), value.begin() + nDest+1);
	}
	else
	{
		strRet.assign(value.c_str(), value.length());
	}
	return strRet;
}

wstring StrUtil::get_parse_string(wstring& value, wchar_t* pDeli)
{
	wstring strRet;
	wstring strSwap;
	int nDest = value.find_first_of(pDeli);
	if (nDest == (int)wstring::npos)
	{
		strRet.assign(value.c_str(), value.length());
		value.clear();
	}
	else if (nDest == 0)
	{
		value.erase(value.begin(), value.begin() + nDest+1);
		strRet.clear();
	}
	else if (nDest > 0)
	{
		strRet.assign(value.c_str(), nDest);
		value.erase(value.begin(), value.begin() + nDest+1);
	}
	else
	{
		strRet.assign(value.c_str(), value.length());
	}
	return strRet;
}

}