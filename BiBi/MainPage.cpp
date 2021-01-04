#include "pch.h"
#include "MainPage.h"
#include "MainPage.g.cpp"
using namespace winrt::Windows::Storage::Streams;


using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::BiBi::implementation
{
    MainPage::MainPage()
    {
        InitializeComponent();
    }

    //int MainPage::StartServer()
    //{
    //    /*try
    //    {*/
    //        OutputDebugString(L"excuted");
    //        // 收到数据报回调函数
    //        this->m_serverDatagramSocket.MessageReceived({ this, &MainPage::MessageReceived });

    //        // 开始监听
    //        this->m_serverDatagramSocket.BindServiceNameAsync(L"22229");

    //        // 加入组播组
    //        this->m_serverDatagramSocket.JoinMulticastGroup(winrt::Windows::Networking::HostName(L"229.2.2.9"));

    //        return 1;
    //    /*}
    //    catch (winrt::hresult_error const& ex)
    //    {
    //        Windows::Networking::Sockets::SocketErrorStatus webErrorStatus{ Windows::Networking::Sockets::SocketError::GetStatus(ex.to_abi()) };
    //        throw webErrorStatus != Windows::Networking::Sockets::SocketErrorStatus::Unknown ? winrt::to_hstring((int32_t)webErrorStatus) : winrt::to_hstring(ex.to_abi());
    //    }*/
    //}

    //void MainPage::MessageReceived(Windows::Networking::Sockets::DatagramSocket const&, Windows::Networking::Sockets::DatagramSocketMessageReceivedEventArgs const& args)
    //{
    //    // 读取数据
    //    DataReader dataReader{ args.GetDataReader() };

    //    winrt::hstring msgReceived{ dataReader.ReadString(dataReader.UnconsumedBufferLength()) };

    //    // 使用对话框显示收到的消息
    //    winrt::Windows::UI::Xaml::Controls::ContentDialog notify;
    //    notify.Title(box_value(L"New message"));
    //    notify.Content(box_value(msgReceived));
    //    notify.CloseButtonText(L"OK");
    //    // 显示对话框
    //    notify.ShowAsync();
    //}

    int32_t MainPage::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void MainPage::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void MainPage::ClickHandler(IInspectable const&, RoutedEventArgs const&)
    {
        // 启动Udp客户端
        //auto re = StartServer();
        UdpClient.StartServer();
        UdpClient.AnnounceAsync();
        //Announce();
    }
}
