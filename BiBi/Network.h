#pragma once
#include<pch.h>
namespace winrt::BiBi::implementation {
	namespace ProtocolTokens {
		const winrt::hstring OnlineAnnouncement{ L"ALOHA" };
		const winrt::hstring OfflineAnnouncement{ L"ADDIO" };
		const winrt::hstring MessageSend{ L"MESSG" };
		const winrt::hstring CallMake{ L"ACALL" };
	}

	struct UdpClientStrt {
	private:
		// Udp Socket
		Windows::Networking::Sockets::DatagramSocket m_serverDatagramSocket;
		// 消息注册处理函数列表
		//std::unordered_map<winrt::hstring, std::vector<std::function<void(winrt::hstring)>>> registry;


		//// 消息事件分派处理函数
		//void MessageReceived(Windows::Networking::Sockets::DatagramSocket const& /* sender */, Windows::Networking::Sockets::DatagramSocketMessageReceivedEventArgs const& args);
	public:
		// 应用启动时启动Udp监听服务器
		void StartServer();
		// 组播设备存在
		winrt::Windows::Foundation::IAsyncAction AnnounceAsync(const winrt::hstring&);
		// 注册消息处理
		// 模板参数 S：用于适配各个页面struct类型
		template<typename S>
		inline winrt::hstring RegisterCastProcess(winrt::hstring token, S* that, void(S::* callback)(Windows::Networking::Sockets::DatagramSocket const& /* sender */, Windows::Networking::Sockets::DatagramSocketMessageReceivedEventArgs const& args)) {
			m_serverDatagramSocket.MessageReceived({ that, callback });
			return L"";
		}
		// 注册异步消息处理
		// 模板参数 S：用于适配各个页面struct类型
		template<typename S>
		inline winrt::hstring RegisterCastProcessAsync(winrt::hstring token, S* that, Windows::Foundation::IAsyncAction(S::* callback)(Windows::Networking::Sockets::DatagramSocket const& /* sender */, Windows::Networking::Sockets::DatagramSocketMessageReceivedEventArgs const& args)) {
			m_serverDatagramSocket.MessageReceived({ that, callback });
			return L"";
		}
	};

	// 全局公用Socket
	extern UdpClientStrt UdpClient;

	

}