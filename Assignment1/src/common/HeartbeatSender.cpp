#include <common/HeartbeatSender.hpp>

namespace Common
{

HeartbeatSender::HeartbeatSender(std::string messageQueueName)
: HeartbeatSender(messageQueueName, boost::chrono::milliseconds(2000))
{ }
HeartbeatSender::HeartbeatSender(std::string messageQueueName, 
                                 boost::chrono::milliseconds sendingInterval)
: _messageQueueName(messageQueueName),
  _sendingInterval(sendingInterval)
{ }



std::string HeartbeatSender::getMessageQueueName()
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _messageQueueName;
}
void HeartbeatSender::setMessageQueueName(std::string messageQueueName)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _messageQueueName = messageQueueName;
}
boost::chrono::milliseconds HeartbeatSender::getSendingInterval()
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _sendingInterval;
}
void HeartbeatSender::getSendingInterval(boost::chrono::milliseconds sendingInterval)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _sendingInterval = sendingInterval;
}
    
} // namespace Common
