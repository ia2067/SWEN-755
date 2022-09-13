#include <common/HeartbeatReceiver.hpp>
#include <common/HeartbeatMessage.hpp>
#include <iostream>

namespace bip = boost::interprocess;
namespace bc = boost::chrono;

namespace Common
{
HeartbeatReceiver::HeartbeatReceiver(std::string messageQueueName, 
                                     bc::milliseconds checkInterval,
                                     bc::milliseconds expiredInterval)
: _state(INIT),
  _messageQueueName(messageQueueName),
  _checkInterval(checkInterval),
  _expiredInterval(expiredInterval)
{ }
HeartbeatReceiver::~HeartbeatReceiver()
{  
    end();
}
//-----------------------------------------------------------------------------
std::string HeartbeatReceiver::getMessageQueueName()
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _messageQueueName;
}
void HeartbeatReceiver::setMessageQueueName(std::string messageQueueName)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _messageQueueName = messageQueueName;
}
bc::milliseconds HeartbeatReceiver::getExpiredInterval()
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _expiredInterval;
}
void HeartbeatReceiver::setExpiredInterval(bc::milliseconds expiredInterval)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _expiredInterval = expiredInterval;
}
bc::milliseconds HeartbeatReceiver::getCheckInterval()
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _checkInterval;
}
void HeartbeatReceiver::setCheckInterval(bc::milliseconds checkInterval)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _checkInterval = checkInterval;
}
//-----------------------------------------------------------------------------
void HeartbeatReceiver::_setState(State_e state)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _state = state;
}
HeartbeatReceiver::State_e HeartbeatReceiver::_getState()
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _state;
}
//-----------------------------------------------------------------------------
void HeartbeatReceiver::addSenderId(std::string newSenderId)
{
    typedef std::pair<std::string, bc::system_clock::time_point> beatPair_t;
    // Time of zero to show its OLD (aka no beats yet)
    bc::system_clock::time_point time_zero(bc::milliseconds(0));
    beatPair_t bp(newSenderId, time_zero);

    // add pair to last beat map
    _lastBeats.insert(bp);
}

bool HeartbeatReceiver::isAlive(std::string id)
{
    return (_deadIds.count(id) == 0);
}
std::list<std::string> HeartbeatReceiver::deadIds()
{
    std::list<std::string> theDeadOnes;
    for(auto deadPairs : _deadIds)
    {
        theDeadOnes.push_back(deadPairs.first);
    }
    return theDeadOnes;
}


void HeartbeatReceiver::_run()
{
    do
    {
        bc::milliseconds nextSleepTime;
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
        boost::this_thread::sleep_for(nextSleepTime);
    } while (!_getShutdown());

    _setState(SHUTDOWN);
}

boost::chrono::milliseconds HeartbeatReceiver::_init()
{
    _setState(CREATING_MQ);
    return bc::milliseconds(0);
}
boost::chrono::milliseconds HeartbeatReceiver::_createMQ()
{
    size_t max_num_msgs = 100;
    size_t bufSize  = 2 * sizeof(Common::HeartbeatMessage);

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
        return bc::milliseconds(250);
    }

    _setState(LISTENING);
    return bc::milliseconds(100);

}
boost::chrono::milliseconds HeartbeatReceiver::_listen()
{
    bool msgsRemain = true;
    while(_pMQ && msgsRemain)
    {
        size_t bufSize = 2 * sizeof(Common::HeartbeatMessage); 
        char buf[bufSize];
        size_t rcvdSize;
        uint pri;

        Common::HeartbeatMessage rcvd_hbm;

        if(_pMQ->try_receive(buf, bufSize, rcvdSize, pri))
        {
            std::istringstream iss(std::string(buf,bufSize));
            boost::archive::text_iarchive ia(iss);

            ia >> rcvd_hbm;

            std::time_t t = bc::system_clock::to_time_t(rcvd_hbm.getBeatTime());

            // std::cout << "Got beat for " << rcvd_hbm.getId() << " @ " << std::ctime(&t) << std::endl;
            _lastBeats[rcvd_hbm.getId()] = rcvd_hbm.getBeatTime();
        }
        else
        {
            msgsRemain = false;
        }
    }

    _setState(CHECK_PULSES);
    return getCheckInterval();
}

boost::chrono::milliseconds HeartbeatReceiver::_checkPulses()
{
    for(auto beatPair : _lastBeats)
    {
        std::string id = beatPair.first;
        bc::system_clock::time_point lb = beatPair.second;

        bc::system_clock::time_point curr_time = bc::system_clock::now();
        bc::system_clock::duration diff = curr_time - lb;
        bc::milliseconds diff_ms = bc::duration_cast<bc::milliseconds>(diff);
        if(diff_ms > getExpiredInterval())
        {
            // std::cout << "DEAD: " << id << " | " << bc::duration_cast<bc::seconds>(diff) << std::endl;
            _deadIds[id] = diff_ms;
        }
    }
    _setState(LISTENING);
    return bc::milliseconds(0);
}
} // namespace Common
