#pragma once

#include "MainPage.g.h"
#include "Network.h"
namespace winrt::BiBi::implementation
{
    struct MainPage : MainPageT<MainPage>
    {
        MainPage();

        

        int32_t MyProperty();
        void MyProperty(int32_t value);

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
