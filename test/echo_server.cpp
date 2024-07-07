#include <iostream>
#include "Channel.h"
#include "EventLoop.h"
#include "TcpSocket.h"

using namespace std;
using namespace yuan;

int main(int argc, char const *argv[])
{
    if (argc < 3)
    {
        cout << "Usage: server ip port" << endl;
        return 0;
    }

    string strIp = argv[1];
    int port = std::stoi(argv[2]);

    ISocket::ptr pSocket = make_shared<CTcpSocket>();
    pSocket->Bind({"127.0.0.1", 12345});
    pSocket->Listen();
    CEventLoop loop;
    CChannel::ptr pChannel = make_shared<CChannel>(&loop, pSocket, true);
    loop.AddChannel(pChannel);
    loop.Start();

    return 0;
}
