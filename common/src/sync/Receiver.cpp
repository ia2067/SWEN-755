#include <sync/Receiver.hpp>
#include <sync/Message.hpp>
#include <iostream>

namespace bip = boost::interprocess;

namespace Sync
{
    Receiver::Receiver(std::string messageQueueName)
    : _state(INIT),
      _pMQ(std::make_shared<Core::MessageQueue<Message>>(messageQueueName, true))
    { }

    Receiver::~Receiver()
    {
        _pMQ->disconnect();
        end();
    }

    std::string Receiver::getMessageQueueName()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        return _pMQ->getMessageQueueName();
    }

    void Receiver::_setState(State_e state)
    {
        // std::cout << "SyncRx::setState: " << state << std::endl;
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
        do
        {
            std::chrono::milliseconds nextSleepTime;
            switch (_getState())
            {
            case INIT:
                nextSleepTime = _init();
                break;
            case CREATING_MQ:
                nextSleepTime = _createMQ();
                break;
            case LISTENING:
                nextSleepTime = _listen();
                break;
            case CHECK_VALUES:
                nextSleepTime = _checkValues();
            default:
                break;
            }
            std::this_thread::sleep_for(nextSleepTime);
        } while (!_getShutdown());

        _setState(SHUTDOWN);        
    }

    std::chrono::milliseconds Receiver::_init()
    {
        _setState(CREATING_MQ);
        return std::chrono::milliseconds(100);
    }

    std::chrono::milliseconds Receiver::_createMQ()
    {
        if(!_pMQ->connect())
            return std::chrono::milliseconds(250);
            

        _setState(LISTENING);
        return std::chrono::milliseconds(100);

    }

    std::chrono::milliseconds Receiver::_listen()
    {
        bool msgsRemain = true;
        while(msgsRemain)
        {
            Sync::Message syncm;
            if(_pMQ->recvMessage(syncm, msgsRemain))
            {
                // std::string id = hbm.getId();
                // std::chrono::system_clock::time_point tp = hbm.getBeatTime();
                // _lastBeats[id] = tp;
                _cacheRxValues = syncm.getVals();
            }
        }

        _setState(CHECK_VALUES);
        return std::chrono::milliseconds(100);
    }

    std::chrono::milliseconds Receiver::_checkValues()
    {
        // Need to bubble up the values to the 'owner'
        _setState(LISTENING);
        return std::chrono::milliseconds(100);
    }
}