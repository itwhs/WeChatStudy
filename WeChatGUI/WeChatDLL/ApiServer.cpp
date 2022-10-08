#include "ApiServer.h"
#include <httplib.h>
#include <nlohmann/json.hpp>
#include "WeChat/ChatMsg.h"
#include "WeChatDLL.h"
#include <AnyCall/AnyCall.h>
#include "Function/ContactFunction.h"
#include "WeChat/common.h"
#include "Public/Strings.h"
#include <MyTinySTL/vector.h>

unsigned int gWechatInstance;

void* SendMessageMgr_Instance()
{
	return AnyCall::invokeStdcall<void*>((void*)(WeChatDLL::Instance().getWinMoudule() + 0x141890));
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
	ChatMsg retChatMsg;
	memset(&retChatMsg, 0x0, sizeof(ChatMsg));
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
				nickName = ContactModule::Instance().getContactInfoDynamic(strAtUser).nickName;
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

//接受群消息
void Api_recvGroupMsg(const httplib::Request& req, httplib::Response& res)
{
	
}

//接受私聊消息
void Api_recvPrivateMsg(const httplib::Request& req, httplib::Response& res)
{

}

//接受朋友圈消息
void Api_recvSnsMsg(const httplib::Request& req, httplib::Response& res)
{

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

void StartApiServer(unsigned short port)
{
	gWechatInstance = WeChatDLL::Instance().getWinMoudule();

	httplib::Server svr;

	//消息相关
	svr.Get("/recvGroupMsg", Api_recvGroupMsg);
	svr.Get("/recvPrivateMsg", Api_recvPrivateMsg);
	svr.Post("/sendTextMsg", Api_sendTextMsg);
	svr.Post("/sendTextMsgEx", Api_sendTextMsgEx);
	svr.Post("/sendImageMsg", Api_sendImageMsg);

	svr.Get("/getCustomEmotionList", Api_getCustomEmotionList);
	svr.Post("/sendCustomEmotion", Api_sendCustomEmotion);

	svr.Get("/recvSnsMsg", Api_recvSnsMsg);
	svr.Get("/getLoginUserInfo", Api_getLoginUserInfo);




	svr.listen("0.0.0.0",port);
}