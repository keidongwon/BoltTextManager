#pragma once
#include <string>
#include <vector>

class TextFile
{
public:
	std::wstring path;
	std::wstring filename;
	int encoding;
};

typedef std::vector<TextFile> VECTEXTFILES;

