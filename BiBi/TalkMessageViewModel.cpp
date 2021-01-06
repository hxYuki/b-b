#include "pch.h"
#include "TalkMessageViewModel.h"
#include "TalkMessageViewModel.g.cpp"

namespace winrt::BiBi::implementation
{
    TalkMessageViewModel::TalkMessageViewModel()
    {
        m_talkMessage = winrt::make<BiBi::implementation::TalkMessage>();
        m_talkHistory = winrt::single_threaded_observable_vector<BiBi::TalkMessage>();
    }
    BiBi::TalkMessage TalkMessageViewModel::TalkMessage()
    {
        return m_talkMessage;
    }
    Windows::Foundation::Collections::IObservableVector<BiBi::TalkMessage> TalkMessageViewModel::TalkHistory()
    {
        return m_talkHistory;
    }
}
