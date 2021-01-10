from socket import *
import threading
import cv2
import re
import time
import sys
import os
import struct
import pickle
import zlib
import wave

# 测试摄像头能不能打开
# aa=cv2.VideoCapture(0)
# for i in range(20):
#     aaa,bbb=aa.read()
# while aa.isOpened():
#     ret, frame = aa.read()
#     cv2.imshow("a",frame)
#     cv2.waitKey(1)
class Video_Client(threading.Thread):
    # 构造函数
    def __init__(self ,ip, port, level, version):
        # 线程初始化
        threading.Thread.__init__(self)
        self.setDaemon(True)
        self.ADDR = (ip, port)
        if level <= 3:
            self.interval = level
        else:
            self.interval = 3
        self.fx = 1 / (self.interval + 1)
        if self.fx < 0.3:
            self.fx = 0.3
        # ipv4和ipv6
        if version == 4:
            self.sock = socket(AF_INET, SOCK_STREAM)
        else:
            self.sock = socket(AF_INET6, SOCK_STREAM)
        # opencv打开摄像头函数
        self.cap = cv2.VideoCapture(0)
    # 析构函数
    def __del__(self) :
        self.sock.close()
        self.cap.release()
    def run(self):
        print("VEDIO client starts...")
        while True:# 断线重连
            try:
                self.sock.connect(self.ADDR)
                break
            except:
                time.sleep(3)
                continue
        print("VEDIO client connected...")
        while self.cap.isOpened():
            ret, frame = self.cap.read()
            # cv2.imshow("a",frame)
            sframe = cv2.resize(frame, (0,0), fx=self.fx, fy=self.fx)
            data = pickle.dumps(sframe)
            # 压缩图像
            zdata = zlib.compress(data, zlib.Z_BEST_COMPRESSION)
            try:
                self.sock.sendall(struct.pack("L", len(zdata)) + zdata)
            except:
                break
            for i in range(self.interval):
                self.cap.read()
# 服务器端最终代码如下，增加了对接收到数据的解压缩处理。

class Video_Server(threading.Thread):
    def __init__(self, port, version) :
        threading.Thread.__init__(self)
        self.setDaemon(True)
        self.ADDR = ('', port)
        if version == 4:
            self.sock = socket(AF_INET ,SOCK_STREAM)
        else:
            self.sock = socket(AF_INET6 ,SOCK_STREAM)
    def __del__(self):
        self.sock.close()
        try:
            cv2.destroyAllWindows()
        except:
            pass
    def run(self):
        print("VEDIO server starts...")
        self.sock.bind(self.ADDR)
        self.sock.listen(1)
        conn, addr = self.sock.accept()
        print("remote VEDIO client success connected...")
        data = "".encode("utf-8")
        payload_size = struct.calcsize("L")
        cv2.namedWindow('Remote', cv2.WINDOW_NORMAL)
        while True:
            while len(data) < payload_size:
                data += conn.recv(81920)
            packed_size = data[:payload_size]
            data = data[payload_size:]
            msg_size = struct.unpack("L", packed_size)[0]
            while len(data) < msg_size:
                data += conn.recv(81920)
            zframe_data = data[:msg_size]
            data = data[msg_size:]
            # 解压缩
            frame_data = zlib.decompress(zframe_data)
            frame = pickle.loads(frame_data)
            cv2.imshow('Remote', frame)
            if cv2.waitKey(1) & 0xFF == 27:
                break