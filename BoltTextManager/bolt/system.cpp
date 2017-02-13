#include "stdafx.h"
#include "system.h"

string System::get_datetime(const string& fmt)
{
	DATETIMEA(fmt.c_str())
}

wstring System::get_datetimew(const wstring& fmt)
{
	DATETIMEW(fmt.c_str())
}

