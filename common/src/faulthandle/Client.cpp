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
            std::cout << "CLIENT COULDNT CONNECT TO MQ" << std::endl;
            msg = BAD_MSG;
            return false;
        }
        
        //CLEAR THE QUEUE
        bool areMoreMsgs = true;
        Message fakeMsg; 
        while(areMoreMsgs && _pMQ->recvMessage(fakeMsg, areMoreMsgs))
        { }

        if(!_send(msg))
        {
            std::cout << "CLIENT COULDNT SEND MESSAGE" << std::endl;
            msg = BAD_MSG;
            return false;
        }
        else if(!_recieve(msg))
        {
            std::cout << "CLIENT COULDNT RECV MESSAGE" << std::endl;
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
            std::cout << "CLIENT RX TIMEOUT!" << std::endl;
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
        std::cout << "  expected message was : " << expectedMsgType << std::endl; 
        std::cout << "Rx-ed message was type : "<< msg.getMessageType() << std::endl;
        return (expectedMsgType == msg.getMessageType());    
    }

    
    
} // namespace FaultHandle
