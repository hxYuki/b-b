#include<pch.h>
#include"Network.h"
#include <winrt/Windows.UI.Composition.h>
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Networking::Sockets;
using namespace winrt::Windows::Storage::Streams;

namespace winrt::BiBi::implementation {

    
    void UdpClientStrt::StartServer()
    {
        try
        {
            //OutputDebugString(L"excuted");
            // 收到数据报回调函数
            //this->m_serverDatagramSocket.MessageReceived({ this, &UdpClientStrt::MessageReceived });

            // 开始监听
            this->m_serverDatagramSocket.BindServiceNameAsync(L"22229");

            // 加入组播组
            this->m_serverDatagramSocket.JoinMulticastGroup(winrt::Windows::Networking::HostName(L"229.2.2.9"));
        }
        catch (winrt::hresult_error const& ex)
        {
            Windows::Networking::Sockets::SocketErrorStatus webErrorStatus{ Windows::Networking::Sockets::SocketError::GetStatus(ex.to_abi()) };
            throw webErrorStatus != Windows::Networking::Sockets::SocketErrorStatus::Unknown ? winrt::to_hstring((int32_t)webErrorStatus) : winrt::to_hstring(ex.to_abi());
        }
    }

    
    IAsyncAction UdpClientStrt::AnnounceAsync()
    {
        try
        {
            Windows::Networking::HostName multicastHostName{ L"229.2.2.9" };
            // 获取输出流
            IOutputStream outputStream = co_await m_serverDatagramSocket.GetOutputStreamAsync(multicastHostName, L"22229");
    
            winrt::hstring msg{ L"b&b:"+ProtocolTokens::OnlineAnnouncement+L":imhere" };
            // 向流中写入数据
            DataWriter dataWriter{ outputStream };
            // 存在消息
            dataWriter.WriteString(msg);
            // 发送
            co_await dataWriter.StoreAsync();
            dataWriter.DetachStream();
        }
        catch (winrt::hresult_error const& ex)
        {
            Windows::Networking::Sockets::SocketErrorStatus webErrorStatus{ Windows::Networking::Sockets::SocketError::GetStatus(ex.to_abi()) };
            throw webErrorStatus != Windows::Networking::Sockets::SocketErrorStatus::Unknown ? winrt::to_hstring((int32_t)webErrorStatus) : winrt::to_hstring(ex.to_abi());
        }
    }
#pragma region Deprecated Code
    /*void UdpClientStrt::MessageReceived(Windows::Networking::Sockets::DatagramSocket const& sender, Windows::Networking::Sockets::DatagramSocketMessageReceivedEventArgs const& args)
    {*/
    //// 读取数据
    //DataReader dataReader{ args.GetDataReader() };
    //winrt::hstring msgReceived{ dataReader.ReadString(dataReader.UnconsumedBufferLength()) };
    //// 转换为wstring view
    //auto ms = std::wstring_view(msgReceived);
    //// 取协议头
    //if (ms.substr(0, 3) != L"b&b")
    //    return;
    //// 取token
    //auto token = ms.substr(4, 5);
    //// 寻找token
    //auto i = registry.find(hstring(token));
    //if (i == registry.end())
    //    return;

    //// 对已注册函数进行调用
    //for (const auto& cb : i->second) {
    //    // 取消息内容，传入回调
    //    cb(hstring(ms.substr(10)));
    //}
//}

//winrt::hstring implementation::UdpClientStrt::RegisterCast(winrt::hstring token, Windows::Foundation::IAsyncAction callback)
//{
//    auto i = registry.find(token);

//    // token存在，将函数附加到列表尾
//    if (i != registry.end()) 
//        //i->second.push_back(callback);
//    
//    // token不存在，初始化列表
//    //registry[token] = { callback };
//    return token;
//}

#pragma endregion

    UdpClientStrt UdpClient = UdpClientStrt();

}