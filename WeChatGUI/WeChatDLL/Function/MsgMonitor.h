#pragma once
#include <vector>
#include <string>
#include <list>
#include <mutex>
#include <nlohmann/json.hpp>
#include "../public/RWMutex.h"

enum msgCategory
{
	WX_Unknow = 0x0,
	//��¼��ά����Ϣ
	WX_LoginQRCodeMsg = 0x20000,
};

struct MsgUploadInfo
{
	nlohmann::json msg;
};

class MsgBuffer
{
public:
	MsgBuffer();
	void pushMsg(MsgUploadInfo& msg);
	void popMsg(std::vector<MsgUploadInfo>& outMsgList);
private:
	std::list<MsgUploadInfo> buffer;
	std::mutex mute;
};

enum WeChatVersion;
class MsgMonitor
{
public:
	static MsgMonitor& Instance();
	bool InitMsgMonitor(WeChatVersion ver);
	std::vector<MsgUploadInfo> SyncMsg();
	void AddMsg(MsgUploadInfo& msg);

	//��ȡȺ��Ա�¼�
	bool getAddMemberEventMsg(std::wstring& eventName, MsgUploadInfo& outInfo);
	//���Ⱥ��Ա�¼�
	void setAddMemberEventMsg(std::wstring& eventName,MsgUploadInfo& msg);
private:
	WeChatVersion WeChatVer;
	MsgBuffer msgBuffer;

	//�¼���Ϣ��
	std::map<std::wstring, MsgUploadInfo> addMemberEventMap;
	std::mutex addEventMutex;
};