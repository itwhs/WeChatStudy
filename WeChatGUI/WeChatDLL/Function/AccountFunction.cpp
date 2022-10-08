#include "AccountFunction.h"
#include "../WeChatDLL.h"
#include "../InlineHook/InlineHook.h"

InlineHook gHook_SetCurrentUserWxid;
std::string AccountFunction::currentUserWxid;

AccountFunction& AccountFunction::Instance()
{
	static AccountFunction gAccountFunction;
	return gAccountFunction;
}

void __stdcall MySetCurrentUserWxid(HookContext* hookContext)
{
	char* pWxid = (char*)(hookContext->EAX);
	if (pWxid) {
		AccountFunction::currentUserWxid = pWxid;
	}
}
	


bool AccountFunction::InitAccountModule(WeChatVersion v)
{
	WeChatVer = v;

	DWORD hWeChatWinDLL = WeChatDLL::Instance().getWinMoudule();
	if (WeChatVer == WeChat_3_7_6_44) {
		gHook_SetCurrentUserWxid.AddHook((LPVOID)(hWeChatWinDLL + 0x7232CD), MySetCurrentUserWxid);
		return true;
	}

	return false;
}

