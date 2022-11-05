# WeChatStudy
StudyWechat，Support Version

- [x]  3.7.6.44

The project is for study only, it is prohibited to use it for illegal purposes.

项目仅供学习参考，禁止用于非法用途。



# 项目说明

WeChatDLL为核心DLL，在微信上搭建了一个http服务。

WeChatClient为客户端，通过http协议与微信通讯，使用前需要安装以下python库。

```bash
pip3 install pywin32
pip3 install numpy
pip3 install pefile
```



# API接口说明

## 接收消息

```python
resp = requests.get("http://127.0.0.1:5000/syncMsg")
print(resp.text)
```

## 接收朋友圈消息(不推荐频繁调用)

```python
resp = requests.get("http://127.0.0.1:5000/syncSns")
print(resp.text)
```

## 获取通讯录列表

```python
resp = requests.get("http://127.0.0.1:5000/getContactList")
print(resp.text)
```

## 获取通讯录

```python
contactList = []
contactList.append("filehelper")
resp = requests.post("http://127.0.0.1:5000/getContactInfo",json = contactList)
print(resp.text)
```

## 发送文本消息

```python
data = dict()
data["to_wxid"] = "filehelper"
data["msg"] = "hello wechat"
resp = requests.post("http://127.0.0.1:5000/sendTextMsg", json = data)
print(resp.text)
```

## 发送图片

```python
data = dict()
data["to_wxid"] = "filehelper"
data["image_path"] = '''D:\\test.png'''
resp = requests.post("http://127.0.0.1:5000/sendImageMsg", json = data)
print(resp.text)
```

## 发送文件

```python
data = dict()
data["to_wxid"] = "filehelper"
data["file_path"] = '''D:\\test.bin'''
resp = requests.post("http://127.0.0.1:5000/sendFile", json = data)
print(resp.text)
```

