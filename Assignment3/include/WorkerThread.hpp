#ifndef ASSIGNMENT3_WORKERTHREAD_HPP
#define ASSIGNMENT3_WORKERTHREAD_HPP

#include <mutex>
#include <core/Thread.hpp>
#include <Command.hpp>

namespace Assignment3
{
class WorkerThread : public Core::Thread 
{
static int NEXT_THREAD_ID;

public:
    enum State_e {
        INIT,
        READY,
        EXECUTE,
        DONE,
        SHUTDOWN
    };

public:
    WorkerThread();
    virtual ~WorkerThread() = default;

private: //core::thread
    void _run() override;

private: //State functions
    std::chrono::milliseconds _state_init(); 
    std::chrono::milliseconds _state_ready();
    std::chrono::milliseconds _state_execute(); 
    std::chrono::milliseconds _state_done();


private: // members
    std::mutex _mutex;
    int _id;

public:
    int getId();

public: //Command Operations
    void addCommand(std::shared_ptr<Command> cmd);
    std::shared_ptr<Command> getFinishedCommand();
    bool isReady();
    bool isDone();
private:
    std::shared_ptr<Command> _cmd;

public: // public state management
    State_e getState();
private: // state managment
    State_e _state;
    void _setState(State_e);

};
} // namespace Assignment3


#endif