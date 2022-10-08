#pragma once
#include <string>

enum WeChatVersion;
class AccountFunction
{
public:
	static AccountFunction& Instance();
	bool InitAccountModule(WeChatVersion ver);
public:
	static std::string currentUserWxid;
private:
	WeChatVersion WeChatVer;
};