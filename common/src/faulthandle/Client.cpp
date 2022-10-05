#include <faulthandle/Client.hpp>
#include <faulthandle/Message.hpp>
#include <thread>
#include <chrono>

namespace FaultHandle
{
    

    Client::Client(std::string mqn)
    : _pMQ(std::make_shared<Core::MessageQueue<Message>>(mqn, false))
    { }

    bool Client::query(Message& msg)
    {
        Message BAD_MSG(BAD_MESSAGE_TYPE, {});

        // scope lock
        std::lock_guard<std::mutex> lock(_mutex);
        if(!_pMQ->connect())
        {
            msg = BAD_MSG;
            return false;
        }
        else if(!_send(msg))
        {
            msg = BAD_MSG;
            return false;
        }
        else if(!_recieve(msg))
        {
            msg = BAD_MSG;
            return false;
        }
        return true;
    }
    bool Client::_send(Message& msg)
    {
        return _pMQ->sendMessage(msg);
    }
    bool Client::_recieve(Message& msg)
    {
        auto oldMsgType = msg.getMessageType();

        if(!_pMQ->timedRecvMessage(msg, std::chrono::milliseconds(1000)))
        {
            return false;
        }

        MessageType_e expectedMsgType = BAD_MESSAGE_TYPE;
        switch(oldMsgType)
        {
            case CMD_GETDATA:
                expectedMsgType = RSP_GETDATA;
                break;
            case CMD_WAKEUP:
                expectedMsgType = RSP_WAKEUP;
                break;
            default:
                expectedMsgType = BAD_MESSAGE_TYPE;
                break;
        }
        
        return (expectedMsgType == msg.getMessageType());    
    }

    
    
} // namespace FaultHandle
