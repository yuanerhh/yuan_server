#include <iostream>
#include "TcpServer.h"
#include "Exception.h"
#include "Log.h"
#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>

using namespace std;
using namespace yuan;

void ReadMsgCB(CConnector::ptr pConn, IBuffer* pBuf)
{
    myLog << "in buf capacity: " << pBuf->Capacity() << ", datasize: "
                << pBuf->DataSize() << endl;

    myLog << "thread id: " << syscall(SYS_gettid) << endl;
    
    auto data = pBuf->ReadAll();
    pConn->Send(data.data(), data.size());
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
