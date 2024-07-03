#include "Channel.h"

namespace yuan {

CChannel::CChannel(IEventPoller::ptr poller, ISocket::ptr pSocket)
    : m_poller(poller), m_pSocket(pSocket)
{

}

CChannel::~CChannel()
{

}

ISocket::ptr CChannel::GetSocket()
{
    return m_pSocket;
}

void CChannel::SetReadCB(FuncEventCB func)
{
    m_funReadCB = func;
}

void CChannel::SetWriteCB(FuncEventCB func)
{
    m_funWriteCB = func;
}

void CChannel::SetCloseCB(FuncEventCB func)
{
    m_funCloseCB = func;
}

void CChannel::SetReadStatus(bool bStatus)
{
    if (bStatus)
    {
        m_eventType |= EVENT_IN;
    }
    else
    {
        m_eventType &= (~EVENT_IN);
    }
    m_poller->UpdateEvent(m_pSocket, m_eventType, nullptr);
}

void CChannel::SetWriteStatus(bool bStatus)
{
    if (bStatus)
    {
        m_eventType |= EVENT_OUT;
    }
    else
    {
        m_eventType &= (~EVENT_OUT);
    }
    m_poller->UpdateEvent(m_pSocket, m_eventType, nullptr);
}   

void CChannel::SetCloseStatus(bool bStatus)
{

}


}