#ifndef ASSIGNMENT3_THREADPOOL_HPP
#define ASSIGNMENT3_THREADPOOL_HPP

#include <WorkerThread.hpp>
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

    enum Priority_e {
        LOW,
        HIGH
    };

public:
    ThreadPool(int num_threads);
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
    void addCommand(Command cmd, Priority_e pri);
    
private:
    int _numThreads;
    std::queue<Command> _lowPriorityQueue;
    std::queue<Command> _highPriorityQueue;
    std::vector<WorkerThread> _threads;

public: // public state management
    State_e getState();
private: // state managment
    State_e _state;
    void _setState(State_e);

};
} // namespace Assignment3


#endif