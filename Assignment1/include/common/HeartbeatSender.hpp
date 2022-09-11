#ifndef COMMON_HEARTBEATSENDER_HPP
#define COMMON_HEARTBEATSENDER_HPP

#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <mutex>

namespace Common
{
class HeartbeatSender
{
public:
    HeartbeatSender(std::string);
    HeartbeatSender(std::string, boost::chrono::milliseconds);
    ~HeartbeatSender();

public:
    /**
     * @brief calling this will spin off a separate thread to wait
     *        and trigger the heartbeat every `_sendingInterval`
     *        amount of time
     * 
     * @return true Successfully spun off thread to start beating
     * @return false something is wrong boi
     */
    bool beginBeating();

public: // Gets/Sets (boring)
    std::string getMessageQueueName();
    void setMessageQueueName(std::string);
    boost::chrono::milliseconds getSendingInterval();
    void getSendingInterval(boost::chrono::milliseconds);

private:
    std::mutex _mutex;
    std::string _messageQueueName;
    boost::chrono::milliseconds _sendingInterval;
    std::shared_ptr<boost::thread> _pThread;
    
};
} // namespace Common

#endif // COMMON_HEARTBEATSENDER_HPP
