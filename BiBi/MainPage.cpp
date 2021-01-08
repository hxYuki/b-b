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

#pragma region Deprecated Code
	//int MainPage::StartServer()
	//{
	//    /*try
	//    {*/
	//        OutputDebugString(L"excuted");
	//        // 收到数据报回调函数
	//        this->m_serverDatagramSocket.MessageReceived({ this, &MainPage::MessageReceived });

	//        // 开始监听
	//        this->m_serverDatagramSocket.BindServiceNameAsync(L"22229");

	//        // 加入组播组
	//        this->m_serverDatagramSocket.JoinMulticastGroup(winrt::Windows::Networking::HostName(L"229.2.2.9"));

	//        return 1;
	//    /*}
	//    catch (winrt::hresult_error const& ex)
	//    {
	//        Windows::Networking::Sockets::SocketErrorStatus webErrorStatus{ Windows::Networking::Sockets::SocketError::GetStatus(ex.to_abi()) };
	//        throw webErrorStatus != Windows::Networking::Sockets::SocketErrorStatus::Unknown ? winrt::to_hstring((int32_t)webErrorStatus) : winrt::to_hstring(ex.to_abi());
	//    }*/
	//}

	//void MainPage::MessageReceived(Windows::Networking::Sockets::DatagramSocket const&, Windows::Networking::Sockets::DatagramSocketMessageReceivedEventArgs const& args)
	//{
	//    // 读取数据
	//    DataReader dataReader{ args.GetDataReader() };

	//    winrt::hstring msgReceived{ dataReader.ReadString(dataReader.UnconsumedBufferLength()) };

	//    // 使用对话框显示收到的消息
	//    winrt::Windows::UI::Xaml::Controls::ContentDialog notify;
	//    notify.Title(box_value(L"New message"));
	//    notify.Content(box_value(msgReceived));
	//    notify.CloseButtonText(L"OK");
	//    // 显示对话框
	//    notify.ShowAsync();
	//}

#pragma endregion


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
		auto debug = GetUID();
		winrt::BiBi::implementation::Protocol::MessageBuilder mb(debug, L"");
		OutputDebugString(L"received: \n");
		
		/*DataReader dataReader{ args.GetDataReader() };
		winrt::hstring msgReceived{ dataReader.ReadString(dataReader.UnconsumedBufferLength()) };*/
		auto msg = Protocol::MessageBuilder::ReadFrom(args.GetDataReader());
		if (msg.uid == debug)
			co_return;
		switch (msg.type)
		{
		case Protocol::MessageType::Online:
			// 收到其他用户上线消息
			if (msg.content == Protocol::Kinds::PeerSeeking) {
				// 添加至列表
				//auto d = UserData();
				/*d.UserId(msg.uid);
				d.Username(msg.username);
				d.Addr(args.RemoteAddress().ToString());*/
				//d.Online(true);
				//auto td = winrt::;
				/*UserDataVM().UserList()*/
				UserDataVM().UserList().Append(make<BiBi::implementation::UserData>(msg.uid, msg.username, args.RemoteAddress().ToString(), L"", true));

				// 发送问候
				auto out = co_await WorkerClient.GetTargetStream(args.RemoteAddress(), Port);
				mb.SendToStream(out, Protocol::MessageType::Online, Protocol::Kinds::PeerGreeting);
			}

			// 收到问候
			else if (msg.content == Protocol::Kinds::PeerGreeting) {
				// 添加至列表
				//auto d = UserData();
				/*d.UserId(msg.uid);
				d.Username(msg.username);
				d.Addr(args.RemoteAddress().ToString());*/
				//d.Online(true);
				//auto t = UserData(d);
				UserDataVM().UserList().Append(make<BiBi::implementation::UserData>(msg.uid, msg.username, args.RemoteAddress().ToString(), L"", true));

				//UserDataVM().UserList().Append(make<winrt::BiBi::UserData>(d));
				//UserDataVM().UserList().Insert(msg.uid, make<UserData>(d));

			}
			break;

		default:
			break;
		}
		//OutputDebugString(msgReceived.c_str());
		//OutputDebugString(L"\n");
	}
}
