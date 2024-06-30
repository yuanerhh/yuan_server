#include "TcpSocket.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <fcntl.h>
#include "Exception.h"

using namespace std;

namespace yuan {

CTcpSocket::CTcpSocket()
{
    int nSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == nSocket)
    {
        CUnexpectedThrow(string("Create socket failed, info: ") + strerror(errno));
    }
    m_socket = nSocket;
}

CTcpSocket::CTcpSocket(int socket)
    : m_socket(socket)
{}

CTcpSocket::~CTcpSocket()
{
    if (-1 != m_socket)
    {
        close(m_socket);
        m_socket = -1;
    }
}

void CTcpSocket::Bind(const NET_ADDR& stAddr)
{
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(stAddr.strIp.c_str());
    addr.sin_port = htons(stAddr.port);
    if (-1 == bind(m_socket, (sockaddr *)&addr, sizeof(addr)))
    {
        CNetOptErrorThrow(string("Bind socket failed, info: ") + strerror(errno));
    }

    m_stBindAddr = stAddr;
}

void CTcpSocket::Listen(int backlog)
{
    if (-1 == listen(m_socket, backlog))
    {
        CNetOptErrorThrow(string("Listen socket failed, info: ") + strerror(errno));
    }
}

void CTcpSocket::Connect(const NET_ADDR& stAddr)
{
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(stAddr.strIp.c_str());
    addr.sin_port = htons(stAddr.port);
    if (-1 == connect(m_socket, (sockaddr *)&addr, sizeof(addr)))
    {
        CNetOptErrorThrow(string("Connect failed, info: ") + strerror(errno));
    }

    m_stConnectAddr = stAddr;
}

CTcpSocket::ptr CTcpSocket::Accept()
{
    sockaddr_in addr;
    socklen_t addrLen;
    memset(&addr, 0, sizeof(addr));
    if (-1 == accept4(m_socket, (sockaddr *)&addr, &addrLen, SOCK_NONBLOCK))
    {
        CNetOptErrorThrow(string("Accept4 failed, info: ") + strerror(errno));
    }

    CTcpSocket::ptr pAccept = make_shared<CTcpSocket>();
    char buf[50] = {0};
    if (inet_ntop(AF_INET, &(addr.sin_addr), buf, sizeof(buf)) == NULL) 
    {
        CNetOptErrorThrow(string("inet_ntop failed, info: ") + strerror(errno));
    }

    pAccept->m_stBindAddr.strIp = buf;
    pAccept->m_stBindAddr.port = ntohs(addr.sin_port);
}

void CTcpSocket::SetNoBlock(bool status)
{
    int flags = fcntl(m_socket, F_GETFL, 0);
    if (-1 == flags)
    {
        CNetOptErrorThrow(string("F_GETFL failed, info: ") + strerror(errno));
    }

    if (-1 == fcntl(m_socket, F_SETFL, flags | SOCK_NONBLOCK))
    {
        CNetOptErrorThrow(string("F_SETFL noblock failed, info: ") + strerror(errno));
    }
}

void CTcpSocket::SetReuseAddr(bool status)
{
    if (-1 == setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &status, sizeof(status)))
    {
        CNetOptErrorThrow(string("Set reuse addr failed, info: ") + strerror(errno));
    }
}

void CTcpSocket::SetReusePort(bool status)
{
    if (-1 == setsockopt(m_socket, SOL_SOCKET, SO_REUSEPORT, &status, sizeof(status)))
    {
        CNetOptErrorThrow(string("Set reuse port failed, info: ") + strerror(errno));
    }
}

void CTcpSocket::SetNoDelay(bool status)
{
    if (-1 == setsockopt(m_socket, SOL_SOCKET, TCP_NODELAY, &status, sizeof(status)))
    {
        CNetOptErrorThrow(string("Set tcp nodelay failed, info: ") + strerror(errno));
    }
}

void CTcpSocket::SetKeepalive(bool status)
{
    if (-1 == setsockopt(m_socket, SOL_SOCKET, SO_KEEPALIVE, &status, sizeof(status)))
    {
        CNetOptErrorThrow(string("Set keepalive failed, info: ") + strerror(errno));
    }
}


}