#include "Contact.h"
#include "../public/Strings.h"
#include "../WeChatDLL.h"
#include <AnyCall/AnyCall.h>

MyContact copyContact(Contact* pContact)
{
	MyContact ret;
	ret.userName = copyMMString(&pContact->userName).c_str();
	ret.alias = copyMMString(&pContact->alias).c_str();
	ret.encryptUserName = copyMMString(&pContact->encryptUserName).c_str();
	ret.remark = copyMMString(&pContact->remark).c_str();
	ret.nickName = copyMMString(&pContact->nickName).c_str();
	return ret;
}


Contact::Contact()
{
	switch (WeChatDLL::Instance().getWechatVersion()) {
	case WeChat_3_7_6_44:
		AnyCall::invokeThiscall<void>((void*)this, (void*)(WeChatDLL::Instance().getWinMoudule() + 0x76CBC0));
		return;
	case WeChat_3_8_0_33:
		AnyCall::invokeThiscall<void>((void*)this, (void*)(WeChatDLL::Instance().getWinMoudule() + 0xD10690));
		return;
	}
}

void Contact::free()
{
	switch (WeChatDLL::Instance().getWechatVersion()) {
	case WeChat_3_7_6_44:
		AnyCall::invokeThiscall<void>((void*)this, (void*)(WeChatDLL::Instance().getWinMoudule() + 0x76D6C0));
		return;
	case WeChat_3_8_0_33:
		AnyCall::invokeThiscall<void>((void*)this, (void*)(WeChatDLL::Instance().getWinMoudule() + 0xD11190));
		return;
	}
}

ContactX::ContactX()
{

}

ContactX::~ContactX()
{
	free();
}
