#include "TcpSocket.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "Exception.h"

using namespace std;

namespace yuan {

CTcpSocket::CTcpSocket(TCP_VERSION version)
    : m_version(version)
{
    int domain = 0;
    if (version == IPV4)
    {
        domain = AF_INET;
    }
    else
    {
        CNotSupportThrow("Not support ipv6");
    }

    int nSocket = socket(domain, SOCK_STREAM, 0);
    if (-1 == nSocket)
    {
        CUnexpectedThrow(string("Create socket failed, info: ") + strerror(errno));
    }

    
}

CTcpSocket::~CTcpSocket()
{
    if (-1 != m_socket)
    {
        close(m_socket);
        m_socket = -1;
    }
}

void CTcpSocket::Bind(const std::string& strIp, int port)
{
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(strIp.c_str());
    addr.sin_port = htons(port);
    if (-1 == bind(m_socket, (sockaddr *)&addr, sizeof(addr)))
    {
        CNetOptErrorThrow(string("Bind socket failed, info: ") + strerror(errno));
    }
}

void CTcpSocket::Listen(int backlog)
{
    if (-1 == listen(m_socket, backlog))
    {
        CNetOptErrorThrow(string("Listen socket failed, info: ") + strerror(errno));
    }
}

void CTcpSocket::Connect(const std::string& strIp, int port)
{
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(strIp.c_str());
    addr.sin_port = htons(port);
    if (-1 == connect(m_socket, (sockaddr *)&addr, sizeof(addr)))
    {
        CNetOptErrorThrow(string("Connect failed, info: ") + strerror(errno));
    }
}

}