#pragma once
#include "App.xaml.g.h"

namespace winrt::BiBi::implementation
{
    struct App : AppT<App>
    {
        App();

        //Windows::Networking::Sockets::DatagramSocket m_clientDatagramSocket;
        

        void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs const&);
        void OnSuspending(IInspectable const&, Windows::ApplicationModel::SuspendingEventArgs const&);
        void OnNavigationFailed(IInspectable const&, Windows::UI::Xaml::Navigation::NavigationFailedEventArgs const&);

        //void StartServer();
        //void Announce();
        //void MessageReceived(Windows::Networking::Sockets::DatagramSocket const& /* sender */, Windows::Networking::Sockets::DatagramSocketMessageReceivedEventArgs const& args);

    };
}
