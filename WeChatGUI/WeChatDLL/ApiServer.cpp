#include "ApiServer.h"
#include <httplib.h>
#include <nlohmann/json.hpp>
#include "WeChat/ChatMsg.h"
#include "WeChatDLL.h"
#include <AnyCall/AnyCall.h>
#include "Function/ContactFunction.h"
#include "Function/MsgMonitor.h"
#include "Function/SnsFunction.h"
#include "WeChat/common.h"
#include "Public/Strings.h"
#include <MyTinySTL/vector.h>

unsigned int gWechatInstance;

void* SendMessageMgr_Instance()
{
	return AnyCall::invokeStdcall<void*>((void*)(WeChatDLL::Instance().getWinMoudule() + 0x141890));
}

void* AppMsgMgr_Instance()
{
	return AnyCall::invokeStdcall<void*>((void*)(WeChatDLL::Instance().getWinMoudule() + 0x1442E0));
}

void Api_SendFile(const httplib::Request& req, httplib::Response& res)
{
	nlohmann::json json = nlohmann::json::parse(req.body);
	nlohmann::json retJson;
	std::string toWxid = json["to_wxid"].get<std::string>();
	std::string msgContent = json["file_path"].get<std::string>();
	if (toWxid.empty() || msgContent.empty()) {
		retJson["code"] = 201;
		retJson["msg"] = "empty param";
		res.set_content(retJson.dump(), "application/json");
		return;
	}
	ChatMsgX retChatMsg;
	memset(&retChatMsg, 0x0, sizeof(ChatMsgX));
	MymmString sendWxid, filepath, unknowFiled1,unknowField2;
	sendWxid.assignUTF8(toWxid.c_str());
	filepath.assignUTF8(msgContent.c_str());
	AnyCall::invokeThiscall<void>(AppMsgMgr_Instance(), (void*)(gWechatInstance + 0x479B30), &retChatMsg,
		sendWxid, filepath, unknowFiled1, 0, unknowField2);
	retJson["code"] = 200;
	retJson["msg"] = "send ok";
	res.set_content(retJson.dump(), "application/json");
	return;
}

void Api_sendImageMsg(const httplib::Request& req, httplib::Response& res)
{
	nlohmann::json json = nlohmann::json::parse(req.body);
	nlohmann::json retJson;
	std::string toWxid = json["to_wxid"].get<std::string>();
	std::string msgContent = json["image_path"].get<std::string>();
	if (toWxid.empty() || msgContent.empty()) {
		retJson["code"] = 201;
		retJson["msg"] = "empty param";
		res.set_content(retJson.dump(), "application/json");
		return;
	}
	ChatMsgX retChatMsg;
	memset(&retChatMsg, 0x0, sizeof(ChatMsgX));
	MymmString sendWxid, imagePath, unknowFiled;
	sendWxid.assignUTF8(toWxid.c_str());
	imagePath.assignUTF8(msgContent.c_str());
	AnyCall::invokeThiscall<void>(SendMessageMgr_Instance(), (void*)(gWechatInstance + 0x5CCDD0), &retChatMsg,
		&sendWxid, &imagePath, unknowFiled);
	retJson["code"] = 200;
	retJson["msg"] = "send ok";
	res.set_content(retJson.dump(), "application/json");
	return;
}

void Api_sendTextMsg(const httplib::Request& req, httplib::Response& res)
{
	nlohmann::json json = nlohmann::json::parse(req.body);
	nlohmann::json retJson;
	std::string toWxid = json["to_wxid"].get<std::string>();
	std::string msgContent = json["msg"].get<std::string>();
	if (toWxid.empty() || msgContent.empty()) {
		retJson["code"] = 201;
		retJson["msg"] = "empty param";
		res.set_content(retJson.dump(), "application/json");
		return;
	}
	ChatMsg objMsg = { 0 };
	MymmString sendWxid, sendMsg;
	sendWxid.assignUTF8(toWxid.c_str());
	sendMsg.assignUTF8(msgContent.c_str());
	mystl::vector<MymmString> atUserList;
	AnyCall::invokeAnycall(&objMsg, &sendWxid, (void*)(gWechatInstance + 0x5CD2E0), &sendMsg, &atUserList, (void*)1, 0, 0x0);
	retJson["code"] = 200;
	retJson["msg"] = "send ok";
	res.set_content(retJson.dump(), "application/json");
	return;
}

