#pragma once
#include <cstddef>
#include <utility>
#include <string>

namespace yuan {

class IBuffer 
{
public:
    using DATA = std::string;

    virtual ~IBuffer() {}
    virtual size_t DataSize() = 0;
    virtual size_t Capacity() = 0;
    virtual void Resize(size_t size) = 0;
    virtual void Write(const char* pData, size_t size) = 0;
    virtual DATA Read(size_t size) = 0;
    virtual DATA ReadAll() = 0;
};

}