#include "pch.h"
#include "UserData.h"
#include "UserData.g.cpp"


namespace winrt::BiBi::implementation
{
    hstring UserData::UserId()
    {
        return m_userId;
    }
    void UserData::UserId(hstring const& value)
    {
        if (m_userId != value)
        {
            m_userId = value;
            m_propertyChanged(*this, Windows::UI::Xaml::Data::PropertyChangedEventArgs{ L"UserId" });
        }
    }
    hstring UserData::Username()
    {
        return m_username;
    }
    void UserData::Username(hstring const& value)
    {
        if (m_username != value)
        {
            m_username = value;
            m_propertyChanged(*this, Windows::UI::Xaml::Data::PropertyChangedEventArgs{ L"Username" });
        }
    }
    hstring UserData::Addr()
    {
        return m_addr;
    }
    void UserData::Addr(hstring const& value)
    {
        if (m_addr != value)
        {
            m_addr = value;
            m_propertyChanged(*this, Windows::UI::Xaml::Data::PropertyChangedEventArgs{ L"Addr" });
        }
    }
    hstring UserData::Avatar()
    {
        return m_avatar;
    }
    void UserData::Avatar(hstring const& value)
    {
        if (m_avatar != value)
        {
            m_avatar = value;
            m_propertyChanged(*this, Windows::UI::Xaml::Data::PropertyChangedEventArgs{ L"Avatar" });
        }
    }
    winrt::event_token UserData::PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
    {
        return m_propertyChanged.add(handler);
    }
    void UserData::PropertyChanged(winrt::event_token const& token) noexcept
    {
        m_propertyChanged.remove(token);
    }
}
