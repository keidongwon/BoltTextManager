#pragma once

class ConfigManager
{
public:
	ConfigManager();
	~ConfigManager() {};

public:
	void Load();
	void Save();
	void CreateIniFile(BOOL bReset = FALSE);

private:
	TCHAR ininame[MAX_PATH];
	TCHAR currentdir[MAX_PATH];
	TCHAR programfiles[MAX_PATH];
	TCHAR localappdata[MAX_PATH];
	CString Bool2Cstr(BOOL flag);

// for Common
public:
	CString get_system_name()					{ return L"BoltTextManager 2.0"; }
	CString get_allow_extensions()				{ return allowExtensions; }
	CString get_option_name()					{ return optionName; }
	CString get_editor_path()					{ return editorPath; }
	CString get_default_encoding()				{ return defaultEncoding; }
	int get_main_image()						{ return mainImage; }

	BOOL is_include_extension(CString value);
	BOOL is_include_subfolder()					{ return includeSubFolder; }
	BOOL is_overwrite_readonly()				{ return overwriteReadOnly; } 
	BOOL is_overwrite_file()					{ return overwriteFile; }
	BOOL is_option_name_prefix()				{ return prefixName; }
	BOOL is_custom_editor()						{ return customEditor; }

	void set_main_image(int no)					{ mainImage = no; }
	void set_allow_extensions(CString ext)		{ allowExtensions = ext; }
	void set_option_name(CString name)			{ optionName = name; }
	void set_editor_path(CString path)			{ editorPath = path; }
	void set_include_subfolder(BOOL flag)		{ includeSubFolder = flag; }
	void set_overwrite_readonly(BOOL flag)		{ overwriteReadOnly = flag; }
	void set_overwrite_file(BOOL flag)			{ overwriteFile = flag; }
	void set_option_name_prefix(BOOL flag)		{ prefixName = flag; }
	void set_custom_editor(BOOL flag)			{ customEditor = flag; }
	void set_default_encoding(CString value)	{ defaultEncoding = value; }

private:
	CString allowExtensions;
	CString optionName;
	CString editorPath;
	CString defaultEncoding;
	BOOL includeSubFolder;
	BOOL overwriteReadOnly;
	BOOL overwriteFile;
	BOOL prefixName;
	BOOL customEditor;
	int mainImage;

// for Transparent
public:
	int GetTransparentDlgConfig()				{ return nDlgConfig; }
	void SetTransparentDlgConfig(int value)		{ nDlgConfig = value; }

private:
	int nDlgConfig;
};

extern ConfigManager config;
