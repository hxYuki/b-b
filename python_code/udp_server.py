#!/usr/bin/python
# -*- coding: UTF-8 -*-
import socket
import time
import datetime
# import tkinter
# import tkMessageBox
# # 创建一个矩形，指定画布的颜色为白色
# from tkinter import *
# root = Tk()
# # 创建一个Canvas，设置其背景色为白色
# cv = Canvas(root,bg = 'white')
# # 创建一个矩形，坐标为(10,10,110,110)
# cv.create_rectangle(10,10,110,110)
# cv.pack()
# root.mainloop()
# # 为明显起见，将背景色设置为白色，用以区别 root
# top.mainloop()
# 创建socket对象
# SOCK_DGRAM    udp模式


s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# s.bind(("localhost", 8000))  # 绑定服务器的ip和端口
s.bind(("192.168.1.109", 8000)) 
print("server is running")

while True:
    data = s.recv(1024)  # 一次接收1024字节
    t = time.time()
    t = int(round(t * 1000000))
    print(str(t-int(data.decode()))+"us")  # decode()解码收到的字节