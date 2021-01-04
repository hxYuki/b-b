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
    };
}

namespace winrt::BiBi::factory_implementation
{
    struct MainPage : MainPageT<MainPage, implementation::MainPage>
    {
    };
}
