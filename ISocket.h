#pragma once

#include <string>
#include <stdint.h>
#include <memory>
#include "NetAddr.h"

#define LISTEN_SIZE 125

namespace yuan
{

class ISocket
{
public:    
    using ptr = std::shared_ptr<ISocket>;

    virtual ~ISocket() {}
    virtual void Close() = 0;
    virtual int GetFd() = 0;
    virtual CNetAddr GetAddr() = 0;

    virtual void Bind(const CNetAddr& stAddr) = 0;
    virtual void Listen(int backlog = LISTEN_SIZE) = 0;
    virtual void Connect(const CNetAddr& stAddr) = 0;
    virtual ISocket::ptr Accept() = 0;
    virtual std::int32_t Send(const char* pBuf, size_t size) = 0;
    virtual std::int32_t Recv(char* pBuf, size_t size) = 0;

    virtual void SetNoBlock(bool status) = 0;
    virtual void SetReuseAddr(bool status) = 0;
    virtual void SetReusePort(bool status) = 0;
    virtual void SetNoDelay(bool status) = 0;
    virtual void SetKeepalive(bool status) = 0;
};

}