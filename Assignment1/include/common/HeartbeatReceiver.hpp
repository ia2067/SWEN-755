#ifndef COMMON_HEARTBEATRECEIVER_HPP
#define COMMON_HEARTBEATRECEIVER_HPP

#include <common/Thread.hpp>

#include <boost/chrono.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>

#include <map>
#include <vector>
#include <string>
#include <mutex>

namespace Common
{
    class HeartbeatReceiver : public Thread
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
        HeartbeatReceiver(std::string, 
                         boost::chrono::milliseconds = boost::chrono::milliseconds(2500),
                         boost::chrono::milliseconds = boost::chrono::milliseconds(5000));
        ~HeartbeatReceiver();
        
    public: // Gets/Sets (boring)
        std::string getMessageQueueName();
        void setMessageQueueName(std::string);
        boost::chrono::milliseconds getExpiredInterval();
        void setExpiredInterval(boost::chrono::milliseconds);
        boost::chrono::milliseconds getCheckInterval();
        void setCheckInterval(boost::chrono::milliseconds);
    
    public:
        void addSenderId(std::string);
        bool isAlive(std::string);
        std::list<std::string> deadIds();

    private:
        void _setState(State_e);
        State_e _getState();

    private: //Common::Thread
        void _run() override;

    private: // methods for states
        boost::chrono::milliseconds _init();
        boost::chrono::milliseconds _createMQ();
        boost::chrono::milliseconds _listen();
        boost::chrono::milliseconds _checkPulses();

    private:
        std::mutex _mutex;
        State_e _state;
        std::string _messageQueueName;
        boost::chrono::milliseconds _checkInterval;
        boost::chrono::milliseconds _expiredInterval;
        std::map<std::string, boost::chrono::system_clock::time_point> _lastBeats;
        std::map<std::string, boost::chrono::milliseconds> _deadIds; // ms is time since last beat
        std::shared_ptr<boost::interprocess::message_queue> _pMQ;
    };
} // namespace Common





#endif