#ifndef HEARTBEAT_SENDER_HPP
#define HEARTBEAT_SENDER_HPP

#include <core/Thread.hpp>
#include <core/MessageQueue.hpp>

#include <chrono>

#include <mutex>

namespace Heartbeat
{
    class Message;

    /**
     * @brief Will periodically send Heartbeats
     * 
     */
class Sender : public Core::Thread
{

    public:
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
        std::chrono::milliseconds getSendingInterval();
        void setSendingInterval(std::chrono::milliseconds);
        State_e getState();
        void pauseBeat();
        void resumeBeat();

    private:
        void _setState(State_e);
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
        int _beatSegment;
        std::chrono::milliseconds _sendingInterval;
        std::shared_ptr<Core::MessageQueue<Message>> _pMQ;
    
};
} // namespace HEartbeat

#endif // HEARTBEAT_SENDER_HPP
