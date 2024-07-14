#pragma once

#include <set>
#include <memory>
#include "IEventPoller.h"
#include "Channel.h"

namespace yuan {

class CEventLoop
{
public:
    CEventLoop();
    ~CEventLoop() {}

    IEventPoller::ptr GetEventPoller();
    void AddChannel(CChannel::ptr pChannel);
    void RemoveChannel(CChannel::ptr pChannel);
    size_t GetChannelSize();
    void Start();

private:
    CChannel::ptr __GetChBySocket(ISocket::ptr pSocket);

private:
    std::set<CChannel::ptr> m_channelMgr;
    IEventPoller::ptr m_poller;
};


}