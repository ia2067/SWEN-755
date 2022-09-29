#include <sync/Receiver.hpp>
#include <sync/Message.hpp>
#include <iostream>

namespace bip = boost::interprocess;

namespace Sync
{
    Receiver::Receiver(std::string messageQueueName)
    : _state(INIT),
      _messageQueueName(messageQueueName)
    { }

    Receiver::~Receiver()
    {
        end();
    }

    std::string Receiver::getMessageQueueName()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        return _messageQueueName;
    }

    void Receiver::setMessageQueueName(std::string messageQueueName)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _messageQueueName = messageQueueName;
    }

    void Receiver::_setState(State_e state)
    {
        // std::cout << "HBR::setState: " << state << std::endl;
        std::lock_guard<std::mutex> lock(_mutex);
        _state = state;
    }

    Receiver::State_e Receiver::_getState()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        return _state;
    }

    void Receiver::_run()
    {
        _setState(INIT);
    }
}