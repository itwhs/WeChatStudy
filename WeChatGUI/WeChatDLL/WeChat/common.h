#pragma once
#include <string>


//原生结构体

struct mmString
{
	wchar_t* pUnicode;
	int Mysize;
	int Myres;
	char* pUTF8;
	int uLen;
	void assign(const wchar_t* src, int len);
	void assign(const char* src);
	void assignUTF8(const char* src);
	void free();
	bool empty();
};

std::wstring copyMMString(mmString* mmStr);

//封装结构体
struct MymmString
{
	MymmString();
	~MymmString();
public:
	mmString coreStr;
};

