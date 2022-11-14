#include "AccountFunction.h"
#include "../WeChatDLL.h"
#include "../InlineHook/InlineHook.h"

InlineHook gHook_SetCurrentUserWxid;
InlineHook gHook_AccountSvrLogin;
InlineHook gHook_AccountSvrLogout;
std::string AccountFunction::currentUserWxid;
HANDLE gLoginEvent;

AccountFunction& AccountFunction::Instance()
{
	static AccountFunction gAccountFunction;
	return gAccountFunction;
}

void __stdcall MySetCurrentUserWxid(HookContext* hookContext)
{
	//gHook_SetCurrentUserWxid.AddHook((LPVOID)(hWeChatWinDLL + 0x7232CD), MySetCurrentUserWxid);
	char* pWxid = (char*)(hookContext->EAX);
	if (pWxid) {
		AccountFunction::currentUserWxid = pWxid;
	}
}


//hook的是函数头
void __stdcall AccountService_login(HookContext* hookContext)
{
	SetEvent(gLoginEvent);
	//第一个参数
	char* pWxid = (char*)*(DWORD*)(hookContext->ESP + 0x4);
	AccountFunction::currentUserWxid = pWxid;
}

void __stdcall AccountService_logout(HookContext* hookContext)
{
	ResetEvent(gLoginEvent);
}

std::string AccountFunction::WaitUtilLogin()
{
	WaitForSingleObject(gLoginEvent, INFINITE);
	return AccountFunction::currentUserWxid;
}

bool AccountFunction::InitAccountModule(WeChatVersion v)
{
	std::string loginEvent = "WeChatLogin_" + std::to_string(GetCurrentProcessId());
	gLoginEvent = CreateEventA(NULL, true, false, loginEvent.c_str());

	WeChatVer = v;
	DWORD hWeChatWinDLL = WeChatDLL::Instance().getWinMoudule();

	switch (WeChatVer) {
	case WeChat_3_7_6_44:
		gHook_AccountSvrLogin.AddHook((LPVOID)(hWeChatWinDLL + 0x723270), AccountService_login);
		gHook_AccountSvrLogout.AddHook((LPVOID)(hWeChatWinDLL + 0x7240F0), AccountService_logout);
		return true;
	case WeChat_3_8_0_33:
		gHook_AccountSvrLogin.AddHook((LPVOID)(hWeChatWinDLL + 0xCC7BC0), AccountService_login);
		gHook_AccountSvrLogout.AddHook((LPVOID)(hWeChatWinDLL + 0xCC8A50), AccountService_logout);
		return true;
	default:
		break;
	}
	return false;
}

