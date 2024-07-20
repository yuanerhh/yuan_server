#include "Connector.h"

#include "EventLoop.h"
#include "TcpSocket.h"
#include "Log.h"
#include "Exception.h"

using namespace std;

namespace yuan {

CConnector::CConnector(CEventLoop* pEventLoop, ISocket::ptr pSocket)
    : m_pEventLoop(pEventLoop)
    , m_pSocket(pSocket)
{
    m_pChannel = make_shared<CChannel>(pEventLoop, pSocket, false);
    m_pChannel->SetReadStatus(true);
    m_pChannel->SetEdgeTrigger(true);
    m_pChannel->SetReadCB(std::bind(&CConnector::OnReadMsg, this));
    m_pEventLoop->AddChannel(m_pChannel);
}

CConnector::~CConnector()
{
    m_pEventLoop->RemoveChannel(m_pChannel);
}

ISocket::ptr CConnector::GetSocket()
{
    return m_pSocket;
}

void CConnector::OnReadMsg()
{
    const int nBufSize = 1024;
    char buf[nBufSize] = {0};

    try
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
                if (m_cbClose)
                {
                    m_cbClose(shared_from_this());
                }
                break;
            }

            if (m_cbReadMsg)
            {
                m_cbReadMsg(shared_from_this(), buf, nRecvSize);
            }
            // m_pSocket->Send(buf, nRecvSize);
        }

    }
    catch(const CException& e)
    {
        myLog << e.what() << '\n';
    }
}

void CConnector::SetReadMsgCB(ReadMsgCB funcCB)
{
    m_cbReadMsg = funcCB;
}

void CConnector::SetCloseCB(CloseCB funcCB)
{
    m_cbClose = funcCB;
}

std::int32_t CConnector::Send(const char* pBuf, size_t size)
{
    m_pSocket->Send(pBuf, size);
}

}