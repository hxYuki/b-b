#pragma once
#include "TalkMessageViewModel.g.h"
#include "TalkMessage.h"

namespace winrt::BiBi::implementation
{
    struct TalkMessageViewModel : TalkMessageViewModelT<TalkMessageViewModel>
    {
        TalkMessageViewModel();

        BiBi::TalkMessage TalkMessage();
        Windows::Foundation::Collections::IObservableVector<BiBi::TalkMessage> TalkHistory();
    private:
        BiBi::TalkMessage m_talkMessage{ nullptr };
        Windows::Foundation::Collections::IObservableVector<BiBi::TalkMessage> m_talkHistory;
    };
}
namespace winrt::BiBi::factory_implementation
{
    struct TalkMessageViewModel : TalkMessageViewModelT<TalkMessageViewModel, implementation::TalkMessageViewModel>
    {
    };
}
