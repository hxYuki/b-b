#include "pch.h"
#include "UserDataViewModel.h"
#include "UserDataViewModel.g.cpp"

namespace winrt::BiBi::implementation
{
    UserDataViewModel::UserDataViewModel() {
        m_userList = winrt::single_threaded_observable_map<hstring,BiBi::UserData>();
    }
    BiBi::UserData UserDataViewModel::UserData()
    {
        return m_userData;
    }
    Windows::Foundation::Collections::IObservableMap<hstring, BiBi::UserData> UserDataViewModel::UserList()
    {
        return m_userList;
    }
}
