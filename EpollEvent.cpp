#include "EpollEvent.h"

#include <string>
#include <unistd.h>
#include "Exception.h"

using namespace std;

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

void CEpollEvent::AddEvent(const ISocket::ptr& pSocket, EVENT_TYPE emType)
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

    ev.data.fd = pSocket->GetFd();
    if (-1 == epoll_ctl(m_fd, EPOLL_CTL_ADD, pSocket->GetFd(), &ev))
    {
        CNetOptErrorThrow(string("epoll_ctl EPOLL_CTL_ADD failed, info: ") + strerror(errno));
    }
}

void CEpollEvent::RemoveEvent(const ISocket::ptr& pSocket)
{
    if (-1 == epoll_ctl(m_fd, EPOLL_CTL_DEL, pSocket->GetFd(), nullptr))
    {
        CNetOptErrorThrow(string("epoll_ctl EPOLL_CTL_DEL failed, info: ") + strerror(errno));
    }
}

void CEpollEvent::ModifyEvent(const ISocket::ptr& pSocket, EVENT_TYPE emType)
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

    ev.data.fd = pSocket->GetFd();
    if (-1 == epoll_ctl(m_fd, EPOLL_CTL_MOD, pSocket->GetFd(), &ev))
    {
        CNetOptErrorThrow(string("epoll_ctl EPOLL_CTL_MOD failed, info: ") + strerror(errno));
    }
}

}