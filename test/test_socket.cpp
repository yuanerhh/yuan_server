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
        CTcpSocket objSocket();
    }
    catch(const CException& e)
    {
        std::cerr << e.what() << endl;
    }
    
    return 0;
}
