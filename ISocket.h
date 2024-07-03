#pragma once

#include <string>
#include <stdint.h>
#include <memory>

namespace yuan
{

class ISocket
{
public:    
    using ptr = std::shared_ptr<ISocket>;

    virtual ~ISocket() {}
    virtual int GetFd() = 0;
    virtual std::int32_t Send(const char* pBuf, size_t size) = 0;
    virtual std::int32_t Recv(char* pBuf, size_t size) = 0;
};

}