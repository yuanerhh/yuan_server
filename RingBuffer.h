#pragma once

#include <string>
#include "IBuffer.h"

namespace yuan {

class CRingBuffer : public IBuffer
{
public:
    CRingBuffer();
    ~CRingBuffer();

    virtual size_t DataSize() override;
    virtual size_t Capacity() override;
    virtual void Resize(size_t size) override;
    virtual void Write(char* pData, size_t size) override;
    virtual DATA Read(size_t size) override;
    virtual DATA ReadAll() override;

private:
    /** 根据需要写入的数据大小计算resize数值
     *  size: 需要写入的数据大小
     *  return: 返回resize
     */
    size_t __CalResize(size_t size);

private:
    char* m_pData;
    size_t m_capacity;
    std::uint32_t m_readPos;
    std::uint32_t m_writePos;
};

}