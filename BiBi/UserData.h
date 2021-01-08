#pragma once
#include "UserData.g.h"

namespace winrt::BiBi::implementation
{
    struct UserData : UserDataT<UserData>
    {
        UserData() = default;
        UserData(hstring userId, hstring username, hstring addr, hstring avatar, bool online);
        hstring UserId();
        void UserId(hstring const& value);
        hstring Username();
        void Username(hstring const& value);
        hstring Addr();
        void Addr(hstring const& value);
        hstring Avatar();
        void Avatar(hstring const& value);
        bool Online();
        void Online(bool value);
        winrt::event_token PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(winrt::event_token const& token) noexcept;

    private:
        hstring m_userId;
        hstring m_username;
        hstring m_addr;
        hstring m_avatar;
        bool m_online;

        winrt::event<Windows::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;
    };
}
namespace winrt::BiBi::factory_implementation
{
    struct UserData : UserDataT<UserData, implementation::UserData>
    {
    };
}
