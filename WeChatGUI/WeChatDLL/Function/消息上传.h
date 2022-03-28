#pragma once
#include <string>

//文本类型必须是utf8
struct GroupMsgInfo
{
	//消息类型
	int msgType;
	//发送者的名称
	std::string senderName;
	//发送者的WXID
	std::string senderWxid;
	//消息内容
	std::string msgContent;
	//群名
	std::string groupName;
	//群ID
	std::string groupID;
	//机器人WXID
	std::string robotID;
	//消息发送时间
	std::uint64_t postTime;
};

void UploadMsg(GroupMsgInfo& msgInfo);

void PLUGIN_消息上传();