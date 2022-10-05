#ifndef FAULTHANDLE_CLIENT_HPP
#define FAULTHANDLE_CLIENT_HPP

#include <core/MessageQueue.hpp>
#include <faulthandle/Message.hpp>


#include <mutex>

namespace FaultHandle
{
    class Client
    {
        public:
        Client(std::string);
        ~Client() = default;

        public:
            bool query(Message&);
        private:
            bool _send(Message&);
            bool _recieve(Message&);


        private:
            std::mutex _mutex;
            std::shared_ptr<Core::MessageQueue<Message>> _pMQ;
            
    };
}

#endif // FAULTHANDLE_CLIENT_HPP