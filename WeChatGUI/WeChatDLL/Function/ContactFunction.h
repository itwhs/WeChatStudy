#pragma once
#include <string>
#include "../WeChat/Contact.h"

enum WeChatVersion;
class ContactModule
{
public:
	static ContactModule& Instance();
	void InitContactModule(WeChatVersion ver);
	MyContact getContactInfoDynamic(std::string userName);
private:
	void* ContactMgr_Instance();
private:
	WeChatVersion WeChatVer;
};