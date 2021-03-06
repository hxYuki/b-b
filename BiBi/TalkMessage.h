﻿#pragma once
#include "TalkMessage.g.h"


namespace winrt::BiBi::implementation
{
    struct TalkMessage : TalkMessageT<TalkMessage>
    {
        TalkMessage();
        TalkMessage(const TalkMessage&);
        TalkMessage(winrt::hstring const& uid, winrt::hstring const& content);

        winrt::hstring UID();
        void UID(winrt::hstring const& value);
        winrt::hstring Content();
        void Content(winrt::hstring const& value);
        winrt::event_token PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(winrt::event_token const& token) noexcept;
        winrt::BiBi::implementation::TalkMessage& winrt::BiBi::implementation::TalkMessage::operator =(const winrt::BiBi::implementation::TalkMessage & c) {
            m_uID = c.m_uID;
            m_content = c.m_content;
            return *this;
        }

    private:
        winrt::hstring m_uID;
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
