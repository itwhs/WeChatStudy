import time

import webot
import requests

#给大家做个示例吧
def testDemo():

    wechat = webot.new_bot(1234)
    userWxid = wechat.wait_util_login()
    print("登录成功",userWxid)

    while True:
        msgList = wechat.sync_msg()
        print(msgList)
        time.sleep(3)
        pass

    return

if __name__ == '__main__':
    testDemo()

