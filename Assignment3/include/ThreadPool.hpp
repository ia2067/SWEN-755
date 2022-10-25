#ifndef ASSIGNMENT3_THREADPOOL_HPP
#define ASSIGNMENT3_THREADPOOL_HPP

#include <WorkerThread.hpp>
#include <Scheduler.hpp>
#include <Command.hpp>
#include <queue>
#include <vector>


namespace Assignment3
{
class ThreadPool : public Core::Thread 
{

public:
    enum State_e {
        INIT,
        READY,
        // TODO: States?
        SHUTDOWN
    };

public:
    ThreadPool(int num_threads, std::shared_ptr<Scheduler> scheduler);
    virtual ~ThreadPool() = default;

private: //core::thread
    void _run() override;

private: //State functions
    std::chrono::milliseconds _state_init(); 
    std::chrono::milliseconds _state_ready();
    // TODO: States?


private: // members
    std::mutex _mutex;

public: //Command Operations
    void addCommand(std::shared_ptr<Command> cmd, Priority::Priority_e pri);
    std::vector<std::shared_ptr<Command>> getFinishedCommands();
    int getNumberFinishedCmd();
    int getNumberUnfinishedCmd();
    
private: // Thread Pooling management
    int _numThreads;
    int _outstandingCmds;
    std::vector<std::shared_ptr<Command>> _finishedCommands;
    std::vector<std::shared_ptr<WorkerThread>> _threads;
    std::shared_ptr<Scheduler> _scheduler;

public: // public state management
    State_e getState();
private: // state managment
    State_e _state;
    void _setState(State_e);

};

// Useful global function
std::ostream& operator<<(std::ostream& os, const ThreadPool::State_e& state);

} // namespace Assignment3


#endif