void Api_sendTextMsgEx(const httplib::Request& req, httplib::Response& res)
{
	nlohmann::json json = nlohmann::json::parse(req.body);
	nlohmann::json retJson;
	MymmString toWxid;
	toWxid.assignUTF8(json["to_wxid"].get<std::string>().c_str());
	mystl::vector<MymmString> atUserList;
	std::string msgContent;
	auto msgList = json["msg_list"];
	for (unsigned int n = 0; n < msgList.size(); ++n) {
		int msgType = msgList[n]["type"];
		//普通文本
		if (msgType == 0) {
			std::string msg = msgList[n]["msg"];
			msgContent.append(msg);
		}
		//@用户
		else if (msgType == 1) {
			std::string strAtUser = msgList[n]["atUser"];
			std::string nickName = msgList[n]["nickName"];
			if (strAtUser == "notify@all") {
				nickName = LocalCpToUtf8("所有人");
			}
			else if (nickName.empty()) {
				nickName = ContactModule::Instance().GetContactInfoDynamic(strAtUser).nickName;
				nickName = LocalCpToUtf8(nickName.c_str());
			}
			msgContent.append("@" + nickName + " ");
			MymmString mmStrAtUser;
			mmStrAtUser.assignUTF8(strAtUser.c_str());
			atUserList.push_back(mmStrAtUser);
		}
	}
	ChatMsg objMsg;
	MymmString sendMsg;
	sendMsg.assignUTF8(msgContent.c_str());
	AnyCall::invokeAnycall(&objMsg, &toWxid, (void*)(gWechatInstance + 0x5CD2E0), &sendMsg, &atUserList, (void*)1, 0, 0x0);
	retJson["code"] = 200;
	retJson["msg"] = "send ok";
	res.set_content(retJson.dump(), "application/json");
	return;
}

//同步消息

void Api_syncMsg(const httplib::Request& req, httplib::Response& res)
{
	nlohmann::json retJson;
	std::vector<MsgUploadInfo> msgList = MsgMonitor::Instance().SyncMsg();
	nlohmann::json &jsonData = retJson["data"];
	for (unsigned int n = 0; n < msgList.size(); ++n) {
		nlohmann::json tmp;
		tmp["msg_type"] = msgList[n].msgType;
		tmp["msg_id"] = msgList[n].msgID;
		tmp["sender_name"] = msgList[n].senderName;
		tmp["sender_wxid"] = msgList[n].senderWxid;
		tmp["msg_content"] = msgList[n].msgContent;
		if (!msgList[n].wxid.empty()) {
			tmp["wxid"] = msgList[n].wxid;
			tmp["name"] = msgList[n].name;
		}
		tmp["robot_id"] = msgList[n].robotID;
		tmp["post_time"] = msgList[n].postTime;
		jsonData.push_back(tmp);
	}
	retJson["code"] = 200;
	res.set_content(retJson.dump(), "application/json");
	return;
}

//接收朋友圈消息

void Api_syncSns(const httplib::Request& req, httplib::Response& res)
{
	nlohmann::json retJson;
	std::vector<MyTimeLineResp> msgList = SnsModule::Instance().SyncSns();
	nlohmann::json& jsonData = retJson["data"];
	for (unsigned int n = 0; n < msgList.size(); ++n) {
		nlohmann::json tmp;
		tmp["id"] = msgList[n].id;
		tmp["post_time"] = msgList[n].sendTime;
		tmp["sender_wxid"] = LocalCpToUtf8(msgList[n].sendWxid.c_str());
		tmp["sender_name"] = LocalCpToUtf8(ContactModule::Instance().GetContactInfoDynamic(msgList[n].sendWxid).nickName.c_str());
		tmp["content"] = LocalCpToUtf8(msgList[n].content.c_str());
		tmp["title"] = LocalCpToUtf8(msgList[n].title.c_str());
		tmp["description"] = LocalCpToUtf8(msgList[n].description.c_str());
		tmp["content_url"] = LocalCpToUtf8(msgList[n].contentUrl.c_str());
		nlohmann::json& jsonMedia = tmp["media"];
		for (unsigned int m = 0; m < msgList[n].mediaList.size(); ++m) {
			nlohmann::json tmpMedia;
			tmpMedia["id"] = msgList[n].mediaList[m].id;
			tmpMedia["type"] = msgList[n].mediaList[m].type;
			tmpMedia["url"] = msgList[n].mediaList[m].url;
			jsonMedia.push_back(tmpMedia);
		}
		jsonData.push_back(tmp);
	}
	retJson["code"] = 200;
	res.set_content(retJson.dump(), "application/json");
	return;
}

