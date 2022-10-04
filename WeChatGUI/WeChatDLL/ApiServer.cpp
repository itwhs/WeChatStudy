#include "ApiServer.h"
#include <httplib.h>
#include <nlohmann/json.hpp>
#include "WeChat/ChatMsg.h"
#include "WeChatDLL.h"
#include <AnyCall/AnyCall.h>
#include "WeChat/common.h"

unsigned int gWechatInstance;

void Api_Hello(const httplib::Request& req,httplib::Response& res)
{
	res.set_content("Hello World!", "text/plain");
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
	ChatMsg objMsg;
	MymmString sendWxid, sendMsg;
	sendWxid.data.assignUTF8(toWxid.c_str());
	sendMsg.data.assignUTF8(msgContent.c_str());
	unsigned int unknowField = 0x0;
	AnyCall::invokeAnycall(&objMsg, &sendWxid.data, (void*)(gWechatInstance + 0x5CD2E0), &sendMsg.data, &unknowField, (void*)1, 0, 0x0);
	retJson["code"] = 200;
	retJson["msg"] = "send ok";
	res.set_content(retJson.dump(), "application/json");
	return;
}

void StartApiServer(unsigned short port)
{
	gWechatInstance = WeChatDLL::Instance()->getWinMoudule();

	httplib::Server svr;
	svr.Get("/hello", Api_Hello);
	svr.Post("/sendTextMsg", Api_sendTextMsg);
	svr.listen("0.0.0.0",port);
}