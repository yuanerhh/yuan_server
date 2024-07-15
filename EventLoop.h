#pragma once

#include <set>
#include <memory>
#include "IEventPoller.h"
#include "Connector.h"

namespace yuan {

class CEventLoop
{
public:
    CEventLoop();
    ~CEventLoop() {}

    IEventPoller::ptr GetEventPoller();
    void AddChannel(CConnector::ptr pConnector);
    void RemoveChannel(CConnector::ptr pConnector);
    size_t GetConnectSize();
    void Start();

private:
    CChannel::ptr __GetChBySocket(ISocket::ptr pSocket);

private:
    std::set<CConnector::ptr> m_connectorMgr;
    IEventPoller::ptr m_poller;
};


}