## 微信多开(已完成)

关键字，_WeChat_App_Instance_Identity_Mutex_Name

## 消息监控

先是SyncMgr::doAddMsg，解析micromsg::AddMsg，转换成ChatMsg。

然后是SyncMgr::addMsgListToDB，将ChatMsg添加到数据库中。

## 文本消息发送(已经完成)

消息类型

```
1 = 文本消息
```

首先在ExtendChatView某个类函数中读取要发送的消息列表，之后调用ChatViewModel::batchSendMsg批量发送消息，最终走的是SendMessageMgr。

```
ExtendChatView::sub_some ->  ChatViewModel::batchSendMsg -> SendMessageMgr::sendMsg
{
	SendMessageMgr::sendTextMsg
	SendMessageMgr::sendImageMsg
}
```





## 微信通讯录





## 微信自动更新

微信首先会从WeChatWin.dll中读出版本号，存在到一个全局变量中。

UpdateMgr里面存着一个要更新的微信地址，和全局变量进行对比来判断是否要更新。