//获取当前登录用户信息
void Api_getLoginUserInfo(const httplib::Request& req, httplib::Response& res)
{

}

//获取个人表情列表

void Api_getCustomEmotionList(const httplib::Request& req, httplib::Response& res)
{
	

}

//发送个人表情

void Api_sendCustomEmotion(const httplib::Request& req, httplib::Response& res)
{
	
}

void Api_getContactInfo(const httplib::Request& req, httplib::Response& res)
{
	nlohmann::json retJson;
	nlohmann::json json = nlohmann::json::parse(req.body);
	nlohmann::json& jsonData = retJson["data"];
	for (unsigned int n = 0; n < json.size(); ++n) {
		std::string userName = json[n];
		auto contantInfo = ContactModule::Instance().GetContactInfoDynamic(userName);
		nlohmann::json tmp;
		tmp["username"] = LocalCpToUtf8(contantInfo.userName.c_str());
		tmp["alias"] = LocalCpToUtf8(contantInfo.alias.c_str());
		tmp["encrypt_username"] = LocalCpToUtf8(contantInfo.encryptUserName.c_str());
		tmp["remark"] = LocalCpToUtf8(contantInfo.remark.c_str());
		tmp["nickname"] = LocalCpToUtf8(contantInfo.nickName.c_str());
		jsonData.push_back(tmp);
	}
	retJson["code"] = 200;
	res.set_content(retJson.dump(), "application/json");
	return;
}

void Api_getContactList(const httplib::Request& req, httplib::Response& res)
{
	nlohmann::json retJson;
	std::vector<MyContact> contactList = ContactModule::Instance().GetContactList();
	nlohmann::json& jsonData = retJson["data"];
	for (unsigned int n = 0; n < contactList.size(); ++n) {
		nlohmann::json tmp;
		MyContact& contantInfo = contactList[n];
		tmp["username"] = LocalCpToUtf8(contantInfo.userName.c_str());
		tmp["alias"] = LocalCpToUtf8(contantInfo.alias.c_str());
		tmp["encrypt_username"] = LocalCpToUtf8(contantInfo.encryptUserName.c_str());
		tmp["remark"] = LocalCpToUtf8(contantInfo.remark.c_str());
		tmp["nickname"] = LocalCpToUtf8(contantInfo.nickName.c_str());
		jsonData.push_back(tmp);
	}
	retJson["code"] = 200;
	retJson["count"] = contactList.size();
	res.set_content(retJson.dump(), "application/json");
	return;
}

void Api_getHome(const httplib::Request& req, httplib::Response& res)
{
	nlohmann::json retJson;
	retJson["code"] = 200;
	res.set_content(retJson.dump(), "application/json");
	return;
}

void StartApiServer(unsigned short port)
{
	gWechatInstance = WeChatDLL::Instance().getWinMoudule();

	httplib::Server svr;

	//消息相关
	svr.Get("/syncMsg", Api_syncMsg);
	svr.Get("/syncSns", Api_syncSns);

	svr.Post("/sendTextMsg", Api_sendTextMsg);
	svr.Post("/sendTextMsgEx", Api_sendTextMsgEx);
	svr.Post("/sendImageMsg", Api_sendImageMsg);
	svr.Post("/sendFile", Api_SendFile);


	//联系人相关
	svr.Post("/getContactInfo", Api_getContactInfo);
	//获取通讯录列表
	svr.Get("/getContactList", Api_getContactList);


	svr.Get("/getCustomEmotionList", Api_getCustomEmotionList);
	svr.Post("/sendCustomEmotion", Api_sendCustomEmotion);

	svr.Get("/", Api_getHome);
	svr.Get("/getLoginUserInfo", Api_getLoginUserInfo);
	svr.listen("0.0.0.0",port);
}