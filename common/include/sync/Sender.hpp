#ifndef SYNC_SENDER_HPP
#define SYNC_SENDER_HPP

#include <core/Thread.hpp>

#include <boost/interprocess/ipc/message_queue.hpp>

#include <mutex>

namespace Sync
{
    /**
     * @brief Will send Sync messages when prompted
     */
    class Sender : public Core::Thread
    {
        private:
            enum State_e {
                INIT, // This Sync Sender object has been created.
                JOINING_MQ, // attempting to connect to the messaging queue
                IDLE, // Waiting to send
                SENDING // Attempting to send
            };

        public:
            Sender(std::string, std::string);
            ~Sender();

        public: 
            std::string getMessageQueueName();
            void setMessageQueueName(std::string);

        private:
            void _setState(State_e);
            State_e _getState();
            bool _sendSync();

        private: // Core::thread
            void _run() override;

        private:
            std::mutex _mutex;
            State_e _state;
            std::string _id;
            std::string _messageQueueName;
            std::shared_ptr<boost::interprocess::message_queue> _pMQ;
    };
} // namespace Sync

#endif