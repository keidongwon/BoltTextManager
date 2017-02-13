#include "stdafx.h"
#include "ConfigManager.h"
#include "../bolt/ini.h"
#include "../bolt/file.h"
#include "../bolt/strutil.h"

bolt::Ini ini;
static const int BUFFERSIZE = 8192;

ConfigManager::ConfigManager()
{
	TCHAR fullpath[MAX_PATH] = { 0 };
	TCHAR drive[MAX_PATH] = { 0, };
	TCHAR dir[MAX_PATH] = { 0, };
	TCHAR filename[MAX_PATH] = { 0, };
	GetModuleFileName(NULL, fullpath, MAX_PATH);
	_tsplitpath(fullpath, drive, dir, filename, NULL);

	ZeroMemory(ininame, MAX_PATH);
	ZeroMemory(currentdir, MAX_PATH);
	ZeroMemory(localappdata, MAX_PATH);
	ZeroMemory(programfiles, MAX_PATH);
	_stprintf(currentdir, _T("%s%s"), drive, dir);
	_stprintf(ininame, _T("%s%s.ini"), currentdir, filename);
	GetEnvironmentVariable(_T("LOCALAPPDATA"), localappdata, MAX_PATH);
	GetEnvironmentVariable(_T("ProgramFiles"), programfiles, MAX_PATH);

	// common
	allowExtensions				= _T("");
	defaultEncoding				= _T("");
	includeSubFolder			= TRUE;
}

void ConfigManager::CreateIniFile(BOOL bReset)
{
	if ((bReset == FALSE) && (bolt::File::is_exist_file(ininame) == TRUE))
		DeleteFile(ininame);

	ini.set_filename(ininame);
	TCHAR tszTemp[BUFFERSIZE] = {0,};

	// [Common] ---------------------------------------------------------------

	ini.set_value(_T("common"), _T("main_image"), (DWORD)0);

	ZeroMemory(tszTemp, BUFFERSIZE);
	wcscpy_s(tszTemp, BUFFERSIZE, _T("txt,doc,ini,c,h,cc,cpp,hpp,java,js,asp,aspx,cs,py,rb,pas,php,pl,xml,htm,html,css,sql,lua,vb,vbs"));
	ini.set_value(_T("common"), _T("extension"), tszTemp);
	set_allow_extensions(tszTemp);

	ini.set_value(_T("common"), _T("subfolder"), _T("YES"));
	set_include_subfolder(TRUE);

	ini.set_value(_T("common"), _T("readonly"), _T("YES"));
	set_overwrite_readonly(TRUE);

	ini.set_value(_T("common"), _T("overwrite"), _T("YES"));
	set_overwrite_file(TRUE);

	ini.set_value(_T("common"), _T("name_prefix"), _T("NO"));
	set_option_name_prefix(FALSE);

	ini.set_value(_T("common"), _T("option_name"), _T("_UTF8"));
	set_option_name(_T("_UTF8"));

	ini.set_value(_T("common"), _T("custom_editor"), _T("NO"));
	set_custom_editor(FALSE);

	ini.set_value(_T("common"), _T("editor_path"), _T(""));
	set_editor_path(_T(""));

	ini.set_value(_T("common"), _T("default_encoding"), _T("CP949"));
	set_default_encoding(_T("CP949"));

	// [Transparent] ----------------------------------------------------------
	nDlgConfig = 95;

	ini.set_value(_T("transparent"), _T("DlgConfig"), nDlgConfig);
}

