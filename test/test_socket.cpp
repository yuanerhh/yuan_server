#include <iostream>
#include "../TcpSocket.h"
#include "../Exception.h"

using namespace std;
using namespace yuan;

int main(int argc, char const *argv[])
{
    try
    {
        /* code */
        CTcpSocket objSocket;
        objSocket.Bind({"127.0.0.1", 12345});
        objSocket.Listen();
    }
    catch(const CException& e)
    {
        std::cerr << e.what() << endl;
    }
    
    return 0;
}
