#include "pch.h"
#include "UserData.h"
#include "UserData.g.cpp"


namespace winrt::BiBi::implementation
{
    /*UserData::UserData(hstring userId, hstring username, hstring addr, hstring avatar, bool online):m_userId{ userId },m_username{username}, m_addr{addr},m_avatar{avatar}, m_online{online}
    {
    }*/
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
    /*bool UserData::Online()
    {
        return m_online;
    }
    void UserData::Online(bool value)
    {
        if (m_online != value)
        {
            m_online = value;
            m_propertyChanged(*this, Windows::UI::Xaml::Data::PropertyChangedEventArgs{ L"Online" });
        }
    }*/
    winrt::event_token UserData::PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
    {
        return m_propertyChanged.add(handler);
    }
    void UserData::PropertyChanged(winrt::event_token const& token) noexcept
    {
        m_propertyChanged.remove(token);
    }
}
