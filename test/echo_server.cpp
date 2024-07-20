#include <iostream>
#include "TcpServer.h"
#include "Exception.h"

using namespace std;
using namespace yuan;

void ReadMsgCB(CConnector::ptr pConn, char* pBuf, size_t size)
{
    pConn->Send(pBuf, size);
}

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
        CTcpServer server(strIp, port);
        server.SetReadMsgCB(&ReadMsgCB);
        server.Start();
    }
    catch(const CException& e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
