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
	// 发送消息
	Windows::Foundation::IAsyncAction MainPage::SendMessage(winrt::hstring hostname,winrt::hstring content)
	{
		Windows::Networking::HostName host{ hostname };
		auto out = co_await WorkerClient.GetTargetStream(host, Port);
		auto debug = GetUID();
		winrt::BiBi::implementation::Protocol::MessageBuilder mb(debug, L"");

		co_await mb.SendToStream(out, Protocol::MessageType::MessageSend,content);
	}
	// 加载历史记录
	void MainPage::LoadHistory(const winrt::hstring& uid)
	{
		const std::array<BiBi::TalkMessage, 2> ah{ make<BiBi::implementation::TalkMessage>(L"A",L"Hello!"),
			make<BiBi::implementation::TalkMessage>(L"B",L"Bye.") };
		array_view his(ah);
		TalkMessageVM().TalkHistory().ReplaceAll(his);
	}
	// 已读消息
	Windows::Foundation::IAsyncAction MainPage::readMessage(hstring uid)
	{
		// 提取uid的未读消息
		std::vector<TalkMessage> tm;
		for (int i = 0; i < m_unreadMessage.size();i++) {
			if (m_unreadMessage[i].Username() == uid) {
				// 提取未读消息
				tm.emplace_back(m_unreadMessage[i].Username(), m_unreadMessage[i].Content());
				// 删除已加载未读消息
				m_unreadMessage.erase(m_unreadMessage.begin()+i);
				i--;
			}
		}
		// 将uid的消息编码成字符串
		//编码后是这个样子        ␔Content1␔Content2␔Content3␔Content4
		hstring uid_show_data;
		for (int i = 0; i < tm.size(); i++) {
			uid_show_data =L"␔"+ tm[i].Content();
		}
		// 应用数据目录
		StorageFolder dataFolder = ApplicationData::Current().LocalFolder();

		// 判断uid的历史记录是否存在
		auto f = (co_await dataFolder.TryGetItemAsync(L"history\\"+uid+L".his")).as<StorageFile>();
		if (f == nullptr) {
			// 文件中没有历史记录
			// 创建文件
			auto nf = co_await dataFolder.CreateFileAsync(L"history\\" + uid + L".his");
			// 保存uid的历史记录
			co_await Windows::Storage::FileIO::WriteTextAsync(nf, uid_show_data);
		}
		else
		{
			// 从文件中读取已有历史记录
			auto res = co_await FileIO::ReadTextAsync(f);
			// 已有历史记录和新未读消息拼接
			uid_show_data =  res + uid_show_data;
			// 保存uid的历史记录
			co_await Windows::Storage::FileIO::WriteTextAsync(f, uid_show_data);
		}
		// 显示消息代码  待写 在 uid_show_data 里
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
		default:
			break;
		}
		//OutputDebugString(msgReceived.c_str());
		//OutputDebugString(L"\n");
	}
}
