#include "pch.h"
#include "UserDataViewModel.h"
#include "UserDataViewModel.g.cpp"

namespace winrt::BiBi::implementation
{
    UserDataViewModel::UserDataViewModel() {
        m_userList = winrt::multi_threaded_observable_vector<BiBi::UserData>();
    }
    BiBi::UserData UserDataViewModel::UserData()
    {
        return m_userData;
    }
    Windows::Foundation::Collections::IObservableVector<BiBi::UserData> UserDataViewModel::UserList()
    {
        return m_userList;
    }
}
