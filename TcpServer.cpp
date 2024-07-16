#include "TcpServer.h"

#include "Log.h"

using namespace std;

namespace yuan {

CTcpServer::CTcpServer(const std::string& strIp, std::int32_t nPort)
    : m_pLoop(new CEventLoop())
    , m_pAccceptor(nullptr)
{
    m_pAccceptor.reset(new CAcceptor(m_pLoop.get(), strIp, nPort));
    m_pAccceptor->SetConnectCB(std::bind(&CTcpServer::OnConnection, this, placeholders::_1));
}

CTcpServer::~CTcpServer()
{}

void CTcpServer::OnConnection(ISocket::ptr pSocket)
{
    myLog;
    auto pConnector = make_shared<CConnector>(m_pLoop.get(), pSocket);
    pConnector->SetCloseCB(std::bind(&CTcpServer::OnClose, this, placeholders::_1));
    pConnector->SetReadMsgCB(m_cbReadMsg);
    m_connectorMgr.insert(pConnector);
}

void CTcpServer::OnClose(CConnector::ptr pConnector)
{
    m_connectorMgr.erase(pConnector);
}

void CTcpServer::SetReadMsgCB(ReadMsgCB funcCB)
{
    m_cbReadMsg = funcCB;
}

void CTcpServer::Start()
{
    m_pLoop->Start();
}

}