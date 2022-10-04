#include "WeChatDLL.h"
#include "public/WinApi.h"
#include "InlineHook/InlineHook.h"
#include "MinHook/MinHook.h"
#include "Function/日志打印.h"
#include "Function/微信多开.h"
#include "Function/消息监控.h"
#include "Function/通讯录.h"
#include "Function/消息上传.h"
#include "微信偏移.h"
#include "ApiServer.h"
#include "WeChat/ChatMsg.h"

void WeChatDLL::InitDLL()
{
	m_hWeChatWinDLL = (DWORD)LoadLibraryA("WeChatWin.dll");

	Patch_微信多开();

	//HOOK_消息监控();
	//HOOK_Contact();
	//修改微信版本至3.6.0.18
	//写内存_HEX(-1, m_hWeChatWinDLL + 微信偏移_版本地址, "12000663");

	std::thread thServer(StartApiServer,5000);
	thServer.detach();
}

WeChatDLL::WeChatDLL()
{
	MH_Initialize();

	//m_MsgRecvLoger = spdlog::basic_logger_mt("MsgRecv", "logs/MsgRecv_" + std::to_string(GetCurrentProcessId()) + ".log");
	//m_MsgRecvLoger->flush_on(spdlog::level::err);
	//m_WechatDLLLoger = spdlog::basic_logger_mt("WeChatDLL", "logs/WeChatDLL_" + std::to_string(GetCurrentProcessId()) + ".log");
	//m_WechatDLLLoger->flush_on(spdlog::level::err);
}

WeChatDLL::~WeChatDLL()
{
	MH_Uninitialize();
}

WeChatDLL* WeChatDLL::Instance()
{
	static WeChatDLL Wechat;
	return &Wechat;
}

std::shared_ptr<spdlog::logger>& WeChatDLL::MsgRecvLogger()
{
	return this->m_MsgRecvLoger;
}

std::shared_ptr<spdlog::logger>& WeChatDLL::WeChatLogger()
{
	return this->m_WechatDLLLoger;
}

DWORD WeChatDLL::getWinMoudule()
{
	return m_hWeChatWinDLL;
}

void WeChatDLL::FreeDLL()
{

}
