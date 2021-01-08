#pragma once
#include "UserDataViewModel.g.h"
#include "UserData.h"

namespace winrt::BiBi::implementation
{
    struct UserDataViewModel : UserDataViewModelT<UserDataViewModel>
    {
        UserDataViewModel();

        BiBi::UserData UserData();
        Windows::Foundation::Collections::IObservableVector<BiBi::UserData> UserList();

    private:
        BiBi::UserData m_userData;
        Windows::Foundation::Collections::IObservableVector<BiBi::UserData> m_userList;
    };
}
namespace winrt::BiBi::factory_implementation
{
    struct UserDataViewModel : UserDataViewModelT<UserDataViewModel, implementation::UserDataViewModel>
    {
    };
}
