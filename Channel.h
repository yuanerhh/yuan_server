/**
 * @file Channel.h
 * @author your name (you@domain.com)
 * @brief 管理一个fd的所有事件配置以及处理环节
 * @version 0.1
 * @date 2024-07-03
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once

#include <functional>
#include "IEventPoller.h"

namespace yuan {

class CEventLoop;

using FuncEventCB = std::function<void(void)>;

class CChannel : public std::enable_shared_from_this<CChannel>
{
public:
    using ptr = std::shared_ptr<CChannel>;

public:
    CChannel(CEventLoop* pEventLoop, ISocket::ptr pSocket, bool bIsListen);
    ~CChannel();
    ISocket::ptr GetSocket();
    void SetReadCB(FuncEventCB func);
    void SetWriteCB(FuncEventCB func);
    void SetCloseCB(FuncEventCB func);
    void SetReadStatus(bool bStatus);
    void SetWriteStatus(bool bStatus);
    void SetCloseStatus(bool bStatus);
    void SetEdgeTrigger(bool bStatus);
    void HandleEvent(const EVENT_DATA& stData);

private:
    CEventLoop* m_pEventLoop;
    IEventPoller* m_poller;
    ISocket::ptr m_pSocket;
    FuncEventCB m_funReadCB; 
    FuncEventCB m_funWriteCB;
    FuncEventCB m_funCloseCB;
    EVENT_TYPE m_eventType;
    bool m_bIsListen;
};


}