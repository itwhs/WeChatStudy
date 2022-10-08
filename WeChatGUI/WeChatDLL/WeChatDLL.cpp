#include "WeChatDLL.h"
#include "public/WinApi.h"
#include "InlineHook/InlineHook.h"
#include "MinHook/MinHook.h"
#include "Function/LogFunction.h"
#include "Function/微信多开.h"
#include "Function/MsgMonitor.h"
#include "Function/ContactFunction.h"
#include "Function/AccountFunction.h"
#include "Function/消息上传.h"
#include "微信偏移.h"
#include "ApiServer.h"
#include "WeChat/ChatMsg.h"

void WeChatDLL::InitDLL()
{
	m_hWeChatWinDLL = (DWORD)LoadLibraryA("WeChatWin.dll");
	std::string dllPath = getModulePath((HMODULE)m_hWeChatWinDLL);
	std::string dllVersion = getFileVersion(dllPath.c_str());
	if (dllVersion.empty()) {
		return;
	}

	if (dllVersion == "3.7.6.44") {
		m_WechatVer = WeChat_3_7_6_44;
	}
	
	Patch_微信多开(m_WechatVer);
	ContactModule::Instance().InitContactModule(m_WechatVer);
	if (!AccountFunction::Instance().InitAccountModule(m_WechatVer)) {
		return;
	}
	if (!MsgMonitor::Instance().InitMsgMonitor(m_WechatVer)) {
		return;
	}

	std::thread thServer(StartApiServer, 5000);
	thServer.detach();
	
	//HOOK_Contact();
	//修改微信版本至3.6.0.18
	//写内存_HEX(-1, m_hWeChatWinDLL + 微信偏移_版本地址, "12000663");
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

WeChatDLL& WeChatDLL::Instance()
{
	static WeChatDLL Wechat;
	return Wechat;
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


WeChatVersion WeChatDLL::getWechatVersion()
{
	return m_WechatVer;
}

void WeChatDLL::FreeDLL()
{

}
