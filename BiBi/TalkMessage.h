#pragma once
#include "TalkMessage.g.h"


namespace winrt::BiBi::implementation
{
    struct TalkMessage : TalkMessageT<TalkMessage>
    {
        TalkMessage() = default;
        TalkMessage(winrt::hstring const& username, winrt::hstring const& content);

        winrt::hstring Username();
        void Username(winrt::hstring const& value);
        winrt::hstring Content();
        void Content(winrt::hstring const& value);
        winrt::event_token PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(winrt::event_token const& token) noexcept;

    private:
        winrt::hstring m_username;
        winrt::hstring m_content;
        winrt::event<Windows::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;
    };
}
namespace winrt::BiBi::factory_implementation
{
    struct TalkMessage : TalkMessageT<TalkMessage, implementation::TalkMessage>
    {
    };
}
