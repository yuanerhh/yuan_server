#include "RingBuffer.h"
#include "Exception.h"
#include "Log.h"
#include "scope_exit.h"

using namespace std;

#define DEFAULT_BUF_SIZE 1024
#define MAX_BUF_SIZE (10 * 1024 * 1024)  //最大10MB

namespace yuan {

CRingBuffer::CRingBuffer()
    : m_pData(new char[DEFAULT_BUF_SIZE]), m_capacity(DEFAULT_BUF_SIZE)
    , m_readPos(0), m_writePos(0), m_isFull(false)
{}

CRingBuffer::~CRingBuffer()
{
    if (m_pData)
    {
        delete[] m_pData;
        m_pData = nullptr;
    }
}

size_t CRingBuffer::DataSize()
{
    if (m_writePos > m_readPos)
    {
        return m_writePos - m_readPos;
    }
    
    if (m_writePos < m_readPos)
    {
        return m_writePos + m_capacity - m_readPos;
    }
    
    if (m_isFull)
    {
        return 0;
    }
    else
    {
        return m_capacity;
    }
}

size_t CRingBuffer::Capacity()
{
    return m_capacity;
}

void CRingBuffer::Resize(size_t size)
{
    if (size < DataSize())
    {
        COutOfBoundThrow("size is smaller than DataSize");
    }
    
    if (size == DataSize())
    {
        m_isFull = true;
    }

    std::string dataTemp = ReadAll();
    delete[] m_pData;
    m_pData = new char[size];
    memcpy(m_pData, dataTemp.data(), dataTemp.size());
    
    if (m_writePos >= m_readPos)
    {
        m_capacity = size;
        return;
    }

    if (m_readPos + DataSize() < size)
    {
        m_writePos = m_readPos + DataSize();
    }
    else
    {
        m_writePos = m_writePos + m_capacity - size;
    }

    m_capacity = size;
}

void CRingBuffer::Write(const char* pData, size_t size)
{
    if (size > (Capacity() - DataSize()))
    {
        Resize(__CalResize(size));
    }

    scope_exit _([this]() {
        if (m_writePos == m_readPos)
        {
            m_isFull = true;
        }
        else
        {
            m_isFull = false;
        }
    });

    if (m_writePos < m_readPos)
    {
        memcpy(m_pData + m_writePos, pData, size);
        m_writePos += size;
        return;
    }

    if (m_writePos + size < m_capacity)
    {
        memcpy(m_pData + m_writePos, pData, size);
        m_writePos += size;
        return;
    }

    memcpy(m_pData + m_writePos, pData, m_capacity - m_writePos);
    memcpy(m_pData, pData + m_capacity - m_writePos, size - m_capacity + m_writePos);
    m_writePos = m_writePos + size - m_capacity;
}

IBuffer::DATA CRingBuffer::Read(size_t size)
{
    if (size > DataSize())
    {
        return ReadAll();
    }

    scope_exit _([this]() {
        if (m_writePos == m_readPos)
        {
            m_isFull = false;
        }
        else
        {
            m_isFull = true;
        }
    });

    if (m_readPos + size < m_capacity)
    {
        string strData(m_pData + m_readPos, size);
        m_readPos += size;
        return strData;
    }

    string strData(m_pData + m_readPos, m_capacity - m_readPos);
    strData.append(m_pData, size - m_capacity + m_readPos);
    m_readPos = size - m_capacity + m_readPos;

    return strData;
}

IBuffer::DATA CRingBuffer::ReadAll()
{
    m_isFull = false;
    if (m_writePos >= m_readPos)
    {
        string strData(m_pData + m_readPos, m_writePos - m_readPos);
        m_readPos = m_writePos;
        return strData;
    }

    string strData(m_pData + m_readPos, m_capacity - m_readPos);
    strData.append(m_pData, m_writePos);
    m_readPos = m_writePos;
    return strData;
}

size_t CRingBuffer::__CalResize(size_t size)
{
    size_t resize = m_capacity;
    while ((resize - DataSize()) < (size + DEFAULT_BUF_SIZE))
    {
        resize = resize * 2;
    }

    if (resize > MAX_BUF_SIZE)
    {
        COutOfBoundThrow("resize: " + std::to_string(resize) + " is exceed max buf size : " 
            + std::to_string(MAX_BUF_SIZE));
    }

    return resize;
}

}