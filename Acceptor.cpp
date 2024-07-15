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
    ISocket::ptr pSocket = make_shared<CTcpSocket>();
    pSocket->SetNoBlock(true);
    pSocket->SetReuseAddr(true);
    pSocket->SetReusePort(true);
    pSocket->Bind({strIp, nPort});
    pSocket->Listen();

    m_pChannel = make_shared<CChannel>(m_pEventLoop, pSocket, true);
    m_pChannel->SetReadStatus(true);
    m_pChannel->SetEdgeTrigger(true);
}

CAcceptor::~CAcceptor()
{

}

void CAcceptor::OnConnection()
{
    try
    {
        //TODO: 当前Accept可能会出现Invalid Param错误，暂不知原因，所以添加try catch处理
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