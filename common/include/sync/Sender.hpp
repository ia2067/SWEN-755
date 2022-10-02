#ifndef SYNC_SENDER_HPP
#define SYNC_SENDER_HPP

#include <core/Thread.hpp>
#include <core/MessageQueue.hpp>

#include <boost/serialization/list.hpp>

#include <mutex>

namespace Sync
{
    class Message;
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
                SENDING, // Attempting to send
                SHUTDOWN
            };

        public:
            Sender(std::string, std::string);
            ~Sender();

        public: 
            std::string getMessageQueueName();
            std::chrono::milliseconds getSendingInterval();
            //void setSendingInterval(std::chrono::milliseconds);
            void cacheValues(std::list<int> cacheList);

        private:
            void _setState(State_e);
            State_e _getState();
            bool _sendSync();

        private: // Core::thread
            void _run() override;

        private: // methods for various states
            std::chrono::milliseconds _init();
            std::chrono::milliseconds _joinMQ();
            std::chrono::milliseconds _idle();
            std::chrono::milliseconds _sending();

        private:
            std::mutex _mutex;
            State_e _state;
            std::string _id;
            int _syncSegment;
            std::chrono::milliseconds _sendingInterval;
            std::shared_ptr<Core::MessageQueue<Message>> _pMQ;
            std::list<int> _cacheList;
    };
} // namespace Sync

#endif