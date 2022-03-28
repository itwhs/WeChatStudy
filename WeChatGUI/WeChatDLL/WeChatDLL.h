#pragma once
#include <windows.h>
#include "spdlog/sinks/basic_file_sink.h"

class WeChatDLL
{
public:
	WeChatDLL();
	~WeChatDLL();
public:
	static WeChatDLL* Instance();
	std::shared_ptr<spdlog::logger>& MsgRecvLogger();
	std::shared_ptr<spdlog::logger>& WeChatLogger();
	DWORD getWinMoudule();
	void FreeDLL();
	void InitDLL();
private:
	DWORD m_hWeChatWinDLL;
	std::shared_ptr<spdlog::logger> m_MsgRecvLoger;
	std::shared_ptr<spdlog::logger> m_WechatDLLLoger;
};
