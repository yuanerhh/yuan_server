#pragma once
#include <memory>
#include <functional>
#include "Channel.h"

namespace yuan {

class CEventLoop;

class CConnector : public std::enable_shared_from_this<CConnector>
{
public:
    using ptr = std::shared_ptr<CConnector>;
    using ReadMsgCB = std::function<void(ISocket::ptr, char*, size_t size)>;
    using CloseCB = std::function<void(CConnector::ptr)>;

    CConnector(CEventLoop* pEventLoop, ISocket::ptr pSocket);
    ~CConnector();

    ISocket::ptr GetSocket();
    void OnReadMsg();
    void SetReadMsgCB(ReadMsgCB funcCB);
    void SetCloseCB(CloseCB funcCB);

private:
    CEventLoop* m_pEventLoop;
    ISocket::ptr m_pSocket;
    CChannel::ptr m_pChannel;
    ReadMsgCB m_cbReadMsg;
    CloseCB m_cbClose;
};

}