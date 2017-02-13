#include "stdafx.h"
#include <atlconv.h>
#include <stdint.h>
#include "EncodingUtil.h"
#include "../bolt/strutil.h"
#include "iconv.h"
#include "text_encoding_detect.h"
#include "uchardet/uchardet.h"
#include "EncodingMapper.h"

using namespace AutoIt::Common;

EolType EncodingUtil::get_eol_format(const char* const buf, size_t length, EolType defvalue)
{
	for (size_t i = 0; i != length; ++i)
	{
		if (buf[i] == CR)
		{
			if (i + 1 < length && buf[i + 1] == LF)
				return EolType::windows;

			return EolType::macos;
		}
		if (buf[i] == LF)
			return EolType::unix;
	}
	return defvalue; // fallback unknown
}

int EncodingUtil::detect_codepage(char* buf, size_t len)
{
	uchardet_t ud = uchardet_new();
	uchardet_handle_data(ud, buf, len);
	uchardet_data_end(ud);
	const char* cs = uchardet_get_charset(ud);
	int codepage = EncodingMapper::getInstance()->getEncodingFromString(cs);
	uchardet_delete(ud);
	return codepage;
}

string EncodingUtil::detect_charset(char* buf, size_t len)
{
	uchardet_t ud = uchardet_new();
	uchardet_handle_data(ud, buf, len);
	uchardet_data_end(ud);
	const char* cs = uchardet_get_charset(ud);
	string charset = cs;
	uchardet_delete(ud);
	return charset;
}

void EncodingUtil::insert_bom(FILE *fp, const char *encoding)
{
	fseek(fp, 0, 0);
	if (strcmp(encoding, "UTF-8") == 0)
	{
		const char utf8_bom[] = { '\xef', '\xbb', '\xbf' };
		fwrite(utf8_bom, 1, sizeof(utf8_bom), fp);
	}
	else if (strcmp(encoding, "UTF-16LE") == 0)
	{
		const char utf16le_bom[] = { '\xff', '\xfe' };
		fwrite(utf16le_bom, 1, sizeof(utf16le_bom), fp);
	}
	else if (strcmp(encoding, "UTF-16BE") == 0)
	{
		const char utf16be_bom[] = { '\xfe', '\xff' };
		fwrite(utf16be_bom, 1, sizeof(utf16be_bom), fp);
	}
}

string EncodingUtil::get_format(EolType eol)
{
	switch (eol)
	{
	case EolType::windows:
		return string("\r\n");
		break;
	case EolType::unix:
		return string("\n");
		break;
	case EolType::macos:
		return string("\r");
		break;
	default:
		return string("\r\n");
	}
}

string EncodingUtil::convert_eol(char *buf, EolType toeol, EolType fromeol)
{
	if (toeol == EolType::unknown || toeol == fromeol) return "";
	string tmpstr = buf;
	string source = get_format(fromeol);
	string target = get_format(toeol);
	tmpstr = bolt::StrUtil::replaceall(tmpstr, source, target);
	return tmpstr;
}

bool EncodingUtil::check(const wchar_t *filepath, EolType &eol, wstring &encoding, wstring &charset)
{
	int _encoding = TextEncodingDetect::None;

	FILE *fp = _wfopen(filepath, L"rb");
	_fseeki64(fp, 0, SEEK_END);
	long filesize = (long)_ftelli64(fp);
	_fseeki64(fp, 0, 0);
	if (filesize == 0) return false;
	char *str = new char[filesize + 1];
	memset(str, 0, filesize);
	str[filesize] = '\0';
	size_t readsize = fread(str, 0x1, filesize, fp);

	TextEncodingDetect ted;
	_encoding = ted.DetectEncoding((const unsigned char*)str, readsize);
	encoding = std::to_wstring(_encoding);

	string _charset = detect_charset(str, readsize);
	charset.assign(_charset.begin(), _charset.end());
	//int codepage = detect_codepage(str, readsize);

	if (_encoding == TextEncodingDetect::UTF16_LE_BOM || _encoding == TextEncodingDetect::UTF16_BE_BOM) {
		char fromcode[32] = { 0x00, };
		if (_encoding == TextEncodingDetect::UTF16_LE_BOM) 
			strcpy(fromcode, "UTF-16LE");
		else if (_encoding == TextEncodingDetect::UTF16_BE_BOM) 
			strcpy(fromcode, "UTF-16BE");

		iconv_t it = iconv_open("UTF-8", fromcode);
		if (it != (iconv_t)-1) {
			const char *str_ptr = (const char*)str;
			size_t out_buf_size = readsize * 3 + 3;
			char *out_buf = new char[out_buf_size];
			memset(out_buf, '\0', out_buf_size);
			char *output_buf_ptr = out_buf;
			size_t outbytesleft = out_buf_size;
			int ret = iconv(it, &str_ptr, &readsize, &output_buf_ptr, &outbytesleft);
			size_t out_size = out_buf_size - outbytesleft;
			eol = get_eol_format(out_buf, out_size, EolType::unknown);
			delete[] out_buf;
		}
	}
	else 
		eol = get_eol_format(str, readsize, EolType::unknown);
	
	delete[] str;
	fclose(fp);

	return true;
}

