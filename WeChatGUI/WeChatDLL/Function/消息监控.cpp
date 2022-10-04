#include "消息监控.h"
#include <json.hpp>
#include <fstream>
#include <vector>
#include <httplib.h>
#include <AnyCall/AnyCall.h>
#include "../InlineHook/InlineHook.h"
#include "../WeChatDLL.h"
#include "../TinyXml/tinyxml2.h"
#include "../public/Strings.h"
#include "../public/Public.h"
#include "../public/WinApi.h"
#include "../WeChat/ChatMsg.h"
#include "../WeChatDLL.h"
#include "../微信偏移.h"
#include "通讯录.h"
#include "消息上传.h"


InlineHook gHook_AddChatMsg;
InlineHook gHook_ImageDownload;
InlineHook gHook_SetCurrentUserWxid;
std::mutex gMutexMsg;

//当前用户的wxid
std::string gStr_CurrentUserWxid;


void WeChatNormalImageHandler(MyChatMsg& chatMsg)
{

}

void WeChatGroupImageHandler(MyChatMsg& chatMsg)
{
	GroupMsgInfo tmpMsgInfo;
	tmpMsgInfo.msgType = chatMsg.msgType;
	tmpMsgInfo.robotID = gStr_CurrentUserWxid;
	tmpMsgInfo.senderWxid = chatMsg.sendWxid;
	tmpMsgInfo.senderName = LocalCpToUtf8(getContactInfoDynamic(chatMsg.sendWxid).nickName.c_str());
	tmpMsgInfo.groupID = chatMsg.FromUserName;
	tmpMsgInfo.groupName = LocalCpToUtf8(getContactInfoDynamic(chatMsg.FromUserName).nickName.c_str());
	tmpMsgInfo.postTime = 1000 * uint64_t(chatMsg.CreateTime);
	if (chatMsg.imagePath == "") {
		return;
	}
	tmpMsgInfo.msgContent = ReadFileToString(chatMsg.imagePath.c_str());
	if (!tmpMsgInfo.msgContent.size()) {
		return;
	}
	UploadMsg(tmpMsgInfo);
}

void __stdcall MyDownloadImageSuccessed(HookContext* hookContext)
{
	char* pNetSceneGetMsgImgCDN = (char*)hookContext->ECX;
	ChatMsg* pChatMsg = (ChatMsg*)(pNetSceneGetMsgImgCDN + 0x5F4);

	MyChatMsg myChatMsg = CopyChatMsg(pChatMsg);
	mmString* pImageDownloadPath = (mmString*)(pNetSceneGetMsgImgCDN + 0x98C);
	myChatMsg.imagePath = UnicodeToAnsi(copyMMString(pImageDownloadPath).c_str());
	if (myChatMsg.FromUserName.find("@chatroom") != std::string::npos) {
		WeChatGroupImageHandler(myChatMsg);
	}
	else {
		WeChatNormalImageHandler(myChatMsg);
	}
}

void Handle_TicketInfoMsg(MyChatMsg& chatMsg)
{
	GroupMsgInfo tmpMsg;
	tmpMsg.msgType = chatMsg.msgType;
	tmpMsg.robotID = gStr_CurrentUserWxid;
	tmpMsg.postTime = 1000 * uint64_t(chatMsg.CreateTime);
	tmpMsg.groupID = chatMsg.FromUserName;
	tmpMsg.groupName = LocalCpToUtf8(getContactInfoDynamic(chatMsg.FromUserName).nickName.c_str());
	tmpMsg.senderWxid = chatMsg.sendWxid;
	tmpMsg.senderName = LocalCpToUtf8(getContactInfoDynamic(chatMsg.sendWxid).nickName.c_str());
	tmpMsg.msgContent = LocalCpToUtf8(chatMsg.msgContent.c_str());
	UploadMsg(tmpMsg);
}

