#include <sync/Sender.hpp>
#include <sync/Message.hpp>

#include <iostream>

const int NUM_SYNC_SEGMENTS = 10;
namespace bip = boost::interprocess;

namespace Sync
{
    Sender::Sender(std::string messageQueueName)
    : _state(INIT),
      _pMQ(std::make_shared<Core::MessageQueue<Message>>(messageQueueName, false))
    { }

    Sender::~Sender()
    {
        _pMQ->disconnect();
        end();
    }

    std::string Sender::getMessageQueueName()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        return _pMQ->getMessageQueueName();
    }

    std::chrono::milliseconds Sender::getSendingInterval()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        return _sendingInterval;
    }

    void Sender::_setState(State_e state)
    {
        //std::cout << "Sync::State: " << state << std::endl;
        std::lock_guard<std::mutex> lock(_mutex);
        _state = state;
    }

    Sender::State_e Sender::getState()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        return _state;
    }

    bool Sender::_sendSync()
    {
        Sync::Message syncm(_cacheList);
        
        if(!_pMQ->sendMessage(syncm))
        {
            return false;
        }
        return true;
    }

    void Sender::cacheValues(std::list<int> cacheValues)
    {
        //std::cout << "CACHE VALUES" << std::endl;
        _cacheList = cacheValues;
    }

    void Sender::_run()
    {
        do 
        {
            std::chrono::milliseconds nextSleepTime;
            switch(getState())
            {
            case INIT:
                nextSleepTime = _init();
                break;
            case JOINING_MQ:
                nextSleepTime = _joinMQ();
                break;
            case IDLE:
                nextSleepTime = _idle();
                break;
            case SENDING:
                nextSleepTime = _sending();
                break;
            default:
                break;
            }
            std::this_thread::sleep_for(nextSleepTime);
        } while(!_getShutdown());

        _setState(SHUTDOWN);
    }

    std::chrono::milliseconds Sender::_init()
    {
        _setState(JOINING_MQ);
        return std::chrono::milliseconds(0);
    }

    std::chrono::milliseconds Sender::_joinMQ()
    {
        if(!_pMQ->connect())
            return std::chrono::milliseconds(250);

        // Success!
        _setState(IDLE);
        // little pause before first beats
        return std::chrono::milliseconds(100);
    }

    std::chrono::milliseconds Sender::_idle()
    {
        if(_syncSegment >= NUM_SYNC_SEGMENTS)
        {
            //std::cout << "SEND SYNC MESSAGE" << std::endl;
            _syncSegment = 0;
            _setState(SENDING);
            return std::chrono::milliseconds(0);
        }
        else
        {
            _syncSegment++;
        }

        // dont want to be 'sleeping' when we are told to shutdown,
        // so lets break the interval up into 10 bits
        return getSendingInterval() / NUM_SYNC_SEGMENTS;
    }

    std::chrono::milliseconds Sender::_sending()
    {
        _sendSync();
        _setState(IDLE);
        return std::chrono::milliseconds(0);
    }
} // namespace Sync