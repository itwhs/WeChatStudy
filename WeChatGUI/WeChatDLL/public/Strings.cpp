#include "Strings.h"
#include <windows.h>

unsigned char BinMap[256] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0,		//123456789
		0, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0,	//ABCDEF
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0,	//abcdef
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };


std::vector<unsigned char> HexStrToBin(std::string& HexCode)
{
	std::vector<unsigned char> ret;
	int cLen = HexCode.length() / 2;
	if (cLen <= 0) {
		return ret;
	}
	ret.resize(cLen);
	for (int n = 0; n < cLen; n++) {
		ret[n] = BinMap[HexCode[2 * n]] * 16 + BinMap[HexCode[2 * n + 1]];
	}
	return ret;
}

std::string ReplaceString(std::string& str, std::string to_replaced, const std::string& newchars)
{
	for (std::string::size_type pos(0); pos != std::string::npos; pos += newchars.length())
	{
		pos = str.find(to_replaced, pos);
		if (pos != std::string::npos) {
			str.replace(pos, to_replaced.length(), newchars);
		}
		else {
			break;
		}
	}
	return str;
}

std::string UnicodeToUtf8(const wchar_t* szStr)
{
	std::string ret;
	int nLen = WideCharToMultiByte(CP_UTF8, 0, szStr, -1, NULL, 0, NULL, NULL);
	if (nLen == 0)
	{
		return ret;
	}
	char* pResult = new char[nLen];
	if (pResult == NULL) {
		return ret;
	}
	WideCharToMultiByte(CP_UTF8, 0, szStr, -1, pResult, nLen, NULL, NULL);
	ret = pResult;
	delete[]pResult;
	return ret;
}

std::string UnicodeToAnsi(const wchar_t* szStr)
{
	std::string ret;
	int nLen = WideCharToMultiByte(CP_ACP, 0, szStr, -1, NULL, 0, NULL, NULL);
	if (nLen == 0)
	{
		return ret;
	}
	char* pResult = new char[nLen];
	if(pResult == NULL){
		return ret;
	}
	WideCharToMultiByte(CP_ACP, 0, szStr, -1, pResult, nLen, NULL, NULL);
	ret = pResult;
	delete []pResult;
	return ret;
}

std::wstring AnsiToUnicode(const char* szStr)
{
	std::wstring ret;
	int nLen = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szStr, -1, NULL, 0);
	if (nLen == 0)
	{
		return ret;
	}
	wchar_t* pResult = new wchar_t[nLen];
	if (!pResult) {
		return ret;
	}
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szStr, -1, pResult, nLen);
	ret = pResult;
	delete[]pResult;
	return ret;
}

std::wstring Utf8ToUnicode(const char* szStr)
{
	std::wstring ret;
	int nLen = MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, szStr, -1, NULL, 0);
	if (nLen == 0)
	{
		return ret;
	}
	wchar_t* pResult = new wchar_t[nLen];
	if (!pResult) {
		return ret;
	}
	MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, szStr, -1, pResult, nLen);
	ret = pResult;
	delete[]pResult;
	return ret;
}

std::wstring LocalCpToUtf16(const char* str)
{
	std::wstring convertedString;
	if (!str || !*str)
		return convertedString;
	int requiredSize = MultiByteToWideChar(CP_ACP, 0, str, -1, nullptr, 0);
	if (requiredSize > 0)
	{
		convertedString.resize(requiredSize - 1);
		if (!MultiByteToWideChar(CP_ACP, 0, str, -1, (wchar_t*)convertedString.c_str(), requiredSize))
			convertedString.clear();
	}
	return convertedString;
}

std::string Utf16ToUtf8(const wchar_t* wstr)
{
	std::string convertedString;
	if (!wstr || !*wstr)
		return convertedString;
	auto requiredSize = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
	if (requiredSize > 0)
	{
		convertedString.resize(requiredSize - 1);
		if (!WideCharToMultiByte(CP_UTF8, 0, wstr, -1, (char*)convertedString.c_str(), requiredSize, nullptr, nullptr))
			convertedString.clear();
	}
	return convertedString;
}

std::string LocalCpToUtf8(const char* str)
{
	return Utf16ToUtf8(LocalCpToUtf16(str).c_str());
}


