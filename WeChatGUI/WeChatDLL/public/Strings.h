#pragma once
#include <string>
#include <vector>

//十六进制字符串转字节集
std::vector<unsigned char> HexStrToBin(std::string& HexCode);

std::string ReplaceString(std::string& str,std::string to_replaced,const std::string& newchars);

std::string UnicodeToAnsi(const wchar_t* szStr);

std::wstring AnsiToUnicode(const char* szStr);

std::wstring Utf8ToUnicode(const char* szStr);

//转utf8
std::string LocalCpToUtf8(const char* str);