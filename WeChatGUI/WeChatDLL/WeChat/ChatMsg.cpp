#include "ChatMsg.h"
#include "../public/Strings.h"
#include "../WeChatDLL.h"
#include <AnyCall/AnyCall.h>

MyChatMsg CopyChatMsg(ChatMsg* pChatMsg)
{
	MyChatMsg ret;
	ret.msgType = pChatMsg->msgType;
	ret.msgID = pChatMsg->newMsgId;
	ret.IsOwner = pChatMsg->isOwner;
	ret.FromUserName = UnicodeToAnsi(copyMMString(&pChatMsg->fromUserName).c_str());
	if (!pChatMsg->isOwner) {
		ret.sendWxid = UnicodeToAnsi(copyMMString(&pChatMsg->mixExtra.sendWxid).c_str());
	}
	ret.msgContent = UnicodeToAnsi(copyMMString(&pChatMsg->msgContent).c_str());
	ret.CreateTime = pChatMsg->createTime;
	ret.imagePath = UnicodeToAnsi(copyMMString(&pChatMsg->mixExtra.imagePath).c_str());
	return ret;
}

ChatMsgX::ChatMsgX()
{
	
}

//如何找到析构函数,AppMsgMgr::sendFile
ChatMsgX::~ChatMsgX()
{
	switch (WeChatDLL::Instance().getWechatVersion()) {
	case WeChat_3_7_6_44:
		AnyCall::invokeThiscall<void>((void*)this, (void*)(WeChatDLL::Instance().getWinMoudule() + 0x131EB0));
		return;
	case WeChat_3_8_0_33:
		AnyCall::invokeThiscall<void>((void*)this, (void*)(WeChatDLL::Instance().getWinMoudule() + 0x651C40));
		return;
	}

}
