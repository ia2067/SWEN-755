#ifndef HEARTBEAT_SENDER_HPP
#define HEARTBEAT_SENDER_HPP

#include <core/Thread.hpp>

#include <chrono>
#include <boost/interprocess/ipc/message_queue.hpp>

#include <mutex>

namespace Heartbeat
{
    /**
     * @brief Will periodically send Heartbeats
     * 
     */
class Sender : public Core::Thread
{

    private:
        enum State_e {
            INIT, // This Heartbeat Sender object has been created.
            JOINING_MQ, // attemping to connect to the messaging queue
            BEATING,
            SEND_FAILED,
            SHUTTING_DOWN,
            SHUTDOWN
        };

    public:
        Sender(std::string, std::string);
        Sender(std::string, std::string, std::chrono::milliseconds);
        ~Sender();

    public: // Gets/Sets (boring)
        std::string getMessageQueueName();
        void setMessageQueueName(std::string);
        std::chrono::milliseconds getSendingInterval();
        void setSendingInterval(std::chrono::milliseconds);

    private:
        void _setState(State_e);
        State_e _getState();
        bool _sendBeat();
        
    private: // Core::thread
        void _run() override;

    private: // methods for various states
        std::chrono::milliseconds _init();
        std::chrono::milliseconds _joinMQ();
        std::chrono::milliseconds _beat();
        std::chrono::milliseconds _beatSendFailed();


    private:
        std::mutex _mutex;
        State_e _state;
        std::string _id;
        std::string _messageQueueName;
        int _beatSegment;
        std::chrono::milliseconds _sendingInterval;
        std::shared_ptr<boost::interprocess::message_queue> _pMQ;
    
};
} // namespace HEartbeat

#endif // HEARTBEAT_SENDER_HPP
