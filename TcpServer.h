#pragma once
#include <string>
#include <cstdint>
#include "Acceptor.h"
#include "EventLoop.h"
#include "ISocket.h"
#include "ThreadPool.h"

namespace yuan {

class CTcpServer
{
public:
    using ReadMsgCB = std::function<void(CConnector::ptr, IBuffer*)>;

    CTcpServer(const std::string& strIp, std::int32_t nPort, size_t nThreadNum = 5);
    ~CTcpServer();

    void SetReadMsgCB(ReadMsgCB funcCB);
    void Start();

private:
    void __OnConnection(ISocket::ptr pSocket);
    void __OnClose(CConnector::ptr pConnector);

private:
    std::unique_ptr<CEventLoop> m_pMainLoop;
    std::vector<std::shared_ptr<CEventLoop>> m_vecSubLoops;
    std::unique_ptr<CAcceptor> m_pAccceptor;
    std::unique_ptr<CThreadPool> m_pThreadPool;
    std::set<CConnector::ptr> m_connectorMgr;
    ReadMsgCB m_cbReadMsg;
    size_t m_nThreadNum = 0;
    std::uint64_t m_ui64ConnectorCnt = 0;
};

}