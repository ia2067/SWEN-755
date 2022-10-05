#ifndef FAULTHANDLE_SERVER_HPP
#define FAULTHANDLE_SERVER_HPP

#include <core/Thread.hpp>
#include <core/MessageQueue.hpp>

#include <boost/signals2.hpp>

namespace FaultHandle
{
    class Message;

    class Server : public Core::Thread
    {
        public:
            enum State_e {
                INIT, 
                CREATING_MQ,
                LISTENING,
                SHUTDOWN
            };

        public:
            Server(std::string);
            ~Server() = default;

        public:
            std::string getMessageQueueName();
            State_e getState();

        public: //signals
            boost::signals2::signal<void(void)> sigWakeUp;
            boost::signals2::signal<std::list<int>(void)> sigGetData;

        private:
            void _setState(State_e);
        
        private:
            void _run() override;


        private:
            std::chrono::milliseconds _init();
            std::chrono::milliseconds _createMQ();
            std::chrono::milliseconds _listen();

        private:
            void _handleGetData();
            void _handleWakeUp();

        private:
            std::mutex _mutex;
            State_e _state;
            std::shared_ptr<Core::MessageQueue<Message>> _pMQ;
    };
} // namespace FaultHandle


#endif //FAULTHANDLE_SERVER_HPP