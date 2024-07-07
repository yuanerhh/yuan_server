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

void CEventLoop::Start()
{

}

}