#include <heartbeat/Receiver.hpp>
#include <heartbeat/Message.hpp>
#include <iostream>

namespace bip = boost::interprocess;

namespace Heartbeat
{
Receiver::Receiver(std::string messageQueueName, 
                   std::chrono::milliseconds checkInterval,
                   std::chrono::milliseconds expiredInterval)
: _state(INIT),
  _checkInterval(checkInterval),
  _expiredInterval(expiredInterval),
  _pMQ(std::make_shared<Core::MessageQueue<Message>>(messageQueueName, true))
{ }
Receiver::~Receiver()
{  
    _pMQ->disconnect();
    end();
}
//-----------------------------------------------------------------------------
std::string Receiver::getMessageQueueName()
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _pMQ->getMessageQueueName();
}
std::chrono::milliseconds Receiver::getExpiredInterval()
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _expiredInterval;
}
void Receiver::setExpiredInterval(std::chrono::milliseconds expiredInterval)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _expiredInterval = expiredInterval;
}
std::chrono::milliseconds Receiver::getCheckInterval()
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _checkInterval;
}
void Receiver::setCheckInterval(std::chrono::milliseconds checkInterval)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _checkInterval = checkInterval;
}
//-----------------------------------------------------------------------------
void Receiver::_setState(State_e state)
{
    // std::cout << "HBR::setState: " << state << std::endl;
    std::lock_guard<std::mutex> lock(_mutex);
    _state = state;
}
Receiver::State_e Receiver::getState()
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _state;
}
//-----------------------------------------------------------------------------
void Receiver::addSenderId(std::string newSenderId)
{
    typedef std::pair<std::string, std::chrono::system_clock::time_point> beatPair_t;
    // Time of zero to show its OLD (aka no beats yet)
    std::chrono::system_clock::time_point time_zero(std::chrono::milliseconds(0));
    beatPair_t bp(newSenderId, time_zero);

    // add pair to last beat map
    _lastBeats.insert(bp);
}

std::set<std::string> Receiver::deadIds()
{
    return _deadIds;
}
std::chrono::system_clock::time_point Receiver::getLastBeat(std::string id)
{
    return _lastBeats.at(id);
}

void Receiver::_run()
{
    do
    {
        std::chrono::milliseconds nextSleepTime;
        switch (getState())
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
        case CHECK_PULSES:
            nextSleepTime = _checkPulses();
        case SHUTTING_DOWN:
            continue;
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
        Heartbeat::Message hbm;
        if(_pMQ->recvMessage(hbm, msgsRemain))
        {
            std::string id = hbm.getId();
            std::chrono::system_clock::time_point tp = hbm.getBeatTime();

            if(id.length() != 0)
            {
                // std::cout << "recording beat for " << id << std::endl;
                _lastBeats[id] = tp;
            }
        }
    }

    _setState(CHECK_PULSES);
    return std::chrono::milliseconds(100);
}

std::chrono::milliseconds Receiver::_checkPulses()
{
    for(auto beatPair : _lastBeats)
    {
        bool isDead = false;
        std::string id = beatPair.first;

        using clock = std::chrono::system_clock;
        clock::time_point nowp = clock::now();
        clock::time_point lastAliveBeat = nowp - getExpiredInterval(); 

        isDead = (lastAliveBeat > beatPair.second);

        if(isDead && _potentiallyDeadIds.count(id) == 0)
        {
            // std::cout << id << "is potentially dead..." << std::endl;
            _potentiallyDeadIds.emplace(id);
        }
        else if (isDead && _potentiallyDeadIds.count(id) > 0 && _deadIds.count(id) == 0)
        {
            // std::cout << id << "is dead..." << std::endl;
            _deadIds.emplace(id);
            // std::cout << "signaling dead: " << id << std::endl;
            sigNewDead(id); //signals up that someone died!
        }
        else if (!isDead && _deadIds.count(id) > 0)
        {
            // std::cout << "erasing dead: " << id << std::endl;
            _potentiallyDeadIds.erase(id);
            _deadIds.erase(id);
        }
    }
    _setState(LISTENING);
    return getCheckInterval();
}
} // namespace Heartbeat
