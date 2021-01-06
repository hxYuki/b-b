#pragma once

#include "TalkMessageViewModel.h"
#include "MainPage.g.h"
#include "Network.h"
namespace winrt::BiBi::implementation
{
    struct MainPage : MainPageT<MainPage>
    {
        MainPage();
        BiBi::TalkMessageViewModel TalkMessageVM();

    private:
        BiBi::TalkMessageViewModel m_talkMessageVM;
        //using TalkMessage = std::pair<winrt::hstring, winrt::hstring>;
        //using TalkHistory = std::vector<decltype(box_value(TalkMessage{}))> ;

        //decltype(single_threaded_observable_vector(std::move(TalkHistory{}))) currentHistory = single_threaded_observable_vector(std::move(TalkHistory{}));
        // 用户ID
        winrt::hstring uId{L""};
        void SetUID(const winrt::hstring&);

        // 测试聊天记录

    public:
        const winrt::hstring& GetUID();


        // 载入聊天记录
        void LoadHistory(const winrt::hstring& uid);

        // 读取或初始化设备标识
        // 用于区分用户
        Windows::Foundation::IAsyncAction DeviceInitOrRead();



        void ClickHandler(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args);

        // 消息处理函数
        void MessageReceived(Windows::Networking::Sockets::DatagramSocket const& /* sender */, Windows::Networking::Sockets::DatagramSocketMessageReceivedEventArgs const& args);
    };
}

namespace winrt::BiBi::factory_implementation
{
    struct MainPage : MainPageT<MainPage, implementation::MainPage>
    {
    };
}
