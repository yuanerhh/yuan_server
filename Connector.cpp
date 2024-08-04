#include "Connector.h"

#include "EventLoop.h"
#include "TcpSocket.h"
#include "Log.h"
#include "Exception.h"
#include "RingBuffer.h"

using namespace std;

namespace yuan {

CConnector::CConnector(CEventLoop* pEventLoop, ISocket::ptr pSocket)
    : m_pEventLoop(pEventLoop)
    , m_pSocket(pSocket)
    , m_inputBuf(new CRingBuffer())
    , m_outBuf(new CRingBuffer())
{
    m_pChannel = make_shared<CChannel>(pEventLoop, pSocket, false);
    m_pChannel->SetReadStatus(true);
    m_pChannel->SetEdgeTrigger(true);
    m_pChannel->SetReadCB(std::bind(&CConnector::OnReadMsg, this));
    m_pChannel->SetWriteCB(std::bind(&CConnector::OnWriteMsg, this));
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
                //关闭后不能再进行后续操作
                return;
            }
            
            m_inputBuf->Write(buf, nRecvSize);
        }

        if (m_cbReadMsg)
        {
            m_cbReadMsg(shared_from_this(), m_inputBuf.get());
        }

    }
    catch(const CException& e)
    {
        myLog << e.what() << '\n';
    }
}

void CConnector::OnWriteMsg()
{
    if (m_outBuf->DataSize() > 0)
    {
        //追加数据
        m_strWriteData.append(m_outBuf->ReadAll());
    }
    auto size = m_pSocket->Send(m_strWriteData.data(), m_strWriteData.size());
    if (size >= m_strWriteData.size())
    {
        //所有的数据都完成了发送
        m_strWriteData = "";
        m_pChannel->SetWriteStatus(false);
    }
    else
    {
        //更新剩余发送数据
        m_strWriteData = m_strWriteData.substr(size, m_strWriteData.size() - size);
        m_pChannel->SetWriteStatus(true);
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
    try
    {
        m_outBuf->Write(pBuf, size);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    catch(const CException& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    m_pChannel->SetWriteStatus(true);
    OnWriteMsg(); //主动进行一次发送
}

}