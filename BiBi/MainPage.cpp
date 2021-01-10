#include "pch.h"
#include "MainPage.h"
#include "MainPage.g.cpp"

using namespace winrt::Windows::Storage;
using namespace winrt::Windows::Storage::Streams;


using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::BiBi::implementation
{
	hstring special_char = L"␔",special_char2=L"₱";
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


	// 加载历史记录
	Windows::Foundation::IAsyncAction MainPage::LoadHistory(const winrt::hstring& uid)
	{
		/*const std::array<BiBi::TalkMessage, 2> ah{ make<BiBi::implementation::TalkMessage>(L"A",L"Hello!"),
			make<BiBi::implementation::TalkMessage>(L"B",L"Bye.") };
		array_view his(ah);
		TalkMessageVM().TalkHistory().ReplaceAll(his);*/
		// 应用数据目录
		StorageFolder dataFolder = ApplicationData::Current().LocalFolder();
		auto f = (co_await dataFolder.TryGetItemAsync(L"history\\" + uid + L".his")).as<StorageFile>();
		if (f != nullptr) {
			// 从文件中读取已有历史记录
			auto res = co_await FileIO::ReadTextAsync(f);
			// 此时需要将历史记录里的添加到需要显示的列表
			auto file_history = std::wstring_view(res);
			// 先分割每条消息，含uid+content
			// 去掉开头的符号
			file_history = file_history.substr(1, file_history.length() - 1);
			// 找下一个符号位置
			std::size_t found = file_history.find_first_of(special_char);
			// 重复此过程
			while (found != std::string::npos) {
				// 再分割uid和content
				//.  1  . 2
				//0 1 2 3
				std::size_t found2 = file_history.find_first_of(special_char2);
				hstring _uid (file_history.substr(0, found2)),
					_content(file_history.substr(found2 + 1, found - found2 - 1));

				// 传入history
				TalkMessageVM().TalkHistory().Append(make<BiBi::implementation::TalkMessage>(_uid,_content ));
				// 去掉已提取部分+符号
				file_history = file_history.substr(found + 1, file_history.length() - found - 1);
				found = file_history.find_first_of(special_char);
			}
		}
	}


	// 已读消息
	void MainPage::readMessage(hstring uid)
	{
		// 提取uid的未读消息
		std::vector<TalkMessage> tm;
		for (int i = 0; i < m_unreadMessage.size();i++) {
			if (m_unreadMessage[i].UID() == uid) {
				// 提取未读消息
				tm.emplace_back(m_unreadMessage[i].UID(), m_unreadMessage[i].Content());
				// 删除已加载未读消息
				m_unreadMessage.erase(m_unreadMessage.begin()+i);
				i--;
			}
		}
		//// 将uid的消息编码成字符串
		////编码后是这个样子        ␔uid₱Content1␔uid₱Content2␔uid₱Content3
		//hstring uid_show_data;
		//for (int i = 0; i < tm.size(); i++) {
		//	uid_show_data = special_char + uid + special_char2 + tm[i].Content();
		//}
		if (current_uid == L"") {
			// 先保存当前历史记录
			saveHistory(current_uid);
			// 清除当前窗口
			TalkMessageVM().TalkHistory().Clear();
		}
		// 加载历史记录
		LoadHistory(uid);
		// 修改当前打开窗口
		current_uid = uid;
		for (int i = 0; i < tm.size(); i++) {
			TalkMessageVM().TalkHistory().Append(make<BiBi::implementation::TalkMessage>
				(	tm[i].UID()	,	tm[i].Content()		)
			);
		}
	}
	
	
	// 保存历史记录  就是将 TalkMessageVM().TalkHistory() 存入文件
	Windows::Foundation::IAsyncAction MainPage::saveHistory(hstring uid)
	{
		hstring talk_history;
		for (int i = 0; i < TalkMessageVM().TalkHistory().Size(); i++) {
			talk_history = special_char + TalkMessageVM().TalkHistory().GetAt(i).UID() +
				special_char2 + TalkMessageVM().TalkHistory().GetAt(i).Content();
		}
		// 应用数据目录
		StorageFolder dataFolder = ApplicationData::Current().LocalFolder();
		auto f = (co_await dataFolder.TryGetItemAsync(L"history\\" + uid + L".his")).as<StorageFile>();
		if (f == nullptr) {
			// 文件中没有历史记录
			// 创建文件
			f = co_await dataFolder.CreateFileAsync(L"history\\" + uid + L".his");
		}
		//else
		//{
		//	// 从文件中读取已有历史记录
		//	auto res = co_await FileIO::ReadTextAsync(f);
		//	// 已有历史记录和新未读消息拼接
		//	talk_history = res + talk_history;
		//}
		// 保存uid的历史记录
		co_await Windows::Storage::FileIO::WriteTextAsync(f, talk_history);
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
