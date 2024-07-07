#pragma once
#include <vector>
#include <cstdint>
#include <memory>
#include "ISocket.h"

namespace yuan {

enum _EVENT_TYPE
{
    EVENT_IN = 0x1, 
    EVENT_OUT = 0x2,
    EVENT_CLOSE = 0x4,
    EVENT_EDGE_TRIGGER = 0x8,
};

using EVENT_TYPE = std::int32_t;

struct EVENT_DATA
{
    EVENT_TYPE type = 0;
    void* pUserData = nullptr;
};

struct EVENT_OBJ
{
    ISocket::ptr pSocket = nullptr;
    EVENT_DATA stData;
};

class IEventPoller
{
public:
    using ptr = std::shared_ptr<IEventPoller>;
    virtual ~IEventPoller() {}
    virtual void UpdateEvent(const ISocket::ptr& pSocket, EVENT_TYPE emType, void* pEventData) = 0;
    virtual void RemoveEvent(const ISocket::ptr& pSocket) = 0;
    virtual std::vector<EVENT_OBJ> WaitEvent(std::uint32_t ui32Timeout) = 0;
};


}