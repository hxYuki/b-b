#include "pch.h"
#include "MainPage.h"
#include "MainPage.g.cpp"

using namespace winrt::Windows::Storage;
using namespace winrt::Windows::Storage::Streams;


using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::BiBi::implementation
{
	MainPage::MainPage()
	{
		InitializeComponent();

		
		//读取或初始化ID
		DeviceInitOrRead();

		// 启动Socket
		WorkerClient.StartServer();

		// 注册处理函数
		WorkerClient.RegisterCastProcessAsync(Protocol::Tokens::OnlineAnnouncement, this, &MainPage::MessageReceived);

		// 延迟操作
		Windows::System::Threading::ThreadPoolTimer::CreateTimer([&](winrt::Windows::System::Threading::ThreadPoolTimer const& source) {
			FindPeer();

			}, std::chrono::seconds(1));
		//循环广播在线信息
		Windows::System::Threading::ThreadPoolTimer::CreatePeriodicTimer(
			[&](winrt::Windows::System::Threading::ThreadPoolTimer const& source)
				{
				//先置离线
					for (int i = 0; i < UserDataVM().UserList().Size(); i++) {
							UserDataVM().UserList().GetAt(i).Online(false);
					}
					//有回应再置在线
					FindPeer();
				}, std::chrono::seconds(10));

		// 耗时操作
		/*Windows::System::Threading::ThreadPool::RunAsync([&]() {

			});*/

		// 测试： 载入聊天记录
		LoadHistory(L"");
	}
	BiBi::TalkMessageViewModel MainPage::TalkMessageVM()
	{
		return m_talkMessageVM;
	}

	BiBi::UserDataViewModel MainPage::UserDataVM()
	{
		return m_userDataVM;
	}

	winrt::hstring MainPage::GetUID()
	{
		return m_uId;
	}

	void MainPage::SetUID(const winrt::hstring& UID)
	{
		// 不允许后续更改UID
		if (UID != L"")
			m_uId = UID;
		else OutputDebugString(L"fuck shit");

	}

	winrt::hstring MainPage::GetUsername()
	{
		return m_username;
	}

	void MainPage::SetUsername(winrt::hstring const& value)
	{
		m_username = value;
	}

	// 发送消息
	Windows::Foundation::IAsyncAction MainPage::SendMessage(winrt::hstring hostname,winrt::hstring content)
	{
		Windows::Networking::HostName host{ hostname };
		auto out = co_await WorkerClient.GetTargetStream(host, Port);
		auto uid = GetUID(), uname = GetUsername();
		winrt::BiBi::implementation::Protocol::MessageBuilder mb(uid, uname);

		co_await mb.SendToStream(out, Protocol::MessageType::MessageSend,content);
	}

	Windows::Foundation::IAsyncAction MainPage::SendGroupMessage(winrt::hstring const& targetUid, winrt::hstring const& hostId, winrt::hstring const groupName, winrt::hstring const& content) {
		winrt::hstring addr{ L"" };
		// 找到uid对应的用户地址
		for (int i = 0; i < UserDataVM().UserList().Size(); i++) {
			auto user = UserDataVM().UserList().GetAt(i);
			if (user.UserId() == targetUid && user.Addr().size()>0 && user.Online()) {
				
				addr = user.Addr();
			}
		}
		Windows::Networking::HostName hostname{ addr };
		auto out = co_await WorkerClient.GetTargetStream(hostname, Port);
		auto uid = GetUID();
		// 群组消息用户名为 群主ID-组名
		winrt::BiBi::implementation::Protocol::MessageBuilder mb(uid, hostId+L"-"+groupName);
		co_await mb.SendToStream(out, Protocol::MessageType::GroupMessageSend, content);
	}
	
	Windows::Foundation::IAsyncAction MainPage::GroupInvite(winrt::hstring const& hostname, winrt::hstring const& groupName, std::vector<winrt::hstring> const& users) {
		Windows::Networking::HostName host{ hostname };
		auto out = co_await WorkerClient.GetTargetStream(host, Port);
		auto uid = GetUID(), uname = GetUsername();
		winrt::BiBi::implementation::Protocol::MessageBuilder mb(uid, groupName);
		// 拼接用户列表
		hstring ul{ L"" };
		for (const auto& u : users) {
			ul = ul + u + L",";
		}
		co_await mb.SendToStream(out, Protocol::MessageType::GroupInvite, ul);
	}

	void MainPage::LoadHistory(const winrt::hstring& uid)
	{
		const std::array<BiBi::TalkMessage, 2> ah{ make<BiBi::implementation::TalkMessage>(L"A",L"Hello!"),
			make<BiBi::implementation::TalkMessage>(L"B",L"Bye.") };
		array_view his(ah);
		TalkMessageVM().TalkHistory().ReplaceAll(his);
	}

	Windows::Foundation::IAsyncAction MainPage::DeviceInitOrRead()
	{
		// 应用数据目录
		StorageFolder dataFolder = ApplicationData::Current().LocalFolder();

		// 判断Id是否存在
		auto f = (co_await dataFolder.TryGetItemAsync(L"uid.dat")).as<StorageFile>();
		if (f == nullptr) {
			// 读取设备应用ID并转换为用户ID
			std::wstringstream wss;
			wss << std::setfill(L'0') << std::setw(8) << std::hex << Windows::Security::ExchangeActiveSyncProvisioning::EasClientDeviceInformation().Id().Data1;
			auto guid = wss.str();

			/*OutputDebugString(L"Init: ");
			OutputDebugString(guid.c_str());
			OutputDebugString(L"\n");*/

			// 保存UID
			auto nf = co_await dataFolder.CreateFileAsync(L"uid.dat");
			
			co_await Windows::Storage::FileIO::WriteTextAsync(nf, guid);

			// 设置程序内存储
			SetUID(guid.c_str());
		}
		else
		{
			// 从文件中读取
			auto res = co_await FileIO::ReadTextAsync(f);

			/*OutputDebugString(L"Read: ");
			OutputDebugString(res.c_str()); 
			OutputDebugString(L"\n");*/

			// 设置程序内存储
			SetUID(res.c_str());
		}
	}




	void MainPage::ClickHandler(IInspectable const&, RoutedEventArgs const&)
	{
		// 启动Udp客户端
		//auto re = StartServer();
		WorkerClient.AnnounceAsync(GetUID());
		//Announce();
	}
	winrt::Windows::Foundation::IAsyncAction MainPage::FindPeer()
	{
		auto out = co_await WorkerClient.GetTargetStream(MulticastHost, Port);
		auto debug = GetUID();
		winrt::BiBi::implementation::Protocol::MessageBuilder mb(debug, L"");

		co_await mb.SendToStream(out, Protocol::MessageType::Online, Protocol::Kinds::PeerSeeking);
	}
	winrt::Windows::Foundation::IAsyncAction MainPage::MessageReceived(Windows::Networking::Sockets::DatagramSocket const&, Windows::Networking::Sockets::DatagramSocketMessageReceivedEventArgs const& args)
	{
		auto uid = GetUID(), uname = GetUsername();
		winrt::BiBi::implementation::Protocol::MessageBuilder mb(uid, uname);
		OutputDebugString(L"received: \n");
		
		/*DataReader dataReader{ args.GetDataReader() };
		winrt::hstring msgReceived{ dataReader.ReadString(dataReader.UnconsumedBufferLength()) };*/
		auto msg = Protocol::MessageBuilder::ReadFrom(args.GetDataReader());
		if (msg.uid == uid)
			co_return;
		switch (msg.type)
		{
			case Protocol::MessageType::Online:
			// 收到其他用户上线消息
			if (msg.content == Protocol::Kinds::PeerSeeking) {
				//// 添加至列表
				//UserDataVM().UserList().Append(make<BiBi::implementation::UserData>(msg.uid, msg.username, args.RemoteAddress().ToString(), L"", true));

				// 发送问候
				auto out = co_await WorkerClient.GetTargetStream(args.RemoteAddress(), Port);
				mb.SendToStream(out, Protocol::MessageType::Online, Protocol::Kinds::PeerGreeting);
			}

			// 收到问候
			else if (msg.content == Protocol::Kinds::PeerGreeting) {
				// 添加至列表
				//遍历查找置为在线
				for (int i = 0; i < UserDataVM().UserList().Size(); i++){
					if (UserDataVM().UserList().GetAt(i).UserId() == msg.uid) {
						UserDataVM().UserList().GetAt(i).Online(true);
						//已经置在线则不用新添此用户
						co_return;
					}
				}
				//若没有找到则新增用户
				UserDataVM().UserList().Append(make<BiBi::implementation::UserData>(msg.uid, msg.username, args.RemoteAddress().ToString(), L"", true));

				//UserDataVM().UserList().Append(make<winrt::BiBi::UserData>(d));
				//UserDataVM().UserList().Insert(msg.uid, make<UserData>(d));

			}
			break;
			case Protocol::MessageType::Offline:

				break;
			case Protocol::MessageType::CallMake:
				break;
			case Protocol::MessageType::MessageSend:
				//收到消息添加到未读消息列表
				m_unreadMessage.emplace_back( msg.uid, msg.content);
				break;
			case Protocol::MessageType::GroupMessageSend:
				m_unreadMessage.emplace_back(msg.username, msg.content); // TODO: 群聊需要发送人ID字段
			case Protocol::MessageType::GroupInvite:
				/// 添加群组 
				/// 群组ID为 “创建者ID-群组名”
				/// 群组的用户名为用户列表
				/// 群组的地址栏为空
				UserDataVM().UserList().Append(make<UserData>(msg.uid + L"-"+ msg.username, msg.content, L"", L"", false));
				break;
			//case Protocol::MessageType::GroupAware:

		default:
			break;
		}
		//OutputDebugString(msgReceived.c_str());
		//OutputDebugString(L"\n");
	}
}
