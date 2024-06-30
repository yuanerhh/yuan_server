#pragma once

#include <sys/socket.h>
#include <string>

namespace yuan {

enum TCP_VERSION
{
    IPV4, 
    IPV6, 
};

class CTcpSocket
{
public:
    CTcpSocket(TCP_VERSION version = IPV4);
    ~CTcpSocket();
    void Bind(const std::string& strIp, int port);
    void Listen(int backlog = 0);
    void Connect(const std::string& strIp, int port);

private:
    int m_socket = -1; 
    std::string m_strIp = "";
    int m_port = 0;
    TCP_VERSION m_version = IPV4;
};

}