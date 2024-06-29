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
        CTcpSocket objSocket("127.0.0.1", 12345, IPV6);
    }
    catch(const CException& e)
    {
        std::cerr << e.what() << endl;
    }
    
    return 0;
}
