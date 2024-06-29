#include "TcpSocket.h"

#include <sys/socket.h>
#include "Exception.h"

using namespace std;

namespace yuan {

CTcpSocket::CTcpSocket(const std::string& strIp, int port, TCP_VERSION version)
    : m_strIp(strIp), m_port(port), m_version(version)
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

}