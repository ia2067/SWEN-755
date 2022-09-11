#ifndef COMMON_HEARTBEATSENDER_HPP
#define COMMON_HEARTBEATSENDER_HPP

#include <common/Thread.hpp>

#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <mutex>

namespace Common
{
class HeartbeatSender : public Thread
{

public:
    enum State_e {
        INIT, // This Heartbeat Sender object has been created.
        JOINING_MQ, // attemping to connect to the messaging queue
        BEATING,
        SHUTTING_DOWN,
        SHUTDOWN
    };

public:
    HeartbeatSender(std::string);
    HeartbeatSender(std::string, boost::chrono::milliseconds);
    ~HeartbeatSender();

public: // Gets/Sets (boring)
    std::string getMessageQueueName();
    void setMessageQueueName(std::string);
    boost::chrono::milliseconds getSendingInterval();
    void getSendingInterval(boost::chrono::milliseconds);

private:
    void _setState(State_e);
    State_e _getState();
    
private: // common::thread
    void _run() override;

private: // methods for various states
    boost::chrono::milliseconds _init();
    boost::chrono::milliseconds _joinMQ();
    boost::chrono::milliseconds _beat();


private:
    std::mutex _mutex;
    State_e _state;
    std::string _messageQueueName;
    int _beatSegment;
    boost::chrono::milliseconds _sendingInterval;
    std::shared_ptr<boost::interprocess::message_queue> _pMQ;
    
};
} // namespace Common

#endif // COMMON_HEARTBEATSENDER_HPP