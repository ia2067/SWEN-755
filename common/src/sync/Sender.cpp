#include <sync/Sender.hpp>
#include <sync/Message.hpp>

#include <iostream>
namespace bip = boost::interprocess;

namespace Sync
{
    Sender::Sender(std::string id, std::string messageQueueName)
    : _state(INIT),
      _id(id),
      _messageQueueName(messageQueueName),
      _pMQ(nullptr)
    { }

    Sender::~Sender()
    {
        end();
    }

    std::string Sender::getMessageQueueName()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        return _messageQueueName;
    }

    void Sender::setMessageQueueName(std::string messageQueueName)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _messageQueueName = messageQueueName;
    }

    void Sender::_setState(State_e state)
    {
        // std::cout << "HBS::State: " << state << std::endl;
        std::lock_guard<std::mutex> lock(_mutex);
        _state = state;
    }

    Sender::State_e Sender::_getState()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        return _state;
    }

    bool Sender::_sendSync()
    {
        return true;
    }

    void Sender::_run()
    {
        _setState(IDLE);
    }
} // namespace Sync