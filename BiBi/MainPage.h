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
        // 当前打开聊天窗口对方uid
        hstring current_uid;

        const winrt::Windows::Networking::HostName MulticastHost{ L"229.2.2.9" };
        const winrt::hstring Port{ L"22229" };
        // 用户ID
        winrt::hstring m_uId{L""};
        void SetUID(const winrt::hstring&);
        // 用户名
        winrt::hstring m_username{ L"" };

        
        //Protocol::MessageBuilder messageBuilder;

    public:
        winrt::hstring GetUID();

        winrt::hstring GetUsername();
        void SetUsername(winrt::hstring const& value);

#pragma region wrap
        void UpdateUserData();
        void AddUserData(Protocol::Message msg, hstring addr);
#pragma endregion


#pragma region 网络通信相关
        // 发现域内用户
        winrt::Windows::Foundation::IAsyncAction FindPeer();
        // 发送消息
        Windows::Foundation::IAsyncAction SendMessage(winrt::hstring hostname, winrt::hstring content);
        // 发送群组消息
        Windows::Foundation::IAsyncAction SendGroupMessage(winrt::hstring const& uid, winrt::hstring const& hostId, winrt::hstring const groupName, winrt::hstring const& content);;
        // 群组邀请
        Windows::Foundation::IAsyncAction GroupInvite(winrt::hstring const& hostname, winrt::hstring const& groupNum, std::vector<winrt::hstring> const& users);
#pragma endregion

        // 载入聊天记录
        Windows::Foundation::IAsyncAction LoadHistory(const winrt::hstring& uid);
        // 已读消息
        Windows::Foundation::IAsyncAction readMessage(hstring uid);
        // 保存历史记录
        Windows::Foundation::IAsyncAction saveHistory(hstring uid);

        // 读取或初始化设备标识
        // 用于区分用户
        Windows::Foundation::IAsyncAction DeviceInitOrRead();

#pragma region 事件绑定处理
        void ClickHandler(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args);

        // 消息处理函数
        Windows::Foundation::IAsyncAction MessageReceived(Windows::Networking::Sockets::DatagramSocket const& /* sender */, Windows::Networking::Sockets::DatagramSocketMessageReceivedEventArgs const& args);
        
        // 页面加载事件
        void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs const& args);
        // 页面卸载事件
        void OnNavigatingFrom(Windows::UI::Xaml::Navigation::NavigatingCancelEventArgs const& args);
        //void PageUnloadedHandler(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::Navigation::NavigatingCancelEventArgs const& args);
        void Send_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        void Chat_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
#pragma endregion
    };
}

namespace winrt::BiBi::factory_implementation
{
    struct MainPage : MainPageT<MainPage, implementation::MainPage>
    {
    };
}
