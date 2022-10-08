#include "消息上传.h"
#include <httplib.h>
#include <json.hpp>
#include <thread>
#include <ThreadPool.h>
#include <mutex>
#include <vector>
#include "../public/Strings.h"
#include "../WeChatDLL.h"

std::vector<GroupMsgInfo> gMsgList;
std::mutex gMutex_Msg;
ThreadPool gUploadThread(5);

void Msg_Upload(std::vector<GroupMsgInfo> vec_Msg)
{
	
}



void thread_MsgUpload()
{
	while (true) {
		std::this_thread::sleep_for(std::chrono::seconds(10));
		gMutex_Msg.lock();
		if (gMsgList.size()) {
			gUploadThread.enqueue(Msg_Upload, gMsgList);
			gMsgList.clear();
		}
		gMutex_Msg.unlock();
	}
}

void PLUGIN_消息上传()
{
	
}
