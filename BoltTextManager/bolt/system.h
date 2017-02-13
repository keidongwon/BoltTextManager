#pragma once
#include <string>
#include <time.h>

using namespace std;

#define DATETIMEA(TYPE) \
	time_t t; \
	struct tm *tm; \
	time(&t); \
	tm = localtime(&t); \
	char datetime[32] = {0,}; \
	strftime(datetime, 32, TYPE, tm); \
	return datetime;

#define DATETIMEW(TYPE) \
	time_t t; \
	struct tm *tm; \
	time(&t); \
	tm = localtime(&t); \
	wchar_t datetime[32] = {0,}; \
	wcsftime(datetime, 32, TYPE, tm); \
	return datetime;

class System {
public:
	System() {}
	~System() {}

public:
	static string get_datetime(const string& fmt = "%Y-%m-%d %H:%M:%S");
	static wstring get_datetimew(const wstring& fmt = L"%Y-%m-%d %H:%M:%S");
};
