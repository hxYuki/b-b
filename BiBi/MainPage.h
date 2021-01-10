#pragma once

#include "TalkMessageViewModel.h"
#include "UserDataViewModel.h"
#include "MainPage.g.h"
#include "Network.h"
namespace winrt::BiBi::implementation
{
    struct MainPage : MainPageT<MainPage>
    {
        MainPage();
        BiBi::TalkMessageViewModel TalkMessageVM();
        BiBi::UserDataViewModel UserDataVM();

    private:
        BiBi::TalkMessageViewModel m_talkMessageVM;
        BiBi::UserDataViewModel m_userDataVM;
        // 未读消息列表
        std::vector<TalkMessage> m_unreadMessage;

        const winrt::Windows::Networking::HostName MulticastHost{ L"229.2.2.9" };
        const winrt::hstring Port{ L"22229" };
        // 用户ID
        winrt::hstring m_uId{L""};
        void SetUID(const winrt::hstring&);

        
        //Protocol::MessageBuilder messageBuilder;

    public:
        winrt::hstring GetUID();


        // 发送消息
        Windows::Foundation::IAsyncAction SendMessage(winrt::hstring hostname, winrt::hstring content);
        // 载入聊天记录
        void LoadHistory(const winrt::hstring& uid);

        // 读取或初始化设备标识
        // 用于区分用户
        Windows::Foundation::IAsyncAction DeviceInitOrRead();

        // 发现域内用户
        winrt::Windows::Foundation::IAsyncAction FindPeer();

        //void ClickHandler(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args);

        // 消息处理函数
        Windows::Foundation::IAsyncAction MessageReceived(Windows::Networking::Sockets::DatagramSocket const& /* sender */, Windows::Networking::Sockets::DatagramSocketMessageReceivedEventArgs const& args);
        void Send_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void Call_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
    };
}

namespace winrt::BiBi::factory_implementation
{
    struct MainPage : MainPageT<MainPage, implementation::MainPage>
    {
    };
}
