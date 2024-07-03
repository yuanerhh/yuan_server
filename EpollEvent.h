#pragma once

#include <vector>
#include <sys/epoll.h>
#include <map>
#include "IEventPoller.h"
#include "ISocket.h"

namespace yuan {

class CEpollEvent : public IEventPoller
{
    struct EPOLL_DATA
    {
        ISocket::ptr pSocket = nullptr;
        void* pData = nullptr;
    };

public:
    CEpollEvent();
    ~CEpollEvent(){}
    virtual void UpdateEvent(const ISocket::ptr& pSocket, EVENT_TYPE emType, void* pEventData);
    virtual void RemoveEvent(const ISocket::ptr& pSocket);
    virtual std::vector<EVENT_DATA> WaitEvent(std::uint32_t ui32Timeout);

private:
    int m_fd = -1;
    std::map<int, EPOLL_DATA> m_mapDataMgr; // key: fd
};


}
