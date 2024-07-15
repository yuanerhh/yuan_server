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

void CEventLoop::AddChannel(CConnector::ptr pConnector)
{
    m_connectorMgr.insert(pConnector);
}

void CEventLoop::RemoveChannel(CConnector::ptr pConnector)
{
    m_connectorMgr.erase(pConnector);
}

size_t CEventLoop::GetConnectSize()
{
    return m_connectorMgr.size();
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
    for (const auto& item : m_connectorMgr)
    {
        if (item->GetSocket() == pSocket)
        {
            return item->GetChannel();
        }
    }

    return nullptr;
}

}