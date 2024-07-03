#include "EpollEvent.h"

#include <string>
#include <unistd.h>
#include <cassert>
#include "Exception.h"

using namespace std;

#define MAX_EVENTS_NUM 256

namespace yuan {

CEpollEvent::CEpollEvent()
{
    m_fd = epoll_create(1);
    if (-1 == m_fd)
    {
        CNetOptErrorThrow(string("Create socket failed, info: ") + strerror(errno));
    }
}

CEpollEvent::~CEpollEvent()
{
    if (-1 != m_fd)
    {
        close(m_fd);
        m_fd = -1;
    }
}

void CEpollEvent::UpdateEvent(const ISocket::ptr& pSocket, EVENT_TYPE emType, void* pEventData)
{
    epoll_event ev;
    bzero(&ev, sizeof(ev));
    if (emType & EVENT_IN == EVENT_IN)
    {
        ev.events |= EPOLLIN;
    }

    if (emType & EVENT_OUT == EVENT_OUT)
    {
        ev.events |= EPOLLOUT;
    }

    if (m_mapDataMgr.count(pSocket->GetFd() <= 0))
    {
        m_mapDataMgr[pSocket->GetFd()] = {pSocket, pEventData};
        ev.data.ptr = &m_mapDataMgr[pSocket->GetFd()];
        if (-1 == epoll_ctl(m_fd, EPOLL_CTL_ADD, pSocket->GetFd(), &ev))
        {
            CNetOptErrorThrow(string("epoll_ctl EPOLL_CTL_ADD failed, info: ") + strerror(errno));
        }
    }
    else
    {
        m_mapDataMgr[pSocket->GetFd()] = {pSocket, pEventData};
        ev.data.ptr = &m_mapDataMgr[pSocket->GetFd()];
        if (-1 == epoll_ctl(m_fd, EPOLL_CTL_MOD, pSocket->GetFd(), &ev))
        {
            CNetOptErrorThrow(string("epoll_ctl EPOLL_CTL_MOD failed, info: ") + strerror(errno));
        }
    }
}

void CEpollEvent::RemoveEvent(const ISocket::ptr& pSocket)
{
    if (-1 == epoll_ctl(m_fd, EPOLL_CTL_DEL, pSocket->GetFd(), nullptr))
    {
        CNetOptErrorThrow(string("epoll_ctl EPOLL_CTL_DEL failed, info: ") + strerror(errno));
    }
    m_mapDataMgr.erase(pSocket->GetFd());
}

std::vector<EVENT_DATA> CEpollEvent::WaitEvent(std::uint32_t ui32Timeout)
{
    epoll_event events[MAX_EVENTS_NUM];
    auto ret = epoll_wait(m_fd, events, MAX_EVENTS_NUM, ui32Timeout);
    if (ret < 0)
    {
        CNetOptErrorThrow(string("epoll_wait failed, info: ") + strerror(errno));
    }

    std::vector<EVENT_DATA> vecEvent;
    for (int i = 0; i < ret; ++i)
    {
        EVENT_DATA data;
        EPOLL_DATA* pEpollData = static_cast<EPOLL_DATA*>(events[i].data.ptr);
        assert((pEpollData != nullptr));
        if (events[i].events & EPOLLIN == EPOLLIN)
        {
            data.type |= EVENT_IN;
        }

        if (events[i].events & EPOLLOUT == EPOLLOUT)
        {
            data.type |= EVENT_OUT;
        }
        data.pSocket = pEpollData->pSocket;
        data.pData = pEpollData->pData;
        vecEvent.emplace_back(data);
    }

    return vecEvent;
}

}