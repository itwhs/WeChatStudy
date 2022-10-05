#pragma once
#include <string>
#include <vector>

//原生结构体,用于读
struct mmString
{
public:
	mmString();
	void free();
	void assign(const wchar_t* src, int len);
	void assign(const char* src);
	void assignUTF8(const char* src);
public:
	wchar_t* pUnicode;
	int Mysize;
	int Myres;
	char* pUTF8;
	int uLen;
};

std::wstring copyMMString(mmString* mmStr);

//封装结构体,相比原生结构体增加了析构函数和拷贝构造函数呢
struct MymmString:public mmString
{
public:
	MymmString();
	//拷贝构造函数
	MymmString(const MymmString& obj);
	~MymmString();
};
