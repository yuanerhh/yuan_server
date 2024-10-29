#pragma once

#include <sys/socket.h>
#include <string>
#include <memory>
#include "NetAddr.h"
#include "ISocket.h"
#include "Utils.h"

namespace yuan {

class CTcpSocket : public ISocket, public CNonCoybale
{
public:
    using ptr = std::shared_ptr<CTcpSocket>;
public:
    CTcpSocket();
    explicit CTcpSocket(int socket);
    ~CTcpSocket();

    void Close() override;
    int GetFd() override;
    CNetAddr GetAddr() override;

    void Bind(const CNetAddr& stAddr) override;
    void Listen(int backlog = LISTEN_SIZE) override;
    void Connect(const CNetAddr& stAddr) override;
    ISocket::ptr Accept() override;
    std::int32_t Send(const char* pBuf, size_t size) override;
    std::int32_t Recv(char* pBuf, size_t size) override;

    void SetNoBlock(bool status) override;
    void SetReuseAddr(bool status) override;
    void SetReusePort(bool status) override;
    void SetNoDelay(bool status) override;
    void SetKeepalive(bool status) override;

private:
    int m_socket = -1; 
    CNetAddr m_stBindAddr;
    CNetAddr m_stConnectAddr;
};

}