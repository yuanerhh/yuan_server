#pragma once

#include <vector>
#include <sys/epoll.h>
#include "IEventListen.h"
#include "ISocket.h"

namespace yuan {

class CEpollEvent : public IEventListen
{
public:
    CEpollEvent();
    ~CEpollEvent(){}
    void AddEvent(const ISocket::ptr& pSocket, EVENT_TYPE emType);
    void RemoveEvent(const ISocket::ptr& pSocket);
    void ModifyEvent(const ISocket::ptr& pSocket, EVENT_TYPE emType);
    std::vector<std::pair<ISocket::ptr, EVENT_TYPE>> WaitEvent(std::uint32_t ui32Timeout);

private:
    int m_fd = -1;
};


}
