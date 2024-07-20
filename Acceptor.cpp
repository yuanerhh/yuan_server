#include "Acceptor.h"

#include "EventLoop.h"
#include "TcpSocket.h"
#include "Log.h"
#include "Exception.h"

using namespace std;

namespace yuan {

CAcceptor::CAcceptor(CEventLoop* pEventLoop, const std::string& strIp, std::int32_t nPort)
    : m_pEventLoop(pEventLoop)
{
    m_pSocket = make_shared<CTcpSocket>();
    m_pSocket->SetNoBlock(true);
    m_pSocket->SetReuseAddr(true);
    m_pSocket->SetReusePort(true);
    m_pSocket->Bind({strIp, nPort});
    m_pSocket->Listen();

    m_pChannel = make_shared<CChannel>(m_pEventLoop, m_pSocket, true);
    m_pChannel->SetReadStatus(true);
    m_pChannel->SetEdgeTrigger(true);
    m_pChannel->SetReadCB(std::bind(&CAcceptor::OnConnection, this));
    m_pEventLoop->AddChannel(m_pChannel);
}

CAcceptor::~CAcceptor()
{
    m_pEventLoop->RemoveChannel(m_pChannel);
}

void CAcceptor::OnConnection()
{
    try
    {
        auto pClientSock = m_pSocket->Accept();
        m_cbConnect(pClientSock);
        myLog << "accept new client ip: " << pClientSock->GetAddr().strIp 
            << ", port: " << pClientSock->GetAddr().port << endl;

    }
    catch(const CException& e)
    {
        myLog << e.what() << '\n';
    }
}

void CAcceptor::SetConnectCB(ConnectCB funcCB)
{
    m_cbConnect = funcCB;
}

}