void ConfigManager::Load()
{
	if (bolt::File::is_exist_file(ininame) == FALSE)
	{
		CreateIniFile();
		return;
	}
 
	ini.set_filename(ininame);

	TCHAR szValue[BUFFERSIZE] = {0,};

	// [Common]

	set_main_image(ini.get_int_value(_T("common"), _T("main_image")));

	ZeroMemory(szValue, BUFFERSIZE);
	ini.get_value(_T("common"), _T("extension"), szValue);
	set_allow_extensions(szValue);

	ZeroMemory(szValue, BUFFERSIZE);
	ini.get_value(_T("common"), _T("subfolder"), szValue);
	if ((bolt::StrUtil::compare_nocase(szValue, _T("YES")) == TRUE) || (bolt::StrUtil::compare_nocase(szValue, _T("ON")) == TRUE))
		set_include_subfolder(TRUE);
	else
		set_include_subfolder(FALSE);

	ZeroMemory(szValue, BUFFERSIZE);
	ini.get_value(_T("common"), _T("readonly"), szValue);
	if ((bolt::StrUtil::compare_nocase(szValue, _T("YES")) == TRUE) || (bolt::StrUtil::compare_nocase(szValue, _T("ON")) == TRUE))
		set_overwrite_readonly(TRUE);
	else
		set_overwrite_readonly(FALSE);

	ZeroMemory(szValue, BUFFERSIZE);
	ini.get_value(_T("common"), _T("overwrite"), szValue);
	if ((bolt::StrUtil::compare_nocase(szValue, _T("YES")) == TRUE) || (bolt::StrUtil::compare_nocase(szValue, _T("ON")) == TRUE))
		set_overwrite_file(TRUE);
	else
		set_overwrite_file(FALSE);

	ZeroMemory(szValue, BUFFERSIZE);
	ini.get_value(_T("common"), _T("name_prefix"), szValue);
	if ((bolt::StrUtil::compare_nocase(szValue, _T("YES")) == TRUE) || (bolt::StrUtil::compare_nocase(szValue, _T("ON")) == TRUE))
		set_option_name_prefix(TRUE);
	else
		set_option_name_prefix(FALSE);

	ZeroMemory(szValue, BUFFERSIZE);
	ini.get_value(_T("common"), _T("option_name"), szValue);
	set_option_name(szValue);

	ZeroMemory(szValue, BUFFERSIZE);
	ini.get_value(_T("common"), _T("custom_editor"), szValue);
	if ((bolt::StrUtil::compare_nocase(szValue, _T("YES")) == TRUE) || (bolt::StrUtil::compare_nocase(szValue, _T("ON")) == TRUE))
		set_custom_editor(TRUE);
	else
		set_custom_editor(FALSE);

	ZeroMemory(szValue, BUFFERSIZE);
	ini.get_value(_T("common"), _T("editor_path"), szValue);
	set_editor_path(szValue);

	ZeroMemory(szValue, BUFFERSIZE);
	ini.get_value(_T("common"), _T("default_encoding"), szValue);
	set_default_encoding(szValue);

	// [Transparent]
	nDlgConfig		= ini.get_int_value(_T("transparent"), _T("DlgConfig"));
}

inline CString ConfigManager::Bool2Cstr(BOOL flag)
{
	if (flag == TRUE) 
		return _T("YES");
	else
		return _T("NO");
}

void ConfigManager::Save()
{
	ini.set_filename(ininame);

	CString value = _T("");

	// [Common] ---------------------------------------------------------------
	ini.set_value(_T("common"), _T("extension"), allowExtensions);
	ini.set_value(_T("common"), _T("subfolder"), Bool2Cstr(includeSubFolder));
	ini.set_value(_T("common"), _T("readonly"), Bool2Cstr(overwriteReadOnly));
	ini.set_value(_T("common"), _T("overwrite"), Bool2Cstr(overwriteFile));
	ini.set_value(_T("common"), _T("name_prefix"), Bool2Cstr(prefixName));
	ini.set_value(_T("common"), _T("option_name"), optionName);
	ini.set_value(_T("common"), _T("custom_editor"), Bool2Cstr(customEditor));
	ini.set_value(_T("common"), _T("editor_path"), editorPath);
	ini.set_value(_T("common"), _T("default_encoding"), defaultEncoding);
	
	// [Transparent] ----------------------------------------------------------
	ini.set_value(_T("transparent"), _T("DlgConfig"), nDlgConfig);
}

BOOL ConfigManager::is_include_extension(CString value)
{
	CString tmpSoruce = L"," + allowExtensions + L",";
	CString tmpValue = L"," + value + L",";
	int pos = tmpSoruce.Find(tmpValue);
	if (pos == -1) return FALSE;
	return TRUE;
}