void Handle_EmojiChatMsg(MyChatMsg& chatMsg)
{
	GroupMsgInfo tmpMsg;
	tmpMsg.msgType = chatMsg.msgType;
	tmpMsg.robotID = gStr_CurrentUserWxid;
	tmpMsg.postTime = 1000 * uint64_t(chatMsg.CreateTime);
	tmpMsg.groupID = chatMsg.FromUserName;
	tmpMsg.groupName = LocalCpToUtf8(getContactInfoDynamic(chatMsg.FromUserName).nickName.c_str());
	tmpMsg.senderWxid = chatMsg.sendWxid;
	tmpMsg.senderName = LocalCpToUtf8(getContactInfoDynamic(chatMsg.sendWxid).nickName.c_str());

	tinyxml2::XMLDocument xmlDocument;
	if (xmlDocument.Parse(chatMsg.msgContent.c_str()) != tinyxml2::XMLError::XML_SUCCESS) {
		WeChatDLL::Instance()->MsgRecvLogger()->error("[Handle_EmojiMsg]解析消息失败:" + chatMsg.msgContent);
		return;
	}
	auto elementMsg = xmlDocument.FirstChildElement("msg");
	if (!elementMsg) {
		WeChatDLL::Instance()->MsgRecvLogger()->error("[Handle_EmojiMsg]未找到msg消息节点:" + chatMsg.msgContent);
		return;
	}
	auto elementEmoji = elementMsg->FirstChildElement("emoji");
	if (!elementEmoji) {
		WeChatDLL::Instance()->MsgRecvLogger()->error("[Handle_EmojiMsg]未找到emoji消息节点:" + chatMsg.msgContent);
		return;
	}
	auto elementCDN = elementEmoji->Attribute("cdnurl");
	if (!elementCDN) {
		WeChatDLL::Instance()->MsgRecvLogger()->error("[Handle_EmojiMsg]未找到cdnurl消息节点:" + chatMsg.msgContent);
		return;
	}
	tmpMsg.msgContent = LocalCpToUtf8(elementCDN);
	UploadMsg(tmpMsg);
}

void Handle_NormalChatMsg(MyChatMsg& chatMsg)
{
	GroupMsgInfo tmpMsg;
	tmpMsg.msgType = chatMsg.msgType;
	tmpMsg.robotID = gStr_CurrentUserWxid;
	tmpMsg.postTime = 1000 * uint64_t(chatMsg.CreateTime);
	tmpMsg.groupID = chatMsg.FromUserName;
	tmpMsg.groupName = LocalCpToUtf8(getContactInfoDynamic(chatMsg.FromUserName).nickName.c_str());
	tmpMsg.senderWxid = chatMsg.sendWxid;
	tmpMsg.senderName = LocalCpToUtf8(getContactInfoDynamic(chatMsg.sendWxid).nickName.c_str());
	tmpMsg.msgContent = LocalCpToUtf8(chatMsg.msgContent.c_str());
	
	UploadMsg(tmpMsg);
}

bool CopyXmlElementText(tinyxml2::XMLElement* dst, tinyxml2::XMLElement* src, const char* nodeName)
{
	auto nodeElement = src->FirstChildElement(nodeName);
	if (!nodeElement) {
		return false;
	}
	auto newNodeElement = dst->FirstChildElement(nodeName);
	if (!newNodeElement) {
		return false;
	}
	const char* pText = nodeElement->GetText();
	if (pText) {
		newNodeElement->SetText(pText);
	}
	else {
		dst->DeleteChild(newNodeElement);
	}
	return true;
}

std::string ParseAppMsg(std::string& appMsg)
{
	tinyxml2::XMLDocument originDocument;
	if (originDocument.Parse(appMsg.c_str()) != tinyxml2::XMLError::XML_SUCCESS) {
		WeChatDLL::Instance()->MsgRecvLogger()->error("[ParseAppMsg]" + appMsg);
		return "";
	}

	auto originMsgElement = originDocument.FirstChildElement("msg");
	if (!originMsgElement) {
		return "";
	}

	//先声明好要存储的结果
	tinyxml2::XMLDocument newDocument;
	newDocument.InsertFirstChild(newDocument.NewDeclaration());
	tinyxml2::XMLElement* newMsgElement = newDocument.NewElement("msg");
	newDocument.InsertEndChild(newMsgElement);
	newMsgElement->InsertNewChildElement("appmsg");
	newMsgElement->InsertNewChildElement("appinfo");

	tinyxml2::XMLElement* newAppMsgElement = newMsgElement->FirstChildElement("appmsg");
	newAppMsgElement->InsertNewChildElement("title");
	newAppMsgElement->InsertNewChildElement("url");
	newAppMsgElement->InsertNewChildElement("des");
	newAppMsgElement->InsertNewChildElement("weappinfo");

	tinyxml2::XMLElement* newWeAppInfoElement = newAppMsgElement->FirstChildElement("weappinfo");
	newWeAppInfoElement->InsertNewChildElement("pagepath");
	newWeAppInfoElement->InsertNewChildElement("username");
	newWeAppInfoElement->InsertNewChildElement("weappiconurl");
	newWeAppInfoElement->InsertNewChildElement("shareId");

	tinyxml2::XMLElement* newAppInfoElement = newAppMsgElement->InsertNewChildElement("appinfo");
	newAppInfoElement->InsertNewChildElement("version");
	newAppInfoElement->InsertNewChildElement("appname");

	//开始转移数据
	auto appmsgElement = originMsgElement->FirstChildElement("appmsg");
	if (appmsgElement) {
		if (appmsgElement->Attribute("appid")) {
			newAppMsgElement->SetAttribute("appid", appmsgElement->Attribute("appid"));
		}
		CopyXmlElementText(newAppMsgElement, appmsgElement, "title");
		CopyXmlElementText(newAppMsgElement, appmsgElement, "url");
		CopyXmlElementText(newAppMsgElement, appmsgElement, "des");

		auto newWeAppInfoElement = newAppMsgElement->FirstChildElement("weappinfo");
		auto weappinfoElement = appmsgElement->FirstChildElement("weappinfo");
		if (weappinfoElement) {
			CopyXmlElementText(newWeAppInfoElement, weappinfoElement, "pagepath");
			CopyXmlElementText(newWeAppInfoElement, weappinfoElement, "username");
			CopyXmlElementText(newWeAppInfoElement, weappinfoElement, "weappiconurl");
			CopyXmlElementText(newWeAppInfoElement, weappinfoElement, "shareId");
		}
	}

	auto appinfoElement = originMsgElement->FirstChildElement("appinfo");
	if (appinfoElement) {
		CopyXmlElementText(newAppInfoElement, appinfoElement, "version");
		CopyXmlElementText(newAppInfoElement, appinfoElement, "appname");
	}

	tinyxml2::XMLPrinter printer;
	newDocument.Print(&printer);
	return printer.CStr();
}

