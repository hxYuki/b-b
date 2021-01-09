#include "pch.h"
#include "TalkMessage.h"
#include "TalkMessage.g.cpp"


namespace winrt::BiBi::implementation
{
    BiBi::implementation::TalkMessage::TalkMessage() {}
    TalkMessage::TalkMessage(const TalkMessage& value) : m_username{ value.m_username }, m_content{value.m_content }
    {

    }
    BiBi::implementation::TalkMessage::TalkMessage(winrt::hstring const& username, winrt::hstring const& content) : m_username{username}, m_content{content}
    {
    }

    hstring TalkMessage::Username()
    {
        return m_username;
    }
    void TalkMessage::Username(hstring const& value)
    {
        if (m_username != value)
        {
            m_username = value;
            m_propertyChanged(*this, Windows::UI::Xaml::Data::PropertyChangedEventArgs{ L"Username" });
        }
    }
    hstring TalkMessage::Content()
    {
        return m_content;
    }
    void TalkMessage::Content(hstring const& value)
    {
        if (m_content != value)
        {
            m_content = value;
            m_propertyChanged(*this, Windows::UI::Xaml::Data::PropertyChangedEventArgs{ L"Content" });
        }
    }
    winrt::event_token TalkMessage::PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
    {
        return m_propertyChanged.add(handler);
    }
    void TalkMessage::PropertyChanged(winrt::event_token const& token) noexcept
    {
        m_propertyChanged.remove(token);
    }
}
