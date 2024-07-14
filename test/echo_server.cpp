#include <iostream>
#include "Channel.h"
#include "EventLoop.h"
#include "TcpSocket.h"
#include "Exception.h"
#include "Log.h"

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

    try
    {
        ISocket::ptr pSocket = make_shared<CTcpSocket>();
        pSocket->SetNoBlock(true);
        pSocket->SetReuseAddr(true);
        pSocket->SetReusePort(true);
        pSocket->Bind({strIp, 12345});
        pSocket->Listen();
        CEventLoop loop;
        CChannel::ptr pChannel = make_shared<CChannel>(&loop, pSocket, true);
        pChannel->SetReadStatus(true);
        pChannel->SetEdgeTrigger(true);
        loop.AddChannel(pChannel);
        loop.Start();
    }
    catch(const CException& e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