void Handle_AppChatMsg(MyChatMsg& chatMsg)
{
	GroupMsgInfo tmpMsg;
	tmpMsg.msgType = chatMsg.msgType;
	tmpMsg.robotID = gStr_CurrentUserWxid;
	tmpMsg.postTime = 1000 * uint64_t(chatMsg.CreateTime);
	tmpMsg.groupID = chatMsg.FromUserName;
	tmpMsg.groupName = LocalCpToUtf8(getContactInfoDynamic(chatMsg.FromUserName).nickName.c_str());
	tmpMsg.senderWxid = chatMsg.sendWxid;
	tmpMsg.senderName = LocalCpToUtf8(getContactInfoDynamic(chatMsg.sendWxid).nickName.c_str());

	std::string appMsg = ParseAppMsg(chatMsg.msgContent);
	tmpMsg.msgContent = LocalCpToUtf8(appMsg.c_str());
	UploadMsg(tmpMsg);
}

void WeChatGroupChatMsgHandler(MyChatMsg& chatMsg)
{
	switch (chatMsg.msgType)
	{
	case 1:
		Handle_NormalChatMsg(chatMsg);	//普通消息
		break;
	case 3:
		//不在这里处理图片消息
		break;
	case 42:
		Handle_TicketInfoMsg(chatMsg);  //名片消息
		break;
	case 43:
		//Handle_VideoMsg(addMsg);    //视频消息
		break;
	case 47:
		Handle_EmojiChatMsg(chatMsg);	//表情消息
		break;
	case 49:
		Handle_AppChatMsg(chatMsg);		//应用消息
		break;
	case 10000:
		//系统通知
		break;
	default:
		int a = 0;
		break;
	}
}

void __stdcall MyAddChatMsg(HookContext* hookContext)
{
	int* pVector = *(int**)hookContext->ESP;
	char* pVectorStart = (char*)pVector[0];
	char* pVecotrEnd = (char*)pVector[1];

	unsigned int msgCount = (pVecotrEnd - pVectorStart) / 0x290;
	if (msgCount == 0) {
		return;
	}

	for (unsigned int n = 0; n < msgCount; ++n) {

		MyChatMsg tmpMsg = CopyChatMsg((ChatMsg*)pVectorStart);

		if (tmpMsg.IsOwner) {
			tmpMsg.sendWxid = gStr_CurrentUserWxid;
		}

		//群聊消息
		if (tmpMsg.FromUserName.find("@chatroom") != std::string::npos) {
			WeChatGroupChatMsgHandler(tmpMsg);
		}
		else {
			//WeChatNormalMsgHandler();
		}

		pVectorStart = pVectorStart + 0x290;
	}
}

void __stdcall MySetCurrentUserWxid(HookContext* hookContext)
{
	char* pWxid = *(char**)(hookContext->ESP + 0x4);
	gStr_CurrentUserWxid = pWxid;
}

bool HOOK_消息监控()
{
	DWORD hWeChatWinDLL = WeChatDLL::Instance()->getWinMoudule();

	gHook_AddChatMsg.AddHook((LPVOID)(hWeChatWinDLL + 0x4CAE11), MyAddChatMsg);
	//gHook_ImageDownload.AddHook((LPVOID)(hWeChatWinDLL + 0x58AD50), MyDownloadImageSuccessed);
	//gHook_SetCurrentUserWxid.AddHook((LPVOID)(hWeChatWinDLL + 0x5045C0), MySetCurrentUserWxid);

	//开启图片自动下载
	//写内存_HEX(-1, WeChatDLL::Instance()->getWinMoudule() + 微信偏移_图片自动下载, "B001C3");
	return true;
}