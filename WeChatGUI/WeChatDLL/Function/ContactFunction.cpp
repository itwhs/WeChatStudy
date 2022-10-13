#include "ContactFunction.h"
#include <unordered_map>
#include "../InlineHook/InlineHook.h"
#include "../WeChatDLL.h"
#include "../public/Strings.h"
#include "../public/RWMutex.h"
#include "../public/Public.h"
#include <AnyCall/AnyCall.h>
#include <MyTinySTL/vector.h>

ContactModule& ContactModule::Instance()
{
	static ContactModule gContactModule;
	return gContactModule;
}

void ContactModule::InitContactModule(WeChatVersion ver)
{
	WeChatVer = ver;
}

std::vector<MyContact> ContactModule::GetContactList()
{
	struct tmpVector
	{
		Contact* pVectorStart;
		Contact* pVectorLast;
		Contact* pVectorEnd;
	};
	std::vector<MyContact> retList;
	if (this->WeChatVer == WeChat_3_7_6_44) {
		tmpVector outContactList = { 0 };
		void* gContactMgr = ContactMgr_Instance();
		AnyCall::invokeThiscall<void>(gContactMgr, (void*)(WeChatDLL::Instance().getWinMoudule() + 0x501060), &outContactList);
		unsigned int msgCount = (outContactList.pVectorLast - outContactList.pVectorStart);
		Contact* pStartContact = outContactList.pVectorStart;
		for (unsigned int n = 0; n < msgCount; ++n) {
			MyContact tmp = copyContact(pStartContact);
			retList.push_back(tmp);
			pStartContact->free();
			pStartContact++;
		}
	}
	return retList;
}

MyContact ContactModule::GetContactInfoDynamic(std::string userName)
{
	MyContact ret;
	if (this->WeChatVer == WeChat_3_7_6_44) {
		ContactX outContatInfo;
		std::wstring wUserName = AnsiToUnicode(userName.c_str());
		mmStringX mUserName;
		mUserName.assign(wUserName.c_str(), wUserName.length());
		void* gContactMgr = ContactMgr_Instance();
		if (!AnyCall::invokeThiscall<bool>(gContactMgr, (void*)(WeChatDLL::Instance().getWinMoudule() + 0x4FD560), &mUserName, &outContatInfo)) {
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
	return NULL;
}
