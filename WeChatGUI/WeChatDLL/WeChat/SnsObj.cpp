#include "SnsObj.h"
#include "../public/Strings.h"

void loadMediaData(SnsMedia* pMediaStart, SnsMedia* pMediaEnd, MyTimeLineResp& outResp)
{
	if (!pMediaStart) {
		return;
	}
	unsigned int count = pMediaEnd - pMediaStart;
	for (unsigned int n = 0; n < count; n++) {
		MySnsMedia tmpMedia;
		//大概是图片
		if (pMediaStart->type == 2) {
			tmpMedia.url = UnicodeToAnsi(copyMMString(&pMediaStart->Url).c_str());
		}
		outResp.mediaList.push_back(tmpMedia);
		pMediaStart++;
	}
}

MyTimeLineResp CopyTimeLineResp(TimelineResp* pTimelineResp)
{
	MyTimeLineResp retResp;

	retResp.id = pTimelineResp->id;
	retResp.sendTime = pTimelineResp->coreMsg.sendTime;
	retResp.sendWxid = UnicodeToAnsi(copyMMString(&pTimelineResp->coreMsg.senderName).c_str());
	retResp.content = UnicodeToAnsi(copyMMString(&pTimelineResp->coreMsg.contentDesc).c_str());
	SnsContentObject* pContentObject = &pTimelineResp->coreMsg.contentObject;
	//图片
	if (pContentObject->contentStyle == 1) {
		loadMediaData(pContentObject->pSnsMediaStart, pContentObject->pSnsMediaEnd, retResp);
		return retResp;
	}
	//转发文案3
	//视频15
	//视频号28
	retResp.title = UnicodeToAnsi(copyMMString(&pContentObject->title).c_str());
	retResp.description = UnicodeToAnsi(copyMMString(&pContentObject->description).c_str());
	retResp.contentUrl = UnicodeToAnsi(copyMMString(&pContentObject->contentUrl).c_str());
	return retResp;

}

