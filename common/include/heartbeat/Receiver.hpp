#ifndef HEARTBEAT_RECEIVER_HPP
#define HEARTBEAT_RECEIVER_HPP

#include <core/Thread.hpp>

#include <chrono>
#include <boost/interprocess/ipc/message_queue.hpp>

#include <set>
#include <map>
#include <vector>
#include <string>
#include <mutex>

namespace Heartbeat
{
    /**
     * @brief A Heartbeat::Receiver which keeps track of alive/dead Heartbeat::Sender (s)
     * 
     */
    class Receiver : public Core::Thread
    {
    private:
        enum State_e {
            INIT, // This Heartbeat Sender object has been created.
            CREATING_MQ, // attemping to connect to the messaging queue
            LISTENING,
            CHECK_PULSES,
            SHUTTING_DOWN,
            SHUTDOWN
        };

    public:
        Receiver(std::string, 
                 std::chrono::milliseconds = std::chrono::milliseconds(2500),
                 std::chrono::milliseconds = std::chrono::milliseconds(5000));
        ~Receiver();
        
    public: // Gets/Sets (boring)
        std::string getMessageQueueName();
        void setMessageQueueName(std::string);
        std::chrono::milliseconds getExpiredInterval();
        void setExpiredInterval(std::chrono::milliseconds);
        std::chrono::milliseconds getCheckInterval();
        void setCheckInterval(std::chrono::milliseconds);
    
    public:
        void addSenderId(std::string);
        std::set<std::string> deadIds();
        std::chrono::system_clock::time_point getLastBeat(std::string);
        

    private:
        void _setState(State_e);
        State_e _getState();

    private: //Core::Thread
        void _run() override;

    private: // methods for states
        std::chrono::milliseconds _init();
        std::chrono::milliseconds _createMQ();
        std::chrono::milliseconds _listen();
        std::chrono::milliseconds _checkPulses();

    private:
        std::mutex _mutex;
        State_e _state;
        std::string _messageQueueName;
        std::chrono::milliseconds _checkInterval;
        std::chrono::milliseconds _expiredInterval;
        std::map<std::string, std::chrono::system_clock::time_point> _lastBeats;
        std::set<std::string> _deadIds;
        std::shared_ptr<boost::interprocess::message_queue> _pMQ;
    };
} // namespace Heartbeat





#endif