#include "common.h"

std::wstring copyMMString(mmString* mmStr)
{
	std::wstring ret;
	if (mmStr->pUnicode && *mmStr->pUnicode) {
		ret.assign(mmStr->pUnicode,mmStr->Mysize);
	}
	return ret;
}


//mmString::mmString()
//{
//	this->pUnicode = 0;
//	this->Mysize = 0;
//	this->Myres = 0;
//	this->pUTF8 = 0;
//	this->uLen = 0;
//}
//
//mmString::mmString(const mmString& src)
//{
//	this->pUnicode = 0;
//	this->Mysize = 0;
//	this->Myres = 0;
//	this->pUTF8 = 0;
//	this->uLen = 0;
//
//	if (src.pUnicode && src.pUnicode[0]) {
//		int cLen = wcslen(src.pUnicode);
//		wchar_t* pBuf = new wchar_t[cLen + 1];
//		if (pBuf) {
//			wmemcpy(pBuf, src.pUnicode, cLen);
//			pBuf[cLen] = 0x0;
//			this->pUnicode = pBuf;
//			this->Mysize = cLen;
//			this->Myres = cLen;
//		}
//	}
//}
//
//mmString::~mmString()
//{
//	if (this->pUnicode) {
//		delete this->pUnicode;
//		this->pUnicode = 0;
//	}
//	if (this->pUTF8) {
//		delete this->pUTF8;
//		this->pUTF8 = 0;
//	}
//	this->Mysize = 0;
//	this->Myres = 0;
//	this->uLen = 0;
//}

void mmString::assign(const wchar_t* src, int len)
{
	this->pUnicode = 0;
	this->Mysize = 0;
	this->Myres = 0;
	this->pUTF8 = 0;
	this->uLen = 0;

	if (!src || !len) {
		return;
	}
	wchar_t* pBuf = new wchar_t[len + 1];
	if (pBuf) {
		wmemcpy(pBuf, src, len);
		pBuf[len] = 0x0;
		this->pUnicode = pBuf;
		this->Mysize = len;
		this->Myres = len;
	}
}

