#pragma once
#include<pch.h>
namespace winrt::BiBi::implementation {
	struct UdpClientStrt {
	private:
		Windows::Networking::Sockets::DatagramSocket m_serverDatagramSocket;

	public:
		void StartServer();
		winrt::Windows::Foundation::IAsyncAction AnnounceAsync();
		winrt::Windows::Foundation::IAsyncAction MessageReceived(Windows::Networking::Sockets::DatagramSocket const& /* sender */, Windows::Networking::Sockets::DatagramSocketMessageReceivedEventArgs const& args);
	};
	extern UdpClientStrt UdpClient;

}