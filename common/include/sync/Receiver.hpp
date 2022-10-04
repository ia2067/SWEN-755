#ifndef SYNC_RECEIVER_HPP
#define SYNC_RECEIVER_HPP

#include <core/Thread.hpp>
#include <core/MessageQueue.hpp>

#include <boost/signals2.hpp>
#include <boost/bind.hpp>

#include <string>
#include <mutex>
#include <boost/serialization/list.hpp>

namespace Sync
{
    class Message;
    /**
     * @brief A Sync::Receiver which handles Sync::Sender messages
     */
    class Receiver : public Core::Thread
    {
        private:
            enum State_e {
                INIT, // This Sync Sender object has been created.
                CREATING_MQ, // attempting to connect to the messaging queue
                LISTENING,
                CHECK_VALUES,
                SHUTDOWN
            };

        public:
            Receiver(std::string);
            ~Receiver();

        public:
            std::string getMessageQueueName();

        public:
            boost::signals2::signal<void (std::list<int> ())> RxSignal;

        private:
            void _setState(State_e);
            State_e _getState();

        private: //Core::Thread
            void _run() override;

        private:
            std::chrono::milliseconds _init();
            std::chrono::milliseconds _createMQ();
            std::chrono::milliseconds _listen();
            std::chrono::milliseconds _checkValues();

        private:
            std::mutex _mutex;
            State_e _state;
            std::list<int> _cacheRxValues;
            std::shared_ptr<Core::MessageQueue<Message>> _pMQ;
    };
} // namespace Sync

#endif