#include "TcpSocket.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <fcntl.h>
#include "Exception.h"
#include "Log.h"

using namespace std;

namespace yuan {

CTcpSocket::CTcpSocket()
{
    int nSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == nSocket)
    {
        CNetOptErrorThrow(string("Create socket failed, info: ") + strerror(errno));
    }
    m_socket = nSocket;
}

CTcpSocket::CTcpSocket(int socket)
    : m_socket(socket)
{}

CTcpSocket::~CTcpSocket()
{
    try
    {
        Close();
    }
    catch(const CException& e)
    {
        myLog << e.what() << std::endl;
    }
}

void CTcpSocket::Bind(const CNetAddr& stAddr)
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

void CTcpSocket::Connect(const CNetAddr& stAddr)
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

ISocket::ptr CTcpSocket::Accept()
{
    sockaddr_in addr;
    socklen_t addrLen;
    memset(&addr, 0, sizeof(addr));

    auto ret = accept4(m_socket, (sockaddr *)&addr, &addrLen, SOCK_NONBLOCK);
    if (-1 == ret)
    {
        CNetOptErrorThrow(string("Accept4 failed, info: ") + strerror(errno));
    }

    CTcpSocket::ptr pAccept = make_shared<CTcpSocket>(ret);
    char buf[50] = {0};
    if (inet_ntop(AF_INET, &(addr.sin_addr), buf, sizeof(buf)) == NULL) 
    {
        CNetOptErrorThrow(string("inet_ntop failed, info: ") + strerror(errno));
    }

    pAccept->m_stBindAddr.strIp = buf;
    pAccept->m_stBindAddr.port = ntohs(addr.sin_port);

    return pAccept;
}

void CTcpSocket::SetNoBlock(bool status)
{
    int flags = fcntl(m_socket, F_GETFL, 0);
    if (-1 == flags)
    {
        CNetOptErrorThrow(string("F_GETFL failed, info: ") + strerror(errno));
    }

    if (status)
    {
        flags |= SOCK_NONBLOCK;
    }
    else
    {
        flags &= (~SOCK_NONBLOCK);
    }

    if (-1 == fcntl(m_socket, F_SETFL, flags | SOCK_NONBLOCK))
    {
        CNetOptErrorThrow(string("F_SETFL noblock failed, info: ") + strerror(errno));
    }
}

void CTcpSocket::SetReuseAddr(bool status)
{
    int nStatus = 0;
    if (status)
    {
        nStatus = 1;
    }
    else
    {
        nStatus = 0;
    }

    if (-1 == setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &nStatus, sizeof(nStatus)))
    {
        CNetOptErrorThrow(string("Set reuse addr failed, info: ") + strerror(errno));
    }
}

void CTcpSocket::SetReusePort(bool status)
{
    int nStatus = 0;
    if (status)
    {
        nStatus = 1;
    }
    else
    {
        nStatus = 0;
    }

    if (-1 == setsockopt(m_socket, SOL_SOCKET, SO_REUSEPORT, &nStatus, sizeof(nStatus)))
    {
        CNetOptErrorThrow(string("Set reuse port failed, info: ") + strerror(errno));
    }
}

void CTcpSocket::SetNoDelay(bool status)
{
    int nStatus = 0;
    if (status)
    {
        nStatus = 1;
    }
    else
    {
        nStatus = 0;
    }

    if (-1 == setsockopt(m_socket, SOL_SOCKET, TCP_NODELAY, &nStatus, sizeof(nStatus)))
    {
        CNetOptErrorThrow(string("Set tcp nodelay failed, info: ") + strerror(errno));
    }
}

void CTcpSocket::SetKeepalive(bool status)
{
    int nStatus = 0;
    if (status)
    {
        nStatus = 1;
    }
    else
    {
        nStatus = 0;
    }
    
    if (-1 == setsockopt(m_socket, SOL_SOCKET, SO_KEEPALIVE, &nStatus, sizeof(nStatus)))
    {
        CNetOptErrorThrow(string("Set keepalive failed, info: ") + strerror(errno));
    }
}

void CTcpSocket::Close()
{
    if (-1 != m_socket)
    {
        close(m_socket);
        m_socket = -1;
    }
}

int CTcpSocket::GetFd()
{
    return m_socket;
}

CNetAddr CTcpSocket::GetAddr()
{
    return m_stBindAddr;
}

std::int32_t CTcpSocket::Send(const char* pBuf, size_t size)
{
    auto nSize = send(m_socket, pBuf, size, 0);
    if (nSize >= 0)
    {
        return nSize;
    }
    
    return -errno;
}
std::int32_t CTcpSocket::Recv(char* pBuf, size_t size)
{
    auto nSize = recv(m_socket, pBuf, size, 0);
    if (nSize >= 0)
    {
        return nSize;
    }
    
    return -errno;
}

}