bool EncodingUtil::check(const char *filepath, EolType &eol, std::wstring &encoding, wstring &charset)
{
	USES_CONVERSION;
	return check(A2W(filepath), eol, encoding, charset);
}

bool EncodingUtil::convert(const char *file_in, const char *file_out, const char *tocode, const char *fromcode, bool tobom, bool frombom, EolType toeol, EolType fromeol)
{
	bool iconv_flag = false;
	FILE *fi = fopen(file_in, "rb");
	fseek(fi, 0, SEEK_END);
	long fi_size = (long)_ftelli64(fi);
	if (fi_size == 0) return false;
	fseek(fi, 0, 0);
	size_t buf_size = fi_size * 3 + 4;

	iconv_t it = NULL;
	if (strcmp(tocode, fromcode) != 0)
	{
		it = iconv_open(tocode, fromcode);
		if (it == (iconv_t)-1) return false;
	}

	if ((strcmp(tocode, fromcode) != 0) && (frombom == true) ||
		(strcmp(tocode, fromcode) == 0) && (frombom == true && tobom == false))
	{
		if (strcmp(fromcode, "UTF-8") == 0)
		{
			fi_size -= 3;
			fseek(fi, 3, 0);
		}
		else if (strcmp(fromcode, "UTF-16LE") == 0 || strcmp(fromcode, "UTF-16BE") == 0)
		{
			fi_size -= 2;
			fseek(fi, 2, 0);
		}
	}

	char *in_buf = new char[buf_size];
	memset(in_buf, 0x00, buf_size);
	size_t in_buf_size = fread(in_buf, 0x1, fi_size, fi);
	in_buf[fi_size] = '\0';
	fclose(fi);
	
	if (strcmp(fromcode, "UTF-16LE") != 0 && strcmp(fromcode, "UTF-16BE") != 0)
	{
		string tmpstr = convert_eol(in_buf, toeol, fromeol);
		if (!tmpstr.empty())
		{
			memset(in_buf, 0x00, buf_size);
			sprintf(in_buf, "%s", tmpstr.c_str());
			in_buf_size = tmpstr.length();
			in_buf[in_buf_size] = '\0';
		}
	}

	char *out_buf = new char[buf_size];
	memset(out_buf, '\0', buf_size);
	size_t out_size = in_buf_size;

	if (strcmp(tocode, fromcode) != 0)
	{
		char *out_buf_ptr = out_buf;
		const char *in_buf_ptr = (const char*)in_buf;
		size_t outbytesleft = buf_size;
		int ret = iconv(it, &in_buf_ptr, &in_buf_size, &out_buf_ptr, &outbytesleft);
		out_size = buf_size - outbytesleft;
		iconv_flag = true;
	}
	else
	{
		memcpy(out_buf, in_buf, out_size + 1);
	}

	FILE *fo = NULL;
	if (file_out == NULL)
		fopen_s(&fo, file_in, "wb");
	else
		fopen_s(&fo, file_out, "wb");

	if ((iconv_flag == false && (tobom == true && frombom == false)) || 
		(iconv_flag == true && tobom == true))
	{
		insert_bom(fo, tocode);
	}
	
	fwrite(out_buf, 1, out_size, fo);
	fclose(fo);

	delete[]in_buf;
	delete[]out_buf;

	return true;
}

bool EncodingUtil::convert(const wchar_t *file_in, const wchar_t *file_out, const wchar_t *tocode, const wchar_t *fromcode, bool tobom, bool frombom, EolType toeol, EolType fromeol)
{
	USES_CONVERSION;
	return convert(W2A(file_in), W2A(file_out), W2A(tocode), W2A(fromcode), tobom, frombom, toeol, fromeol);
}
