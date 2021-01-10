# 不需要建立连接
import socket
import time
import datetime
import pyaudio
import wave

# 创建socket对象
# SOCK_DGRAM    udp模式
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# 发送数据 字节
# 每个缓冲区的帧数
CHUNK = 1024  

# 采样位数
FORMAT = pyaudio.paInt16 

# 单声道
CHANNELS = 1  

# 采样频率
RATE = 44100  
def record_audio(wave_out_path, record_second):

    """ 录音功能 """
    # 实例化对象
    p = pyaudio.PyAudio()  

    # 打开流，传入响应参数
    stream = p.open(format=FORMAT,
                    channels=CHANNELS,
                    rate=RATE,
                    input=True,
                    frames_per_buffer=CHUNK)  

    # 打开 wav 文件。
    wf = wave.open(wave_out_path, 'wb')  
    # 声道设置
    wf.setnchannels(CHANNELS)  
    # 采样位数设置
    wf.setsampwidth(p.get_sample_size(FORMAT))  
    # 采样频率设置
    wf.setframerate(RATE)  

# 写入数据
    for _ in range(0, int(RATE * record_second / CHUNK)):
        data = stream.read(CHUNK)
        wf.writeframes(data)  
        # 关闭流
    stream.stop_stream()  
    stream.close()
    p.terminate()
    wf.close()
record_audio("d:\\123.wav",10)
def play_audio(wave_input_path):
     # 实例化
    p = pyaudio.PyAudio() 
     # 读 wav 文件
    wf = wave.open(wave_input_path, 'rb') 
    stream = p.open(format=p.get_format_from_width(wf.getsampwidth()),
                    channels=wf.getnchannels(),
                    rate=wf.getframerate(),
                    output=True)
    # 读数据
    data = wf.readframes(CHUNK)  
    while len(data) > 0:
        stream.write(data)
        data = wf.readframes(CHUNK)

# 关闭资源
    stream.stop_stream()  
    stream.close()
    p.terminate()
play_audio("d:\\123.wav")


# while True:
#     x = input("> ")
#     t = time.time()
#     t = int(round(t * 1000000))
#     t=str(t)
#     # s.sendto(t.encode(), ("10.12.33.161", 8000))
#     s.sendto(t.encode(), ("192.168.1.109", 8000))
