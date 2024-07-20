#pragma once
#include <string>
#include <cstdint>
#include "Acceptor.h"
#include "EventLoop.h"
#include "ISocket.h"

namespace yuan {

class CTcpServer
{
public:
    using ReadMsgCB = std::function<void(CConnector::ptr, IBuffer*)>;

    CTcpServer(const std::string& strIp, std::int32_t nPort);
    ~CTcpServer();

    void OnConnection(ISocket::ptr pSocket);
    void OnClose(CConnector::ptr pConnector);

    void SetReadMsgCB(ReadMsgCB funcCB);
    void Start();

private:
    std::unique_ptr<CEventLoop> m_pLoop;
    std::unique_ptr<CAcceptor> m_pAccceptor;
    std::set<CConnector::ptr> m_connectorMgr;
    ReadMsgCB m_cbReadMsg;
};

}