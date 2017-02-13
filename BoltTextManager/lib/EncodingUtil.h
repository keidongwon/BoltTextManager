#pragma once
#include <string>

static const int CR = 0x0D;
static const int LF = 0x0A;

enum EolType
{
	windows,
	macos,
	unix,

	// special values
	unknown, // can not be the first value for legacy code
	osdefault = windows,
};

static EolType convertIntToFormatType(int value, EolType defvalue)
{
	switch (value)
	{
	case static_cast<LPARAM>(EolType::windows) :
		return EolType::windows;
	case static_cast<LPARAM>(EolType::macos) :
		return EolType::macos;
	case static_cast<LPARAM>(EolType::unix) :
		return EolType::unix;
	default:
		return defvalue;
	}
}

class EncodingUtil
{
public:
	bool check(const wchar_t *filepath, EolType &eol, std::wstring &encoding, std::wstring &charset);
	bool check(const char *filepath, EolType &eol, std::wstring &encoding, std::wstring &charset);
	bool convert(const wchar_t *file_in, const wchar_t *file_out, const wchar_t *tocode, const wchar_t *fromcode, bool tobom, bool frombom, EolType toeol, EolType fromeol);
	bool convert(const char *file_in, const char *file_out, const char *tocode, const char *fromcode, bool tobom, bool frombom, EolType toeol, EolType fromeol);

private:
	EolType get_eol_format(const char* const buf, size_t length, EolType defvalue = EolType::osdefault);
	int detect_codepage(char* buf, size_t len);
	std::string detect_charset(char* buf, size_t len);
	void insert_bom(FILE *fp, const char *encoding);
	std::string get_format(EolType eol);
	std::string convert_eol(char *buf, EolType toeol, EolType fromeol);
};
