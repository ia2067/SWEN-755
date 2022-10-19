#include <FaultMonitor.hpp>

namespace Assignment2
{

FaultMonitor::FaultMonitor(std::string heartbeatRcvrMQN, std::string primaryMQN, std::string secondaryMQN)
: _primaryDead(false),
  _secondaryDead(false),
  _state(INIT),
  _heartbeatRcvrMQN(heartbeatRcvrMQN),
  _primaryMQN(primaryMQN),
  _secondaryMQN(secondaryMQN),
  _pHBR(nullptr),
  _pPrimary(nullptr),
  _pSecondary(nullptr)
{ }


void FaultMonitor::_run()
{
    do
    {
        std::chrono::milliseconds nextSleepTime;
        switch(getState())
        {
        case INIT:
            nextSleepTime = _init();
            break;
        case WAKING_PRIMARY:
            nextSleepTime = _wakingPrimary();
            break;
        case RUNNING_PRIMARY:
            nextSleepTime = _runningPrimary();
            break;
        case WAKING_SECONDARY:
            nextSleepTime = _wakingSecondary();
            break;
        case RUNNING_SECONDARY:
            nextSleepTime = _runningSecondary();
            break;
        case ALL_DEAD:
            nextSleepTime = _allDead();
            break;
        default:
            break;
        }
        std::this_thread::sleep_for(nextSleepTime);
    } while (!_getShutdown());

    _setState(SHUTDOWN);
}

std::chrono::milliseconds FaultMonitor::_init()
{
    _pHBR = std::make_shared<Heartbeat::Receiver>(_heartbeatRcvrMQN,
                                                  std::chrono::milliseconds(100),
                                                  std::chrono::milliseconds(3000));

    _pPrimary = std::make_shared<FaultHandle::Client>(_primaryMQN);
    _pSecondary = std::make_shared<FaultHandle::Client>(_secondaryMQN);

    _pHBR->start();
    _pHBR->sigNewDead.connect(boost::bind(&FaultMonitor::_handleNewDead, 
                                          this, 
                                          boost::placeholders::_1));

    _setState(WAKING_PRIMARY);
    return std::chrono::milliseconds(1000);
}
std::chrono::milliseconds FaultMonitor::_wakingPrimary() 
{
    FaultHandle::Message wakeup_primary(FaultHandle::MessageType_e::CMD_WAKEUP,{});
    if(_pPrimary->query(wakeup_primary))
    {
        std::cout << "PRIMARY IS AWAKE!" << std::endl;
        _cachePrimaryDead(false);
        _setState(RUNNING_PRIMARY);
    }
    else
        std::cout << "PRIMARY DIDNT WAKE UP AWAKE!" << std::endl;
    return std::chrono::milliseconds(200);
}
std::chrono::milliseconds FaultMonitor::_runningPrimary() 
{
    if(_getPrimaryDead())
    {
        _setState(WAKING_SECONDARY);
        return std::chrono::milliseconds(100);
    }

    FaultHandle::Message get_data_cmd(FaultHandle::MessageType_e::CMD_GETDATA,{});
    if(_pPrimary->query(get_data_cmd))
    {
        std::cout << "Got Data(primary): ";
        for(auto it : get_data_cmd.getSensorData())
        {
            std::cout << it << ", " << std::flush;
        }
        std::cout << std::endl;
    }

    return std::chrono::milliseconds(1000);
}
std::chrono::milliseconds FaultMonitor::_wakingSecondary() 
{
    FaultHandle::Message wake_up_secondary(FaultHandle::MessageType_e::CMD_WAKEUP,{});
    if(_pSecondary->query(wake_up_secondary))
    {
        std::cout << "SECONDARY IS AWAKE!" << std::endl;
        _cacheSecondaryDead(false);
        _setState(RUNNING_SECONDARY);
    }
    else
        std::cout << "SECONDARY DIDNT WAKE UP AWAKE!" << std::endl;

    return std::chrono::milliseconds(200);
}
std::chrono::milliseconds FaultMonitor::_runningSecondary() 
{
    // Attempt to wake up the primary (if not) so it can be targeted again. 
    if(_getPrimaryDead())
    {
        // Reset alive count to track how long it's been alive.
        _primaryAliveCnt = 0;

        FaultHandle::Message wakeup_primary(FaultHandle::MessageType_e::CMD_WAKEUP,{});
        if(_pPrimary->query(wakeup_primary))
        {
            std::cout << "PRIMARY IS AWAKE AGAIN!" << std::endl;
            _cachePrimaryDead(false);
        }
        else 
        {
            std::cout << "PRIMARY DIDN'T WAKE UP!" << std::endl;
        }
    }
    else
    {
        // Update primary alive counter to determine when to switch over.
        _primaryAliveCnt++;
    }

    if(_getSecondaryDead())
    {
        _setState(WAKING_PRIMARY);
        return std::chrono::milliseconds(100);
    }

    FaultHandle::Message get_data_cmd(FaultHandle::MessageType_e::CMD_GETDATA,{});
    if(_pSecondary->query(get_data_cmd))
    {
        std::cout << "Got Data(secondary): ";
        for(auto it : get_data_cmd.getSensorData())
        {
            std::cout << it << ", " << std::flush;
        }
        std::cout << std::endl;
    }

    // If primary sensor has been awake long enough, then time to switch back to it.
    if (_primaryAliveCnt > FaultMonitor::PRI_NUM_CYCLES_ALIVE)
    {
        std::cout << "Switching back to primary." << std::endl;
        _setState(WAKING_PRIMARY);
    }

    return std::chrono::milliseconds(1000);
}
std::chrono::milliseconds FaultMonitor::_allDead()
{
    std::chrono::system_clock::time_point current_time = std::chrono::system_clock::now();
    std::time_t tt = std::chrono::system_clock::to_time_t(current_time);
    std::cout << "------------------------------------" << std::endl;
    std::cout << "current time: " << std::ctime(&tt);
    std::cout << "------------------------------------" << std::endl;
    for(auto dead : _pHBR->deadIds())
    {
        std::cout << "        DEAD: " << dead << std::endl;
        std::chrono::system_clock::time_point tp(_pHBR->getLastBeat(dead));
        std::time_t tt = std::chrono::system_clock::to_time_t(tp);
        std::cout << "  last comms: " << std::ctime(&tt);
        std::cout << "------------------------------------" << std::endl;
    }

    _setState(SHUTDOWN);
    return std::chrono::milliseconds(1000);
}

void FaultMonitor::_handleNewDead(std::string id)
{
    if (id == "primarySensor")
    {
        if(!_getPrimaryDead())
            std::cout << "PRIMARY IS DEAD!" << std::endl;
        _cachePrimaryDead(true);
        
    }
    if(id == "secondarySensor")
    {
        if(!_getSecondaryDead())
            std::cout << "SECONDARY IS DEAD!" << std::endl;
        _cacheSecondaryDead(true);
    }
}
bool FaultMonitor::_getPrimaryDead()
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _primaryDead;
}
bool FaultMonitor::_getSecondaryDead()
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _secondaryDead;
}
void FaultMonitor::_cachePrimaryDead(bool isDead)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _primaryDead = isDead;
}
void FaultMonitor::_cacheSecondaryDead(bool isDead)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _secondaryDead = isDead;
}
FaultMonitor::State_e FaultMonitor::getState()
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _state;
}
void FaultMonitor::_setState(State_e state)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _state = state;
}
} // namespace Assignment2