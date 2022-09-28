#include <HeartbeatReceiver.hpp>
#include <HeartbeatMessage.hpp>
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
    // std::cout << "HBR::setState: " << state << std::endl;
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

std::set<std::string> HeartbeatReceiver::deadIds()
{
    return _deadIds;
}
bc::system_clock::time_point HeartbeatReceiver::getLastBeat(std::string id)
{
    return _lastBeats.at(id);
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
    return bc::milliseconds(0);

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

            std::string id = rcvd_hbm.getId();
            bc::system_clock::time_point tp = rcvd_hbm.getBeatTime();
            _lastBeats[id] = tp;
        }
        else
        {
            msgsRemain = false;
        }
    }

    _setState(CHECK_PULSES);
    return bc::milliseconds(0);
}

boost::chrono::milliseconds HeartbeatReceiver::_checkPulses()
{
    for(auto beatPair : _lastBeats)
    {
        bool isDead = false;
        std::string id = beatPair.first;

        bc::system_clock::duration diff = bc::system_clock::now() - beatPair.second;
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
