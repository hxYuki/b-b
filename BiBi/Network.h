#pragma once
#include<pch.h>
namespace winrt::BiBi::implementation {
	namespace Protocol {
		namespace Tokens {
			// 协议标识常量
			const winrt::hstring OnlineAnnouncement{ L"ALOHA" };
			const winrt::hstring OfflineAnnouncement{ L"ADDIO" };
			const winrt::hstring MessageSend{ L"MESSG" };
			const winrt::hstring CallMake{ L"ACALL" };

		}

		// 协议标识枚举
		enum class MessageType {
			ErrorType = -1, Online, Offline, MessageSend, CallMake
		};
		
		// 消息
		struct Message
		{
			// 发送人ID
			winrt::hstring const& uid;
			// 发送人用户名
			winrt::hstring const& username;
			// 消息类型
			MessageType const& type;
			// 消息内容
			winrt::hstring const& content;
		};

		// 消息构造器
		struct MessageBuilder {

			MessageBuilder(winrt::hstring const& uid, winrt::hstring const& username);
			
			// 将内容打包为消息，写入输出流
			winrt::Windows::Foundation::IAsyncOperation<bool> SendToStream(winrt::Windows::Storage::Streams::IOutputStream& outputStream, MessageType type, winrt::hstring const& content);

			// 将接收信息解析为消息
			static Message ReadFrom(Windows::Storage::Streams::DataReader& source);
		private:
			winrt::hstring const& uid;
			winrt::hstring const& username;

			static winrt::hstring type2token(MessageType type);
			static MessageType token2type(winrt::hstring const& token);
		};
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