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
        // try to rx 3 times, if it dont got it, it dont got it...
        const int NUM_CHECKS = 3;
        bool idcaboutthis = true;

        int counter = 0;
        while(_pMQ->recvMessage(msg, idcaboutthis) && counter++ < NUM_CHECKS)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        return (counter != NUM_CHECKS);    
    }

    
    
} // namespace FaultHandle
