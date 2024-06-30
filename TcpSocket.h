#pragma once

#include <sys/socket.h>
#include <string>
#include <memory>

#define LISTEN_SIZE 125

namespace yuan {

struct NET_ADDR 
{
    std::string strIp = "";
    int port = 0;
};

class CTcpSocket
{
public:
    using ptr = std::shared_ptr<CTcpSocket>;
public:
    CTcpSocket();
    CTcpSocket(int socket);
    ~CTcpSocket();
    void Bind(const NET_ADDR& stAddr);
    void Listen(int backlog = LISTEN_SIZE);
    void Connect(const NET_ADDR& stAddr);
    CTcpSocket::ptr Accept();
    void SetNoBlock(bool status);
    void SetReuseAddr(bool status);
    void SetReusePort(bool status);
    void SetNoDelay(bool status);
    void SetKeepalive(bool status);

private:
    int m_socket = -1; 
    std::string m_strIp = "";
    int m_port = 0;
    NET_ADDR m_stBindAddr;
    NET_ADDR m_stConnectAddr;
};

}