#ifndef ASSIGNMENT2_FAULTMONITOR_HPP
#define ASSIGNMENT2_FAULTMONITOR_HPP

#include <memory> // shared_ptr
#include <string>
#include <mutex>
#include <core/Thread.hpp>

#include <heartbeat/Receiver.hpp>
#include <faulthandle/Client.hpp>



namespace Assignment2
{
class FaultMonitor : public Core::Thread 
{
public:
    enum State_e {
        INIT,
        WAKING_PRIMARY,
        RUNNING_PRIMARY,
        WAKING_SECONDARY,
        RUNNING_SECONDARY,
        ALL_DEAD,
        SHUTDOWN
    };

public:
    FaultMonitor(std::string, std::string, std::string);
    virtual ~FaultMonitor() = default;

private: //core::thread
    void _run() override;

private: //State functions
    std::chrono::milliseconds _init(); 
    std::chrono::milliseconds _wakingPrimary();
    std::chrono::milliseconds _runningPrimary();
    std::chrono::milliseconds _wakingSecondary();
    std::chrono::milliseconds _runningSecondary();
    std::chrono::milliseconds _allDead();

private: //signal handlers
    void _handleNewDead(std::string);

private: // cached members
    std::mutex _mutex;
    bool _primaryDead;
    bool _secondaryDead;

private: //cached members set/gets
    bool _getPrimaryDead();
    bool _getSecondaryDead();
    void _cachePrimaryDead(bool);
    void _cacheSecondaryDead(bool);
public: // public state management
    State_e getState();
private: // state managment
    State_e _state;
    void _setState(State_e);

private: // Message Queue Names
    std::string _heartbeatRcvrMQN;
    std::string _primaryMQN;
    std::string _secondaryMQN;

private: // ptrs
    std::shared_ptr<Heartbeat::Receiver> _pHBR;
    std::shared_ptr<FaultHandle::Client> _pPrimary;
    std::shared_ptr<FaultHandle::Client> _pSecondary;
};
} // namespace Assignment2


#endif