#pragma once
#include <windows.h>
#include "spdlog/sinks/basic_file_sink.h"

enum WechatVersion
{
	WeChat_NotSupport = 0x0,
	WeChat_3_7_6_44 = 0x1,
};

//OTM0ODIxNzMw
class WeChatDLL
{
public:
	WeChatDLL();
	~WeChatDLL();
public:
	static WeChatDLL& Instance();
	std::shared_ptr<spdlog::logger>& MsgRecvLogger();
	std::shared_ptr<spdlog::logger>& WeChatLogger();
	DWORD getWinMoudule();
	void FreeDLL();
	void InitDLL();
private:
	DWORD m_hWeChatWinDLL;
	WechatVersion m_WechatVer;
	std::shared_ptr<spdlog::logger> m_MsgRecvLoger;
	std::shared_ptr<spdlog::logger> m_WechatDLLLoger;
};
