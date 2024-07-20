#pragma once

#include <string>

namespace yuan {

class CBuffer
{
public:
    CBuffer();
    ~CBuffer();

private:
    std::string m_strData;
};

}