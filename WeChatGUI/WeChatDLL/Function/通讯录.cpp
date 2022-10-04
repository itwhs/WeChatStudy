#include "通讯录.h"
#include <unordered_map>
#include "../InlineHook/InlineHook.h"
#include "../WeChatDLL.h"
#include "../public/Strings.h"
#include "../public/RWMutex.h"
#include "../public/Public.h"
#include <AnyCall/AnyCall.h>

//InlineHook gHook_getListFromDB;
//InlineHook gHook_updateList;

void* gContactMgr = 0;
void* gCallGetContact = 0;

//std::unordered_map<std::string, MyContact> gContactMap;
//RWMutex gContactMutex;

////微信每次启动都会先从数据库读取已有的通讯录
//void __stdcall MyGetListFromDB(HookContext* hookContext)
//{
//	Contact* pContact = (Contact*)hookContext->EDI;
//	MyContact tmpContact = copyContact(pContact);
//	gContactMap[tmpContact.userName] = tmpContact;
//}

////微信动态更新通讯录
//void __stdcall MyUpdateList(HookContext* hookContext)
//{
//	BYTE* pVector = *(BYTE**)(hookContext->ESP + 4);
//	BYTE* pVectorStart = *(BYTE**)pVector;
//	BYTE* pVectorEnd = *(BYTE**)(pVector + 4);
//
//	int listSize = (pVectorEnd - pVectorStart) / 0x3E8;
//	if (listSize == 0) {
//		return;
//	}
//
//	gContactMutex.wLock();
//	for (int n = 0; n < listSize; ++n) {
//		MyContact tmpContact = copyContact((Contact*)pVectorStart);
//		gContactMap[tmpContact.userName] = tmpContact;
//		pVectorStart = pVectorStart + 0x3E8;
//	}
//	gContactMutex.wUnlock();
//}

bool HOOK_Contact()
{
	DWORD hWeChatWinDLL = WeChatDLL::Instance()->getWinMoudule();
	gContactMgr = LPVOID(hWeChatWinDLL + 0x21DD240);
	gCallGetContact = LPVOID(hWeChatWinDLL + 0x3E4260);
	//gHook_getListFromDB.AddHook(LPVOID(hWeChatWinDLL + 0x638980), MyGetListFromDB);
	//gHook_getListFromDB.AddHook(LPVOID(hWeChatWinDLL + 0x63B480), MyUpdateList);
	return true;
}

//MyContact getContactInfo(std::string userName)
//{
//	MyContact ret;
//	gContactMutex.rLock();
//	ret = gContactMap[userName];
//	gContactMutex.rUnLock();
//	return ret;
//}

MyContact getContactInfoDynamic(std::string userName)
{
	MyContact ret;
	Contact tmpContact;
	std::wstring wUserName = AnsiToUnicode(userName.c_str());
	mmString mUserName;
	mUserName.assign(wUserName.c_str(), wUserName.length());
	if (!AnyCall::invokeThiscall<bool>((void*)ReadUint(gContactMgr), gCallGetContact, mUserName, &tmpContact)) {
		ret.userName = userName;
		ret.nickName = userName;
		return ret;
	}
	ret = copyContact(&tmpContact);
	if (ret.nickName == "") {
		ret.nickName = userName;
	}
	return ret;
}



