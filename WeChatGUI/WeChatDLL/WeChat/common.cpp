#include "common.h"
#include "../public/Strings.h"
#include "../WeChatDLL.h"
#include <AnyCall/AnyCall.h>

std::wstring copyMMString(mmString* mmStr)
{
	std::wstring ret;
	if (mmStr->pUnicode && *mmStr->pUnicode) {
		ret.assign(mmStr->pUnicode,mmStr->Mysize);
	}
	return ret;
}

mmString::mmString()
{
	pUnicode = 0x0;
	Mysize = 0x0;
	Myres = 0x0;
	pUTF8 = 0x0;
	uLen = 0x0;
}

void mmString::assign(const wchar_t* src, int len)
{
	if (WeChatDLL::Instance().getWechatVersion() == WeChat_3_7_6_44) {
		AnyCall::invokeThiscall<void>((void*)this, (void*)(WeChatDLL::Instance().getWinMoudule() + 0x8167B0), src, len);
	}
}

void mmString::assignUTF8(const char* src)
{
	std::wstring wStr = Utf8ToUnicode(src);
	this->assign(wStr.c_str(), wStr.size());
}

void mmString::assign(const char* src)
{
	std::wstring wStr = AnsiToUnicode(src);
	this->assign(wStr.c_str(),wStr.size());
}

void mmString::free()
{
	if (WeChatDLL::Instance().getWechatVersion() == WeChat_3_7_6_44) {
		AnyCall::invokeThiscall<void>((void*)this, (void*)(WeChatDLL::Instance().getWinMoudule() + 0x131D00));
	}
}

mmStringX::mmStringX(const mmStringX& s)
{
	if (!s.Mysize) {
		return;
	}
	wchar_t* pBuf = new wchar_t[s.Mysize + 1];
	if (pBuf) {
		wmemcpy(pBuf, s.pUnicode, s.Mysize);
		pBuf[s.Mysize] = 0x0;
		this->pUnicode = pBuf;
		this->Mysize = s.Mysize;
		this->Myres = s.Mysize;
	}
}

mmStringX::mmStringX()
{

}

mmStringX::~mmStringX()
{
	free();
}

