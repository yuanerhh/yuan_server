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
    CTcpSocket(const std::string& strIp, int port, TCP_VERSION version = IPV4);
    ~CTcpSocket() {}


private:
    int m_socket = 0; 
    std::string m_strIp = "";
    int m_port = 0;
    TCP_VERSION m_version = IPV4;
};

}