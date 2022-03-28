#pragma once
#include <string>

struct mmString
{
	wchar_t* pUnicode;
	int Mysize;
	int Myres;
	char* pUTF8;
	int uLen;

	void assign(const wchar_t* src, int len);
};



std::wstring copyMMString(mmString* mmStr);