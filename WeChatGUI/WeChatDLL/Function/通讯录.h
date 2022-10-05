#pragma once
#include <string>
#include "../WeChat/Contact.h"

enum WechatVersion;
class ContactModule
{
public:
	static ContactModule& Instance();
	void InitContactModule(WechatVersion ver);
	MyContact getContactInfoDynamic(std::string userName);
private:
	void* ContactMgr_Instance();
private:
	WechatVersion WeChatVer;
};