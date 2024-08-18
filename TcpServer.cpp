#include "TcpServer.h"

#include "Log.h"

using namespace std;

namespace yuan {

CTcpServer::CTcpServer(const std::string& strIp, std::int32_t nPort, size_t nThreadNum)
    : m_pMainLoop(new CEventLoop())
    , m_pAccceptor(nullptr)
    , m_pThreadPool(new CThreadPool(nThreadNum))
    , m_nThreadNum(nThreadNum)
{
    m_pAccceptor.reset(new CAcceptor(m_pMainLoop.get(), strIp, nPort));
    m_pAccceptor->SetConnectCB(std::bind(&CTcpServer::__OnConnection, this, placeholders::_1));

    for (int i = 0; i < nThreadNum; ++i)
    {
        shared_ptr<CEventLoop> pEventLoop = make_shared<CEventLoop>();
        m_pThreadPool->AddTask(std::bind(&CEventLoop::Start, pEventLoop.get()));
        m_vecSubLoops.push_back(pEventLoop);
    }
}

CTcpServer::~CTcpServer()
{}

void CTcpServer::SetReadMsgCB(ReadMsgCB funcCB)
{
    m_cbReadMsg = funcCB;
}

void CTcpServer::Start()
{
    m_pMainLoop->Start();
}

void CTcpServer::__OnConnection(ISocket::ptr pSocket)
{
    myLog;
    auto pConnector = make_shared<CConnector>(m_vecSubLoops[m_ui64ConnectorCnt++ % m_nThreadNum].get(), pSocket);
    pConnector->SetCloseCB(std::bind(&CTcpServer::__OnClose, this, placeholders::_1));
    pConnector->SetReadMsgCB(m_cbReadMsg);
    m_connectorMgr.insert(pConnector);
}

void CTcpServer::__OnClose(CConnector::ptr pConnector)
{
    m_connectorMgr.erase(pConnector);
}

}