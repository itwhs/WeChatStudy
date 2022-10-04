#include "ChatMsg.h"
#include "../public/Strings.h"

MyChatMsg CopyChatMsg(ChatMsg* pChatMsg)
{
	MyChatMsg ret;
	ret.msgType = pChatMsg->msgType;
	ret.IsOwner = pChatMsg->isOwner;
	ret.FromUserName = UnicodeToAnsi(copyMMString(&pChatMsg->fromUserName).c_str());
	if (!pChatMsg->isOwner){
		ret.sendWxid = UnicodeToAnsi(copyMMString(&pChatMsg->mixExtra.sendWxid).c_str());
	}
	ret.msgContent = UnicodeToAnsi(copyMMString(&pChatMsg->msgContent).c_str());
	ret.CreateTime = pChatMsg->createTime;
	ret.imagePath = UnicodeToAnsi(copyMMString(&pChatMsg->mixExtra.imagePath).c_str());
	return ret;
}

