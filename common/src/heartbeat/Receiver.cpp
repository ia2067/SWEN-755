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
  _messageQueueName(messageQueueName),
  _checkInterval(checkInterval),
  _expiredInterval(expiredInterval)
{ }
Receiver::~Receiver()
{  
    end();
}
//-----------------------------------------------------------------------------
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
Receiver::State_e Receiver::_getState()
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
    return std::chrono::milliseconds(0);
}
std::chrono::milliseconds Receiver::_createMQ()
{
    size_t max_num_msgs = 100;
    size_t bufSize  = 2 * sizeof(Heartbeat::Message);

    try
    {
        _pMQ = std::make_shared<bip::message_queue>(bip::create_only,
                                                    getMessageQueueName().c_str(),
                                                    max_num_msgs,
                                                    bufSize);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return std::chrono::milliseconds(250);
    }

    _setState(LISTENING);
    return std::chrono::milliseconds(0);

}
std::chrono::milliseconds Receiver::_listen()
{
    bool msgsRemain = true;
    while(_pMQ && msgsRemain)
    {
        size_t bufSize = 2 * sizeof(Heartbeat::Message); 
        char buf[bufSize];
        size_t rcvdSize;
        uint pri;

        Heartbeat::Message rcvd_hbm;

        if(_pMQ->try_receive(buf, bufSize, rcvdSize, pri))
        {
            std::istringstream iss(std::string(buf,bufSize));
            boost::archive::text_iarchive ia(iss);

            ia >> rcvd_hbm;

            std::string id = rcvd_hbm.getId();
            std::chrono::system_clock::time_point tp = rcvd_hbm.getBeatTime();
            _lastBeats[id] = tp;
        }
        else
        {
            msgsRemain = false;
        }
    }

    _setState(CHECK_PULSES);
    return std::chrono::milliseconds(0);
}

std::chrono::milliseconds Receiver::_checkPulses()
{
    for(auto beatPair : _lastBeats)
    {
        bool isDead = false;
        std::string id = beatPair.first;

        std::chrono::system_clock::duration diff = std::chrono::system_clock::now() - beatPair.second;
        double diff_ms = diff.count() / 1e6;
        double expiredInterval_ms = getExpiredInterval().count();

        isDead = (diff_ms > expiredInterval_ms);


        if(isDead)
        {

            _deadIds.insert(id);
        }
        else
        {
            // std::cout << "erasing dead: " << id << std::endl;
            _deadIds.erase(id);
        }
    }
    _setState(LISTENING);
    return getCheckInterval();
}
} // namespace Common
