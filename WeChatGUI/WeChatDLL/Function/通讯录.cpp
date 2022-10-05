#include "ͨѶ¼.h"
#include <unordered_map>
#include "../InlineHook/InlineHook.h"
#include "../WeChatDLL.h"
#include "../public/Strings.h"
#include "../public/RWMutex.h"
#include "../public/Public.h"
#include <AnyCall/AnyCall.h>

ContactModule& ContactModule::Instance()
{
	static ContactModule gContactModule;
	return gContactModule;
}

void ContactModule::InitContactModule(WechatVersion ver)
{
	WeChatVer = ver;
}

MyContact ContactModule::getContactInfoDynamic(std::string userName)
{
	MyContact ret;
	if (this->WeChatVer == WeChat_3_7_6_44) {
		Contact outContatInfo;
		std::wstring wUserName = AnsiToUnicode(userName.c_str());
		mmString mUserName;
		mUserName.assign(wUserName.c_str(), wUserName.length());
		void* gContactMgr = ContactMgr_Instance();
		if (!AnyCall::invokeThiscall<bool>(gContactMgr, (void*)(WeChatDLL::Instance().getWinMoudule() + 0x4FD560), mUserName, &outContatInfo)) {
			ret.userName = userName;
			ret.nickName = userName;
			return ret;
		}
		ret = copyContact(&outContatInfo);
		if (ret.nickName == "") {
			ret.nickName = userName;
		}
	}
	return ret;
}

void* ContactModule::ContactMgr_Instance()
{
	if (this->WeChatVer == WeChat_3_7_6_44) {
		return AnyCall::invokeStdcall<void*>((void*)(WeChatDLL::Instance().getWinMoudule() + 0x134140));
	}
}
