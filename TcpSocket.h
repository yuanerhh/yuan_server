#pragma once

#include <sys/socket.h>
#include <string>
#include <memory>
#include "NetAddr.h"
#include "ISocket.h"

namespace yuan {

class CTcpSocket : public ISocket
{
public:
    using ptr = std::shared_ptr<CTcpSocket>;
public:
    CTcpSocket();
    CTcpSocket(int socket);
    ~CTcpSocket();

    virtual void Close() override;
    virtual int GetFd() override;

    virtual void Bind(const CNetAddr& stAddr) override;
    virtual void Listen(int backlog = LISTEN_SIZE) override;
    virtual void Connect(const CNetAddr& stAddr) override;
    virtual ISocket::ptr Accept() override;
    virtual std::int32_t Send(const char* pBuf, size_t size) override;
    virtual std::int32_t Recv(char* pBuf, size_t size) override;

    virtual void SetNoBlock(bool status) override;
    virtual void SetReuseAddr(bool status) override;
    virtual void SetReusePort(bool status) override;
    virtual void SetNoDelay(bool status) override;
    virtual void SetKeepalive(bool status) override;

private:
    int m_socket = -1; 
    std::string m_strIp = "";
    int m_port = 0;
    CNetAddr m_stBindAddr;
    CNetAddr m_stConnectAddr;
};

}