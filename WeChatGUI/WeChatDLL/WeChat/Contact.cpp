#include "Contact.h"
#include "../public/Strings.h"
#include "../WeChatDLL.h"
#include <AnyCall/AnyCall.h>

MyContact copyContact(Contact* pContact)
{
	MyContact ret;
	ret.userName = UnicodeToAnsi(copyMMString(&pContact->userName).c_str());
	ret.alias = UnicodeToAnsi(copyMMString(&pContact->alias).c_str());
	ret.encryptUserName = UnicodeToAnsi(copyMMString(&pContact->encryptUserName).c_str());
	ret.remark = UnicodeToAnsi(copyMMString(&pContact->remark).c_str());
	ret.nickName = UnicodeToAnsi(copyMMString(&pContact->nickName).c_str());
	return ret;
}

Contact::Contact()
{
	if (WeChatDLL::Instance().getWechatVersion() == WeChat_3_7_6_44) {
		AnyCall::invokeThiscall<void>((void*)this, (void*)(WeChatDLL::Instance().getWinMoudule() + 0x76CBC0));
	}
}

Contact::~Contact()
{
	if (WeChatDLL::Instance().getWechatVersion() == WeChat_3_7_6_44) {
		AnyCall::invokeThiscall<void>((void*)this, (void*)(WeChatDLL::Instance().getWinMoudule() + 0x76D6C0));
	}
}