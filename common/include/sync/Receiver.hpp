#ifndef SYNC_RECEIVER_HPP
#define SYNC_RECEIVER_HPP

#include <core/Thread.hpp>

#include <boost/interprocess/ipc/message_queue.hpp>

#include <string>
#include <mutex>

namespace Sync
{
    /**
     * @brief A Sync::Receiver which handles Sync::Sender messages
     */
    class Receiver : public Core::Thread
    {
        private:
            enum State_e {
                INIT, // This Sync Sender object has been created.
                CREATING_MQ, // attempting to connect to the messaging queue
                LISTENING
            };

        public:
            Receiver(std::string);
            ~Receiver();

        public:
            std::string getMessageQueueName();
            void setMessageQueueName(std::string);

        public: // Do we need this?
            //void addSenderId(std::string);

        private:
            void _setState(State_e);
            State_e _getState();

        private: //Core::Thread
            void _run() override;

        private:
            std::mutex _mutex;
            State_e _state;
            std::string _messageQueueName;
    };
} // namespace Sync

#endif