#include <heartbeat/Sender.hpp>
#include <heartbeat/Message.hpp>

#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>

const int NUM_BEAT_SEGMENTS = 10;

namespace bip = boost::interprocess;

namespace Heartbeat
{

//-----------------------------------------------------------------------------
Sender::Sender(std::string id, std::string messageQueueName)
: Sender(id, messageQueueName, std::chrono::milliseconds(2000))
{ }
Sender::Sender(std::string id,
                                 std::string messageQueueName, 
                                 std::chrono::milliseconds sendingInterval)
: _state(INIT),
  _id(id),
  _messageQueueName(messageQueueName),
  _beatSegment(0),
  _sendingInterval(sendingInterval),
  _pMQ(nullptr)
{ }

Sender::~Sender()
{  
    end();
}
//-----------------------------------------------------------------------------
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
std::chrono::milliseconds Sender::getSendingInterval()
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _sendingInterval;
}
void Sender::setSendingInterval(std::chrono::milliseconds sendingInterval)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _sendingInterval = sendingInterval;
}
//-----------------------------------------------------------------------------
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
bool Sender::_sendBeat()
{
    Heartbeat::Message hbm(_id,
                           std::chrono::system_clock::now());

    std::ostringstream oss;
    boost::archive::text_oarchive oa(oss);

    oa << hbm;

    std::string serialized_msg(oss.str());
    char msg[serialized_msg.length()];
    serialized_msg.copy(msg, serialized_msg.length());
    auto currentTimePlus1 = std::chrono::system_clock::now() + std::chrono::seconds(1);
    auto intermediate = std::chrono::system_clock::to_time_t(currentTimePlus1);
    auto timeout = boost::posix_time::from_time_t(intermediate);
    // std::cout << "buh buh" << std::endl;
    
    if(!_pMQ->timed_send(msg, serialized_msg.length(), 1, timeout))
    {
        return false;
    }
    return true;
}

void Sender::_run() 
{
    do
    {
        std::chrono::milliseconds nextSleepTime;
        switch (_getState())
        {
        case INIT:
            nextSleepTime = _init();
            break;
        case JOINING_MQ:
            nextSleepTime = _joinMQ();
            break;
        case BEATING:
            nextSleepTime = _beat();
            break;
        case SEND_FAILED:
            nextSleepTime = _beatSendFailed();
        case SHUTTING_DOWN:
            continue;
        default:
            break;
        }
        std::this_thread::sleep_for(nextSleepTime);
    } while (!_getShutdown());

    _setState(SHUTDOWN);
}

std::chrono::milliseconds Sender::_init()
{
    _setState(JOINING_MQ);
    return std::chrono::milliseconds(0);
}
std::chrono::milliseconds Sender::_joinMQ()
{

    try
    {
        _pMQ = std::make_shared<bip::message_queue>(bip::open_only,
                                                    getMessageQueueName().c_str());
    }
    catch(const std::exception& e)
    {
        // std::cerr << e.what() << '\n';
        // try again in 1/4 of a second
        return std::chrono::milliseconds(250);
    }
    
    // Sucess!
    _setState(BEATING);
    // little pause before first beats
    return std::chrono::milliseconds(100);
}
std::chrono::milliseconds Sender::_beat()
{
    if(_beatSegment >= NUM_BEAT_SEGMENTS)
    {
        _beatSegment = 0;
        //
        // do something to check we are alive
        //
        if(!_sendBeat())
        {
            _setState(SEND_FAILED);
            return std::chrono::milliseconds(0);
        }
    }
    else
    {
        _beatSegment++;
    }

    // dont want to be 'sleeping' when we are told to shutdown,
    // so lets break the interval up into 10 bits
    return getSendingInterval() / NUM_BEAT_SEGMENTS;
}
std::chrono::milliseconds Sender::_beatSendFailed()
{
    if(!_sendBeat())
    {
        return std::chrono::milliseconds(100);
    }

    // Success!
    _setState(BEATING);
    return getSendingInterval() / NUM_BEAT_SEGMENTS;
}
    
} // namespace Heartbeat
