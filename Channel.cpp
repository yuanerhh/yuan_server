#include "Channel.h"
#include "Log.h"
#include "EventLoop.h"

using namespace std;

namespace yuan {

CChannel::CChannel(CEventLoop* pEventLoop, ISocket::ptr pSocket, bool bIsListen)
    : m_pEventLoop(pEventLoop), m_poller(nullptr)
    , m_pSocket(pSocket), m_bIsListen(bIsListen)
{
    m_poller = pEventLoop->GetEventPoller().get();
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

void CChannel::SetEdgeTrigger(bool bStatus)
{
    if (bStatus)
    {
        m_eventType |= EVENT_EDGE_TRIGGER;
    }
    else
    {
        m_eventType &= (~EVENT_EDGE_TRIGGER);
    }
    m_poller->UpdateEvent(m_pSocket, m_eventType, nullptr);
}

void CChannel::HandleEvent(const EVENT_DATA& stData)
{
    if (stData.type & EVENT_CLOSE)
    {
        myLog << "client: " << m_pSocket->GetFd() << " disconnect!" << endl;
        m_funCloseCB();
        m_pEventLoop->RemoveChannel(shared_from_this());
    }

    if (stData.type & EVENT_IN)
    {
        if (m_bIsListen)
        {
            auto pClientSock = m_pSocket->Accept();
            auto pClientCh = make_shared<CChannel>(m_pEventLoop, pClientSock, false);
            pClientCh->SetReadStatus(true);
            pClientCh->SetEdgeTrigger(true);
            m_pEventLoop->AddChannel(pClientCh);

            myLog << "accept new client ip: " << pClientSock->GetAddr().strIp 
                << ", port: " << pClientSock->GetAddr().port;
        }
        else
        {
            myLog << "client event in" << endl;
        }
    }

    

}


}