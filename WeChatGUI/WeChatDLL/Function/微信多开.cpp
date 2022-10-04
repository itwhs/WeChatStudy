//中文
#include "微信多开.h"
#include "../微信偏移.h"
#include "../public/WinApi.h"
#include "../WeChatDLL.h"

bool Patch_微信多开()
{
	return 写内存_HEX(-1, WeChatDLL::Instance()->getWinMoudule() + 微信偏移_多开, "EB");
}

