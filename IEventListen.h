#pragma once

namespace yuan {

enum EVENT_TYPE
{
    EVENT_IN = 0x1, 
    EVENT_OUT = 0x2    
};

class IEventListen
{
public:
    virtual ~IEventListen() {}

};


}