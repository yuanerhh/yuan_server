#include "Channel.h"

#include <cstring>
#include "Log.h"
#include "EventLoop.h"
#include "Exception.h"

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
    // myLog << "CChannel ptr" << hex << (void *)this << dec << ", socket ref: " << m_pSocket.use_count() << endl;

    //销毁channel前，需要清除socket绑定的事件以及socket的智能指针引用
    //m_pSocket的生命周期应与其绑定的CChannel对象一致
    m_poller->RemoveEvent(m_pSocket);
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
    const int nBufSize = 1024;
    char buf[nBufSize] = {0};
    // if (stData.type & EVENT_CLOSE)
    // {
    //     myLog << "client: " << m_pSocket->GetFd() << " disconnect!" << endl;
    //     if (m_funCloseCB)
    //     {
    //         m_funCloseCB();
    //     }
    //     m_pEventLoop->RemoveChannel(shared_from_this());
    //     myLog << "GetChannelSize: " << m_pEventLoop->GetChannelSize() << endl;
    // }

    if (stData.type & EVENT_IN)
    {
        if (m_bIsListen)
        {
            try
            {
                //TODO: 当前Accept可能会出现Invalid Param错误，暂不知原因，所以添加try catch处理
                auto pClientSock = m_pSocket->Accept();
                auto pClientCh = make_shared<CChannel>(m_pEventLoop, pClientSock, false);
                pClientCh->SetReadStatus(true);
                pClientCh->SetEdgeTrigger(true);
                m_pEventLoop->AddChannel(pClientCh);

                myLog << "accept new client ip: " << pClientSock->GetAddr().strIp 
                    << ", port: " << pClientSock->GetAddr().port << endl;
            }
            catch(const CException& e)
            {
                myLog << e.what() << '\n';
            }
        }
        else
        {
            // myLog << "client event in" << endl;
            while (true)
            {
                bzero(buf, nBufSize);
                auto nRecvSize = m_pSocket->Recv(buf, nBufSize);
                if (nRecvSize < 0)
                {
                    break;
                }
                //client 套接字关闭
                else if (nRecvSize == 0)
                {
                    myLog << "client: " << m_pSocket->GetFd() << " disconnect!" << endl;
                    if (m_funCloseCB)
                    {
                        m_funCloseCB();
                    }
                    
                    m_pEventLoop->RemoveChannel(shared_from_this());
                    break;
                }

                m_pSocket->Send(buf, nRecvSize);
            }
        }
    }

    

}


}