#include<pch.h>
#include"Network.h"
#include <winrt/Windows.UI.Composition.h>
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Networking::Sockets;
using namespace winrt::Windows::Storage::Streams;

namespace winrt::BiBi::implementation {

    
    void Networker::StartServer()
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

    Windows::Foundation::IAsyncOperation<Windows::Storage::Streams::IOutputStream> Networker::GetTargetStream(Windows::Networking::HostName const& host, winrt::hstring const& port)
    {
        return m_serverDatagramSocket.GetOutputStreamAsync(host, port);
    }

    
    IAsyncAction Networker::AnnounceAsync(const winrt::hstring& uid)
    {
        try
        {
            Windows::Networking::HostName multicastHostName{ L"229.2.2.9" };
            // 获取输出流
            IOutputStream outputStream = co_await m_serverDatagramSocket.GetOutputStreamAsync(multicastHostName, L"22229");
            
            // 发送
            Protocol::MessageBuilder mb(uid, L"");
            if (co_await mb.SendToStream(outputStream, Protocol::MessageType::Online, L"imhere")) {
                // 处理结果
            }
            //winrt::hstring msg{ L"b&b:"+Protocol::Tokens::OnlineAnnouncement+L":["+uid+L"]imhere" };
            //// 向流中写入数据
            //DataWriter dataWriter{ outputStream };
            //// 存在消息
            //dataWriter.WriteString(msg);
            //// 发送
            //co_await dataWriter.StoreAsync();
            //dataWriter.DetachStream();
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

    Networker WorkerClient = Networker();


    Protocol::MessageBuilder::MessageBuilder(winrt::hstring const& uid, winrt::hstring const& username) : uid{ uid }, username{ username }
    {
    }

    winrt::Windows::Foundation::IAsyncOperation<bool> Protocol::MessageBuilder::SendToStream(winrt::Windows::Storage::Streams::IOutputStream& outputStream, MessageType type, winrt::hstring const& content)
    {
        try
        {
            // 拼装消息
            winrt::hstring msg{ L"b&b:" + type2token(type) + L":[" + uid + L":" + username + L"]" + content };
            // 写入消息
            DataWriter dataWriter{ outputStream };
            dataWriter.WriteString(msg);
            // 存储发送
            co_await dataWriter.StoreAsync();
            dataWriter.DetachStream();
            // 成功
            co_return true;
        }
        catch (winrt::hresult_error const& ex)
        {
            // 失败
            co_return false;
        }

    }

    Protocol::Message Protocol::MessageBuilder::ReadFrom(Windows::Storage::Streams::DataReader& source)
    {
        // 读取数据
        winrt::hstring msgReceived{ source.ReadString(source.UnconsumedBufferLength()) };

        auto ms = std::wstring_view(msgReceived);
        // 取协议头
        if (ms.substr(0, 3) != L"b&b")
            return Message{ L"",L"",MessageType::ErrorType,L"" };
        // 取token
        auto token = ms.substr(4, 5);
        auto type = token2type(std::wstring(token).c_str());
        // token不正确
        if (type == MessageType::ErrorType)
            return Message{ L"",L"",MessageType::ErrorType,L"" };

        //Message msg;
        try
        {
            // 解析信息
            auto startPos = ms.find(L"[", 9);
            auto splitPos = ms.find(L":", 10);
            auto endPos = ms.find(L"]", splitPos);
            auto uid = ms.substr(11, splitPos - startPos - 1);
            auto uname = ms.substr(splitPos + 1, endPos - splitPos - 1);
            auto content = ms.substr(endPos + 1);
            return Message{ std::wstring(uid).c_str(),std::wstring(uname).c_str(),type,std::wstring(content).c_str() };
        }
        catch (const std::exception&)
        {
            // 信息损坏
            return Message{ L"",L"",MessageType::ErrorType,L"" };
        }
        
        //return msg;
    }

    winrt::hstring Protocol::MessageBuilder::type2token(Protocol::MessageType type)
    {
        switch (type)
        {
        case MessageType::Online:
            return Protocol::Tokens::OnlineAnnouncement;
        case MessageType::Offline:
            return Protocol::Tokens::OfflineAnnouncement;
        case MessageType::CallMake:
            return Protocol::Tokens::CallMake;
        case MessageType::MessageSend:
            return Protocol::Tokens::MessageSend;
        case MessageType::GroupMessageSend:
            return Protocol::Tokens::GroupMessageSend;
        case MessageType::GroupInvite:
            return Protocol::Tokens::GroupInvite;
        default:
            return L"Error";
        }
    }

    Protocol::MessageType Protocol::MessageBuilder::token2type(winrt::hstring const& token)
    {
        if (token == Protocol::Tokens::OnlineAnnouncement)
            return MessageType::Online;
        else if (token == Protocol::Tokens::OfflineAnnouncement)
            return MessageType::Offline;
        else if (token == Protocol::Tokens::MessageSend)
            return MessageType::MessageSend;
        else if (token == Protocol::Tokens::CallMake)
            return MessageType::CallMake;
        else if (token == Protocol::Tokens::GroupMessageSend)
            return MessageType::GroupMessageSend;
        else if (token == Protocol::Tokens::GroupInvite)
            return MessageType::GroupInvite;
        else return MessageType::ErrorType;
    }

}

