#include "EventLoop.h"
#include "EpollEvent.h"

using namespace std;

namespace yuan {

CEventLoop::CEventLoop()
    : m_poller(make_shared<CEpollEvent>())
{

}

IEventPoller::ptr CEventLoop::GetEventPoller()
{
    return m_poller;
}

void CEventLoop::AddChannel(CChannel::ptr pChannel)
{
    m_channelMgr.insert(pChannel);
}

void CEventLoop::RemoveChannel(CChannel::ptr pChannel)
{
    m_channelMgr.erase(pChannel);
}

size_t CEventLoop::GetChannelSize()
{
    return m_channelMgr.size();
}

void CEventLoop::Start()
{
    while (true)
    {
        auto evObjs = m_poller->WaitEvent(1000);
        for (const auto& evObj: evObjs)
        {
            auto ch = __GetChBySocket(evObj.pSocket);
            if (nullptr != ch)
            {
                ch->HandleEvent(evObj.stData);
            }
        }
    }
}

CChannel::ptr CEventLoop::__GetChBySocket(ISocket::ptr pSocket)
{
    for (const auto& ch : m_channelMgr)
    {
        if (ch->GetSocket() == pSocket)
        {
            return ch;
        }
    }

    return nullptr;
}

}