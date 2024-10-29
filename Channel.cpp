#include "Channel.h"

#include <cstring>
#include "Log.h"
#include "EventLoop.h"
#include "Exception.h"

using namespace std;

namespace yuan {

CChannel::CChannel(CEventLoop* pEventLoop, ISocket::ptr pSocket, bool bIsListen)
    : m_pEventLoop(pEventLoop)
    , m_poller(pEventLoop->GetEventPoller().get())
    , m_pSocket(pSocket)
    , m_bIsListen(bIsListen)
{

}

CChannel::~CChannel()
{
    //销毁channel前，需要清除socket绑定的事件以及socket的智能指针引用
    //m_pSocket的生命周期应与其绑定的CChannel对象一致
    m_poller->RemoveEvent(m_pSocket);
}

ISocket::ptr CChannel::GetSocket()
{
    return m_pSocket;
}

void CChannel::SetReadCB(const FuncEventCB& func)
{
    m_funReadCB = func;
}

void CChannel::SetWriteCB(const FuncEventCB& func)
{
    m_funWriteCB = func;
}

void CChannel::SetCloseCB(const FuncEventCB& func)
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

void CChannel::SetCloseStatus(bool)
{
    //TODO
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
    if (stData.type & EVENT_OUT)
    {
        if (m_funWriteCB)
        {
            m_funWriteCB();
        }
    }

    //必须最后处理EVENT_IN，其中包含close事件，会关闭connector，
    //导致其他的事件处理异常，必须放到最后处理
    if (stData.type & EVENT_IN)
    {
        if (m_funReadCB)
        {
            m_funReadCB();
        }
    }
}


}